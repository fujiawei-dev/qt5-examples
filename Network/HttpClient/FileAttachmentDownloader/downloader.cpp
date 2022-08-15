#include "downloader.h"

FileAttachmentDownloader::FileAttachmentDownloader(QObject *parent) : QObject(parent) {
  m_networkAccessManager = new QNetworkAccessManager(this);
}

void FileAttachmentDownloader::downloadFile(const QString &url) {
  m_urls.append(url);

  if (!downloading) {
    downloading = true;
    m_buffer.clear();
    QNetworkRequest request;
    request.setUrl(m_urls.takeFirst());
    m_networkReply = m_networkAccessManager->get(request);
    connect(m_networkReply, &QNetworkReply::metaDataChanged, this, &FileAttachmentDownloader::onMetaDataChanged);
    connect(m_networkReply, &QNetworkReply::readyRead, this, &FileAttachmentDownloader::onReadReady);
    connect(m_networkReply, &QNetworkReply::finished, this, &FileAttachmentDownloader::onFinished);
  }
}

void FileAttachmentDownloader::onMetaDataChanged() {
  QList<QByteArray> headers = m_networkReply->rawHeaderList();
  for (const QByteArray &header : headers) {
    qDebug() << header << ":" << m_networkReply->rawHeader(header);
  }
}

void FileAttachmentDownloader::onReadReady() {
  QByteArray data = m_networkReply->readAll();
  qDebug() << "read ready:" << data.size();
  m_buffer.append(data);
}

void FileAttachmentDownloader::onFinished() {
  if (m_networkReply->error() == QNetworkReply::NoError) {
    QByteArray disposition = m_networkReply->rawHeader("Content-Disposition");
    auto fileName = disposition.split(';').last().split('=').last().replace("\"", "").trimmed();
    auto filePath = QDir::tempPath() + "/"+ fileName;

    QFile file(filePath);
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
      qDebug() << "open file failed" << file.errorString() << filePath;
    } else {
      file.write(m_buffer);
      file.close();
      emit fileDownloaded(filePath);
    }
  } else {
    qWarning() << "download file failed" << m_networkReply->errorString();
  }

  downloading = false;

  if (!m_urls.isEmpty()) {
    downloadFile(m_urls.takeFirst());
  }
}

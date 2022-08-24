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

  contentLength = m_networkReply->header(QNetworkRequest::ContentLengthHeader).toInt();
}

void FileAttachmentDownloader::onReadReady() {
  QByteArray data = m_networkReply->readAll();
  downloadedLength += data.size();
  m_buffer.append(data);

  emit refreshDownloadProgress((int) (downloadedLength / (contentLength / 100)));
}

void FileAttachmentDownloader::onFinished() {
  if (m_networkReply->error() == QNetworkReply::NoError) {
    QByteArray disposition = m_networkReply->rawHeader("Content-Disposition");
    QString fileName = disposition.split(';').last().split('=').last().replace("\"", "").trimmed();

    if (fileName.isEmpty()) {
      fileName = QFileInfo(m_networkReply->url().path()).fileName();
    }

    auto filePath = QDir::tempPath() + "/" + fileName;

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

  disconnect(m_networkReply, &QNetworkReply::metaDataChanged, this, &FileAttachmentDownloader::onMetaDataChanged);
  disconnect(m_networkReply, &QNetworkReply::readyRead, this, &FileAttachmentDownloader::onReadReady);
  disconnect(m_networkReply, &QNetworkReply::finished, this, &FileAttachmentDownloader::onFinished);

  downloading = false;
  contentLength = 0;
  downloadedLength = 0;

  if (!m_urls.isEmpty()) {
    downloadFile(m_urls.takeFirst());
  }
}

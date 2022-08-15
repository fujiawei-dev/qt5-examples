#ifndef FILE_ATTACHMENT__DOWNLOADER_H
#define FILE_ATTACHMENT__DOWNLOADER_H

#include <QDir>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class FileAttachmentDownloader : public QObject {
  Q_OBJECT
public:
  explicit FileAttachmentDownloader(QObject *parent = nullptr);

  Q_INVOKABLE void downloadFile(const QString &url);

signals:
  void fileDownloaded(const QString &filePath);

private slots:
  void onMetaDataChanged();
  void onReadReady();
  void onFinished();

private:
  bool downloading = false;
  QNetworkAccessManager *m_networkAccessManager;
  QNetworkReply *m_networkReply;
  QByteArray m_buffer;
  QStringList m_urls;
};

#endif//FILE_ATTACHMENT__DOWNLOADER_H

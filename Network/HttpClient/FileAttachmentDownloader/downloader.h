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
  void refreshDownloadProgress(int value);

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

  uint64_t downloadedLength = 0;
  uint64_t contentLength = 0;
};

#endif//FILE_ATTACHMENT__DOWNLOADER_H

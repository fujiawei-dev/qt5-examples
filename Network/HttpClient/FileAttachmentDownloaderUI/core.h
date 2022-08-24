#ifndef FILE_ATTACHMENT_DOWNLOADER_UI__CORE_H
#define FILE_ATTACHMENT_DOWNLOADER_UI__CORE_H

#include <QDebug>
#include <QObject>

#include "downloader.h"

class Core : public QObject {
  Q_OBJECT

public:
  explicit Core(QObject *parent = nullptr);

  Q_INVOKABLE void downloadFile(const QString &url = "");

public slots:
  void onFileDownloaded(const QString &filePath);

signals:
  void refreshDownloadProgress(int value);

private:
  FileAttachmentDownloader *downloader;
};

#endif//FILE_ATTACHMENT_DOWNLOADER_UI__CORE_H

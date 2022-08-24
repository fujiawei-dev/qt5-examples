#include <QTimer>

#include "core.h"

Core::Core(QObject *parent) : QObject(parent) {
  downloader = new FileAttachmentDownloader(this);
  connect(downloader, &FileAttachmentDownloader::fileDownloaded, this, &Core::onFileDownloaded);
  connect(downloader, &FileAttachmentDownloader::refreshDownloadProgress, this, &Core::refreshDownloadProgress);
}

void Core::onFileDownloaded(const QString &filePath) {
  qDebug() << "file downloaded:" << filePath;
}

void Core::downloadFile(const QString &url) {
  downloader->downloadFile("http://localhost:26535/file_attachment/exe");
}

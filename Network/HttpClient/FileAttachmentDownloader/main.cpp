#include <QCoreApplication>
#include <QProcess>
#include <QTimer>

#include "downloader.h"

class Core : public QObject {
  Q_OBJECT

public:
  Core(QObject *parent = nullptr) : QObject(parent) {
    downloader = new FileAttachmentDownloader(this);
    connect(downloader, &FileAttachmentDownloader::fileDownloaded, this, &Core::onFileDownloaded);
  };

public slots:
  void run() {
    downloader->downloadFile("http://localhost:26535/file_attachment/exe");

    for (int i = 0; i < 9; i++) {
      downloader->downloadFile("http://localhost:26535/file_attachment/ascii");
    }
  };

  void onFileDownloaded(const QString &filePath) {
    qDebug() << "file downloaded:" << filePath;

    if (filePath.endsWith(".exe")) {
      QProcess::startDetached(filePath);
    }

    if (++count == 10) {
      emit finished();
    }
  }

signals:
  void finished();

private:
  FileAttachmentDownloader *downloader;
  int count = 0;
};

#include "main.moc"

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  Core *core = new Core(&app);
  // Only for console app. This will run from the application event loop.
  QObject::connect(core, SIGNAL(finished()), &app, SLOT(quit()));
  QTimer::singleShot(0, core, SLOT(run()));

  return QCoreApplication::exec();
}

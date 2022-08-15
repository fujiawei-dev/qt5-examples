#include <QCoreApplication>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  QNetworkRequest request;
  request.setUrl(QUrl("http://localhost:26535/file_attachment/ascii"));

  QNetworkAccessManager manager;
  QNetworkReply *reply = manager.get(request);

  QObject::connect(reply, &QNetworkReply::metaDataChanged, [=] {
    QList<QByteArray> headers = reply->rawHeaderList();
    for (const QByteArray &header : headers) {
      qDebug() << header << ":" << reply->rawHeader(header);
    }
  });

  QFile file("temp.d");
  if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
    qDebug() << "open file failed";
    return 1;
  }

  QObject::connect(reply, &QNetworkReply::readyRead, [&] {
    file.write(reply->readAll());
  });

  QObject::connect(reply, &QNetworkReply::finished, [&] {
    QByteArray disposition = reply->rawHeader("Content-Disposition");
    auto filename = disposition.split(';').last().split('=').last().replace("\"", "").trimmed();
    // 暂时不支持解析 UTF-8 编码的情况
    if (!filename.isEmpty()) {
      file.rename(filename);
    }

    file.close();
    reply->deleteLater();

    QCoreApplication::quit();
  });

  return QCoreApplication::exec();
}

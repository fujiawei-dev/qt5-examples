#ifndef QCLOSE_EVENT__CORE_H
#define QCLOSE_EVENT__CORE_H

#include <QDebug>
#include <QObject>

class Core : public QObject {
  Q_OBJECT

public:
  explicit Core(QObject *parent = nullptr);
};

#endif//QCLOSE_EVENT__CORE_H

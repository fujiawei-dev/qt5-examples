#ifndef SYSTEM_TRAY_ICON__CORE_H
#define SYSTEM_TRAY_ICON__CORE_H

#include <QDebug>
#include <QObject>

class Core : public QObject {
  Q_OBJECT

public:
  explicit Core(QObject *parent = nullptr);
};

#endif//SYSTEM_TRAY_ICON__CORE_H

#ifndef COMBO_BOX__CORE_H
#define COMBO_BOX__CORE_H

#include <QDebug>
#include <QObject>

class Core : public QObject {
  Q_OBJECT

public:
  explicit Core(QObject *parent = nullptr);
};

#endif//COMBO_BOX__CORE_H

#ifndef TOOL_TIP__CORE_H
#define TOOL_TIP__CORE_H

#include <QDebug>
#include <QObject>

class Core : public QObject {
  Q_OBJECT

public:
  explicit Core(QObject *parent = nullptr);
};

#endif//TOOL_TIP__CORE_H

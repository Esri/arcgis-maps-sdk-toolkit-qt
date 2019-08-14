#ifndef NORTHARROWCONTROLLER_H
#define NORTHARROWCONTROLLER_H

#include <QObject>

class NorthArrowController : public QObject
{
  Q_OBJECT
  Q_DISABLE_COPY(NorthArrowController)

public:
  NorthArrowController(QObject* parent = nullptr);
  ~NorthArrowController();
};

#endif // NORTHARROWCONTROLLER_H

#ifndef NORTHARROW_H
#define NORTHARROW_H

#include <QQuickItem>

class NorthArrow : public QQuickItem
{
  Q_OBJECT
  Q_DISABLE_COPY(NorthArrow)

public:
  NorthArrow(QQuickItem *parent = nullptr);
  ~NorthArrow();
};

#endif // NORTHARROW_H

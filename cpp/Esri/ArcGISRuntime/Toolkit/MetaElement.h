#ifndef TOOLKIT_METAELEMENT_H
#define TOOLKIT_METAELEMENT_H

#include <QObject>
#include <QPersistentModelIndex>
#include <QPointer>

class QAbstractItemModel;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class MetaElement : public QObject
{
  Q_OBJECT
public:
  MetaElement(QModelIndex index,
              int customRole,
              QObject* trackedObject,
              QAbstractItemModel* parent);

signals:
  void propertyChanged();

private slots:
  void emitDataChanged();

private:
  QPersistentModelIndex m_index;
  int m_customRole;
  QPointer<QObject> m_trackedObject;
  QAbstractItemModel* m_parentModel;
};

} // Esri
} // ArcGISRuntime
} // Toolkit

#endif // TOOLKIT_METAELEMENT_H

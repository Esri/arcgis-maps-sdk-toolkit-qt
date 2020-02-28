#ifndef TOOLKIT_COORDIANTEEDITDELEGATE_H
#define TOOLKIT_COORDIANTEEDITDELEGATE_H

// Qt headers
#include <QItemDelegate>
#include <QPointer>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class CoordinateConversionController;

class CoordinateEditDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  CoordinateEditDelegate(QObject* parent = nullptr);
  ~CoordinateEditDelegate() override;

  void setController(CoordinateConversionController* c);
  CoordinateConversionController* controller() const;

  void setModelData(QWidget* editor,
                    QAbstractItemModel* model,
                    const QModelIndex& index) const override;

private:
  QPointer<CoordinateConversionController> m_controller;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // TOOLKIT_COORDIANTEEDITDELEGATE_H

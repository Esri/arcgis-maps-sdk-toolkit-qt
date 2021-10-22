#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_ARCGISRUNTIMEENVIRONMENTPROXY_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_ARCGISRUNTIMEENVIRONMENTPROXY_H

#include <QObject>

class ArcGISRuntimeEnvironmentProxy : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)
public:
  Q_INVOKABLE explicit ArcGISRuntimeEnvironmentProxy(QObject* parent = nullptr);
  ~ArcGISRuntimeEnvironmentProxy() override;

  QString apiKey() const;
  void setApiKey(const QString& apiKey);

signals:
  void apiKeyChanged();
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_ARCGISRUNTIMEENVIRONMENTPROXY_H

#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_POPUPVIEWDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_POPUPVIEWDEMOMODEL_H

#include "BaseDemoModel.h"
#include "FeatureLayer.h"
#include "PopupManager.h"
#include <QObject>
#include <QQmlEngine>

class PopupViewDemoModel : public BaseDemoModel
{
    Q_OBJECT
    Q_PROPERTY(
        QObject *popupManager READ popupManager_ WRITE setPopupManager_ NOTIFY popupManagerChanged)
    QML_ELEMENT
public:
    PopupViewDemoModel(QObject *parent = nullptr);
    ~PopupViewDemoModel() override;
    Esri::ArcGISRuntime::Map *initMap_(QObject *parent) const override;
    Esri::ArcGISRuntime::Scene *initScene_(QObject *parent) const override;
signals:
    void popupManagerChanged();

private:
    Esri::ArcGISRuntime::FeatureLayer *featureLayer = nullptr;
    QObject *m_popupManager;

    QObject *popupManager_();
    void setPopupManager_(QObject *);
private slots:
    void setUp();
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_POPUPVIEWDEMOMODEL_H

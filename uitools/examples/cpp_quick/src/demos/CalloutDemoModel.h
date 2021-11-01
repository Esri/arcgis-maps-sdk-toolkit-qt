#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_CALLOUTDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_CALLOUTDEMOMODEL_H

#include "BaseDemoModel.h"

#include <QObject>
#include <QQmlEngine>

class CalloutDemoModel : public BaseDemoModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    CalloutDemoModel(QObject *parent = nullptr);
    ~CalloutDemoModel() override;
    void setUp();

private:
    double m_xClickLoc;
    double m_yClickLoc;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_CALLOUTDEMOMODEL_H

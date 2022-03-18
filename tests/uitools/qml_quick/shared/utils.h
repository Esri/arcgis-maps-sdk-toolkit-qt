#ifndef ESRI_ARCGIS_RUNTIME_TOOLKIT_TESTS_UTILS_H
#define ESRI_ARCGIS_RUNTIME_TOOLKIT_TESTS_UTILS_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>

class Utils : public QObject
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit Utils(QObject* parent = nullptr);
  static QObject* createSingletonInstance(QQmlEngine* engine, QJSEngine* scriptEngine);

  Q_INVOKABLE QObject* findChild(QObject* parent, QString name);
  Q_INVOKABLE QString urlFileName(const QString& url);
};

#endif // ESRI_ARCGIS_RUNTIME_TOOLKIT_TESTS_UTILS_H

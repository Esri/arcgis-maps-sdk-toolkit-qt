#include "utils.h"

Utils::Utils(QObject* parent) :
  QObject(parent)
{
}

QObject* Utils::findChild(QObject* parent, QString name)
{
  return parent->findChild<QObject*>(name);
}

QString Utils::urlFileName(const QString& urlName)
{
  QUrl url(urlName);
  return url.fileName();
}

QObject* Utils::createSingletonInstance(QQmlEngine* engine, QJSEngine* scriptEngine)
{
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);
  return new Utils;
}

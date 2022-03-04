#ifndef UTILS_H
#define UTILS_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>

class Utils;

class Utils : public QObject
{
  Q_OBJECT
public:
  explicit Utils(QObject* parent = 0);
  static QObject* createSingletonInstance(QQmlEngine* engine, QJSEngine* scriptEngine);
  //static Utils* m_instance;

  Q_INVOKABLE QObject* findChild(QObject* parent, QString name);
  Q_INVOKABLE QString urlFileName(const QString url);

signals:

public slots:
};

#endif // UTILS_H

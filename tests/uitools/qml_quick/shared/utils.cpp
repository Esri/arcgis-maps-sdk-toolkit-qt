#include "utils.h"

/*!
 \internal
 \brief Utility class, mostly wrapping QT C++ functionalities not available in QML
 and expose them into QML environment.
 */
Utils::Utils(QObject* parent) :
  QObject(parent)
{
}

/*!
 \internal
 \brief Extract the file name from the \a urlName.
 */
QString Utils::urlFileName(const QString& urlName)
{
  QUrl url(urlName);
  return url.fileName();
}

/*!
 \internal
 \brief Creates a Singleton Instance that can be accesses without instantiating the class.
 \param engine
 \param scriptEngine 
 */
QObject* Utils::createSingletonInstance(QQmlEngine* engine, QJSEngine* scriptEngine)
{
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);
  return new Utils;
}

#ifndef SIGNALSYNCHRONIZER_H
#define SIGNALSYNCHRONIZER_H

#include <QObject>

class SignalSynchronizer : public QObject
{
  Q_OBJECT
public:
  template <typename Iterator, typename Signal>
  SignalSynchronizer(Iterator begin,
                     Iterator end,
                     Signal signal,
                     QObject* parent = nullptr) :
    QObject(parent)
  {
    for (; begin != end; ++begin)
    {
      ++m_counter;
      QObject::connect(
          *begin, signal, this, &SignalSynchronizer::decrementCounter);
    }
  }

  Q_SIGNAL void ready(QPrivateSignal);

private:
  Q_SLOT void decrementCounter();
  unsigned int m_counter{0};
};

#endif // SIGNALSYNCHRONIZER_H

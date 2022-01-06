#include "SignalSynchronizer.h"

void SignalSynchronizer::decrementCounter()
{
  m_counter--;
  if (m_counter == 0)
    emit ready({});
}

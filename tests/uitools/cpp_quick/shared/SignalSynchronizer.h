/*******************************************************************************
 *  Copyright 2012-2022 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/
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

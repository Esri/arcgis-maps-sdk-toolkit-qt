# Copyright 2012-2020 Esri
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
include($$PWD/../common/common.pri)

QT += widgets webenginewidgets svg

TOOLKITWIDGETS_SRC = $$PWD/src

INCLUDEPATH += $$PWD/toolkitwidgets/widgets $$WIDGETPATH

INCLUDEPATH += $$TOOLKITWIDGETS_SRC \
               $$TOOLKITWIDGETS_SRC/Internal

DEPENDPATH += $$TOOLKITWIDGETS_SRC \
              $$TOOLKITWIDGETS_SRC/Internal

HEADERS += $$files($$TOOLKITWIDGETS_SRC/*.h) \
           $$files($$TOOLKITWIDGETS_SRC/Internal/*.h)

SOURCES += $$files($$TOOLKITWIDGETS_SRC/*.cpp) \
           $$files($$TOOLKITWIDGETS_SRC/Internal/*.cpp)

FORMS += $$files($$TOOLKITWIDGETS_SRC/*.ui) \
         $$files($$TOOLKITWIDGETS_SRC/Internal/*.ui)

DEFINES += WIDGETS_ARCGISRUNTIME_TOOLKIT

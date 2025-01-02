# Copyright 2012-2025 Esri
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

# file guard to avoid double inclusion
!defined(TOOLKIT_COMMON_INCLUDED, var) {

    QT += qml

    TOOLKIT_COMMON_SRC = $$PWD/src

    INCLUDEPATH += $$TOOLKIT_COMMON_SRC \
                   $$TOOLKIT_COMMON_SRC/Internal

    DEPENDPATH += $$TOOLKIT_COMMON_SRC \
                  $$TOOLKIT_COMMON_SRC/Internal

    HEADERS += $$files($$TOOLKIT_COMMON_SRC/*.h) \
               $$files($$TOOLKIT_COMMON_SRC/Internal/*.h)

    SOURCES += $$files($$TOOLKIT_COMMON_SRC/*.cpp) \
               $$files($$TOOLKIT_COMMON_SRC/Internal/*.cpp)

    RESOURCES += $$PWD/images/esri_arcgisruntime_toolkit_common_images.qrc

    TOOLKIT_COMMON_INCLUDED = true
}

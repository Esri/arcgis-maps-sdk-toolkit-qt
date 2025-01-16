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

android|ios {
    message("toolkitwidgets.pri is not usable on mobile platforms.")
} else {
    QT += widgets webenginewidgets svg

    TOOLKITWIDGETS_BASE_SRC = $$PWD/src
    TOOLKITWIDGETS_TOOLKIT_SRC = $$TOOLKITWIDGETS_BASE_SRC/Esri/ArcGISRuntime/Toolkit

    INCLUDEPATH += $$TOOLKITWIDGETS_BASE_SRC

    DEPENDPATH += $$TOOLKITWIDGETS_BASE_SRC

    HEADERS += $$files($$TOOLKITWIDGETS_TOOLKIT_SRC/*.h) \
               $$files($$TOOLKITWIDGETS_TOOLKIT_SRC/Internal/*.h)

    SOURCES += $$files($$TOOLKITWIDGETS_TOOLKIT_SRC/*.cpp) \
               $$files($$TOOLKITWIDGETS_TOOLKIT_SRC/Internal/*.cpp)

    FORMS += $$files($$TOOLKITWIDGETS_TOOLKIT_SRC/*.ui) \
             $$files($$TOOLKITWIDGETS_TOOLKIT_SRC/Internal/*.ui)

    DEFINES += WIDGETS_ARCGISRUNTIME_TOOLKIT
}

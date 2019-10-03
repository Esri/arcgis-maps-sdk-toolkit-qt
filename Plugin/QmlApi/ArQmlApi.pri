###############################################################################
# Copyright 2012-2019 Esri
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
###############################################################################

# This file is a part of the AR features provides in the toolkit.
# This file must be imported in the project to use the QML API of the
# ArcGIS Runtime SDK for Qt. See AR.md for details.

isEmpty(AR_TOOLKIT_SOURCE_PATH) {
    error(AR_TOOLKIT_SOURCE_PATH is not set)
}

include($$AR_TOOLKIT_SOURCE_PATH/Common/AR/AR.pri)

HEADERS += \
    $$AR_TOOLKIT_SOURCE_PATH/Plugin/QmlApi/include/QmlArcGISArView.h

SOURCES += \
    $$AR_TOOLKIT_SOURCE_PATH/Plugin/QmlApi/source/QmlArcGISArView.cpp

OTHER_FILES += \
    $$AR_TOOLKIT_SOURCE_PATH/Plugin/QmlApi/qml/ArcGISArView.qml

RESOURCES += \
    $$AR_TOOLKIT_SOURCE_PATH/Plugin/QmlApi/qml/arQml.qrc

INCLUDEPATH += $$AR_TOOLKIT_SOURCE_PATH/Plugin/QmlApi/include
DEPENDPATH += $$AR_TOOLKIT_SOURCE_PATH/Plugin/QmlApi/include

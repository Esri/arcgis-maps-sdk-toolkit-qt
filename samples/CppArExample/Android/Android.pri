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

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

ANDROID_PACKAGE_SOURCE_DIR = $$PWD

OTHER_FILES += \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/drawable-ldpi/icon.png \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/drawable-mdpi/icon.png \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/drawable-hdpi/icon.png \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/drawable-xhdpi/icon.png \
    $$ANDROID_PACKAGE_SOURCE_DIR/res/drawable-xxhdpi/icon.png

DISTFILES += \
    $$ANDROID_PACKAGE_SOURCE_DIR/AndroidManifest.xml \
    $$ANDROID_PACKAGE_SOURCE_DIR/gradle/wrapper/gradle-wrapper.jar \
    $$ANDROID_PACKAGE_SOURCE_DIR/gradlew \
    $$ANDROID_PACKAGE_SOURCE_DIR/build.gradle \
    $$ANDROID_PACKAGE_SOURCE_DIR/gradle/wrapper/gradle-wrapper.properties \
    $$ANDROID_PACKAGE_SOURCE_DIR/gradlew.bat

ANDROID_LIBS = $$dirname(QMAKE_QMAKE)/../lib
ANDROID_EXTRA_LIBS += \
    $$ANDROID_LIBS/libssl.so \
    $$ANDROID_LIBS/libcrypto.so

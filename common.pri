INCLUDEPATH += $$PWD/cpp

CPPPATH = $$PWD/cpp/Esri/ArcGISRuntime/Toolkit

HEADERS += $$CPPPATH/CoordinateConversionConstants.h \
           $$CPPPATH/CoordinateConversionController.h \
           $$CPPPATH/CoordinateConversionOption.h \
           $$CPPPATH/CoordinateConversionResult.h \
           $$CPPPATH/CoordinateOptionDefaults.h \
           $$CPPPATH/GenericListModel.h \
           $$CPPPATH/NorthArrowController.h

SOURCES += $$CPPPATH/CoordinateConversionConstants.cpp \
           $$CPPPATH/CoordinateConversionController.cpp \
           $$CPPPATH/CoordinateConversionOption.cpp \
           $$CPPPATH/CoordinateConversionResult.cpp \
           $$CPPPATH/CoordinateOptionDefaults.cpp \
           $$CPPPATH/GenericListModel.cpp \
           $$CPPPATH/NorthArrowController.cpp

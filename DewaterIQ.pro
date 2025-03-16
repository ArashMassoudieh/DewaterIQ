QT       += core websockets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console

#DEFINES += USE_WEBSOCKETS

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AquaArray.cpp \
    DataStructure.cpp \
    ExpressionCalculator.cpp \
    System.cpp \
    aquatable.cpp \
    main.cpp \
    websocketserver.cpp \
    wholisticdewateringcalculator.cpp


HEADERS += \
    AquaArray.h \
    DataStructure.h \
    ExpressionCalculator.h \
    ExpressionCalculator.hpp \
    System.h \
    aquatable.h \
    websocketserver.h \
    wholisticdewateringcalculator.h


FORMS +=


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

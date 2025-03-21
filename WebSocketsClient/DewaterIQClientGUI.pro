QT = core websockets gui widgets charts

CONFIG += c++17
CONFIG -= thread

DEFINES += DESKTOP #WASM

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../AquaArray.cpp \
        ../aquatable.cpp \
        aquaplotter.cpp \
        aquatablemodel.cpp \
        main_GUI.cpp \
        mainform.cpp \
        websocketclient.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../AquaArray.h \
    ../aquatable.h \
    aquaplotter.h \
    aquatablemodel.h \
    mainform.h \
    websocketclient.h

INCLUDEPATH += ../

FORMS += \
    mainform.ui

contains(DEFINES, WASM): QMAKE_LFLAGS += "-sDISABLE_EXCEPTION_CATCHING=0"

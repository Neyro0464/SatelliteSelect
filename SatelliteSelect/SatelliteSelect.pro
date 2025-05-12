QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CalcMethodUtils.cpp \
        CoordWorkerUtils.cpp \
        FileTxtReader.cpp \
        SatelliteSelect.cpp \
        Sgp4CalcMethod.cpp \
        Source.cpp \
        StationClass.cpp \
        myMath.cpp \
        myTime.cpp \
        test.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    CoordWorkerUtils.h \
    ErrorHandler.h \
    FileTxtReader.h \
    IReader.h \
    SatelliteSelect.h \
    Sgp4CalcMethod.h \
    StationClass.h \
    myVector.h

DISTFILES += \
    TLE.txt

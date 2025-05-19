QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CalcMethodLib/CalcMethodUtils.cpp \
        CalcMethodLib/Sgp4CalcMethod.cpp \
        CalcMethodLib/myMath.cpp \
        CalcMethodLib/myTime.cpp \
        CoordWorkerUtils.cpp \
        FileTxtReader.cpp \
        StationClass.cpp \
        run_test/CalcMethodTest.cpp \
        run_test/FileReaderTest.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    CalcMethodLib/ICalcMethod.h \
    CalcMethodLib/ICalcMethodSGP4.h \
    CalcMethodLib/Sgp4CalcMethod.h \
    CalcMethodLib/myVector.h \
    CoordWorkerUtils.h \
    FileTxtReader.h \
    IReader.h \
    StationClass.h

DISTFILES += \
    TLE.txt \
    run_test/testTLE.txt \
    testTLE.txt

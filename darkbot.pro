#-------------------------------------------------
#
# Project created by QtCreator 2014-09-20T10:00:54
#
#-------------------------------------------------

QT       += core gui widgets webkit webkitwidgets

TARGET = darkbot
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    world.cpp \
    workerthread.cpp

HEADERS  += mainwindow.h \
    world.h \
    workerthread.h

FORMS    += mainwindow.ui

RESOURCES += resource.qrc

gcc {
    QMAKE_CXXFLAGS += -std=c++11 -fopenmp
    QMAKE_CXXFLAGS_RELEASE += -O3
    QMAKE_LFLAGS +=  -fopenmp
}

msvc {
    QMAKE_CXXFLAGS += /openmp
    QMAKE_CXXFLAGS_RELEASE += /O2
}

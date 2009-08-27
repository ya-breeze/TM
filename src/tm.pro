#DEFINES += TABLET
TEMPLATE = app
TARGET = tm
QT += core \
    gui \
    network
INCLUDEPATH += qtsingleapp
HEADERS += TabletWindow.h \
    qtsingleapp/qtlocalpeer.h \
    qtsingleapp/qtlockedfile.h \
    qtsingleapp/qtsingleapplication.h \
    smart.h \
    LastActs.h \
    ActTree.h \
    DayActivities.h \
    Activities.h \
    ChangableObject.h \
    utils.h \
    Activity.h \
    Saver.h \
    Task.h \
    TaskTree.h \
    mainwindow.h
SOURCES += TabletWindow.cpp \
    qtsingleapp/qtlocalpeer.cpp \
    qtsingleapp/qtlockedfile.cpp \
    qtsingleapp/qtlockedfile_unix.cpp \
    qtsingleapp/qtsingleapplication.cpp \
    LastActs.cpp \
    ActTree.cpp \
    DayActivities.cpp \
    Activities.cpp \
    utils.cpp \
    Activity.cpp \
    Saver.cpp \
    Task.cpp \
    TaskTree.cpp \
    main.cpp \
    mainwindow.cpp
FORMS += tabletmain.ui \
    mainwindow.ui
RESOURCES += tm.qrc
TRANSLATIONS = tm.ts
QMAKE_CXXFLAGS += -g3
unix { 
    # VARIABLES
    isEmpty(PREFIX):PREFIX = /usr/local
    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share
    DEFINES += DATADIR=\\\"$$DATADIR\\\" \
        PKGDATADIR=\\\"$$PKGDATADIR\\\"
    
    # MAKE INSTALL
    INSTALLS += target \
        desktop \
        iconxpm
    target.path = $$BINDIR
    desktop.path = $$DATADIR/applications/hildon
    desktop.files += tm.desktop
    iconxpm.path = $$DATADIR/pixmap
    iconxpm.files += tm.xpm
}

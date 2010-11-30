TEMPLATE = app
TARGET = tm
QT += core \
    gui \
    network sql
CONFIG += qxt link_pkgconfig
QXT += core gui
PKGCONFIG = QJson
INCLUDEPATH += qtsingleapp
HEADERS += CalendarShower.h \
    dlgcalendar.h \
    PlannedTasks.h \
    PlannedTaskList.h \
    Filter.h \
    Category.h \
    PlannedTime.h \
    TimeScale.h \
    HideDone.h \
    qtsingleapp/qtlocalpeer.h \
    qtsingleapp/qtlockedfile.h \
    qtsingleapp/qtsingleapplication.h \
    smart.h \
    LastActs.h \
    DayActivities.h \
    Activities.h \
    ChangableObject.h \
    utils.h \
    Activity.h \
    Saver.h \
    Task.h \
    TaskTree.h \
    mainwindow.h \
    CategoryTree.h \
    server.h \
    CategoryEdit.h \
    httpprocessor.h \
    iconcache.h TabletDlgTask.h \
    dlgiconchoose.h \
    constants.h
SOURCES += CalendarShower.cpp TabletDlgTask.cpp\
    dlgcalendar.cpp \
    PlannedTasks.cpp \
    PlannedTaskList.cpp \
    Filter.cpp \
    HideDone.cpp \
    Category.cpp \
    PlannedTime.cpp \
    TimeScale.cpp \
    server.cpp \
    qtsingleapp/qtlocalpeer.cpp \
    qtsingleapp/qtlockedfile.cpp \
    qtsingleapp/qtlockedfile_unix.cpp \
    qtsingleapp/qtsingleapplication.cpp \
    LastActs.cpp \
    DayActivities.cpp \
    Activities.cpp \
    utils.cpp \
    Activity.cpp \
    Saver.cpp \
    Task.cpp \
    TaskTree.cpp \
    main.cpp \
    mainwindow.cpp \
    CategoryTree.cpp \
    CategoryEdit.cpp \
    httpprocessor.cpp \
    iconcache.cpp \
    dlgiconchoose.cpp
FORMS += dlgcalendar.ui \
    PlannedTasks.ui \
    mainwindow.ui \
    CatEdit.ui TabletDlgTask.ui \
    dlgiconchoose.ui
RESOURCES += tm.qrc
TRANSLATIONS = tm_ru.ts
QMAKE_CXXFLAGS += -g3
unix { 
    # VARIABLES
    isEmpty(PREFIX):PREFIX = /usr/
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

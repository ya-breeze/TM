TEMPLATE = app
TARGET = tm
QT += core \
    gui \
    network sql
CONFIG += qxt link_pkgconfig
LIBS += -lQxtGui
QXT += core gui
INCLUDEPATH += qtsingleapp /usr/include/qxt /usr/include/qxt/QxtCore
HEADERS += CalendarShower.h \
    dlgcalendar.h \
    Filter.h \
    Category.h \
    PlannedTime.h \
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
    CategoryEdit.h \
    iconcache.h TabletDlgTask.h \
    dlgiconchoose.h \
    constants.h \
    diagramm/ConvertorFromSeconds.h \
    diagramm/Convertor.h              \
    diagramm/diagramm.h \
    diagramm/diagrammitem.h \
    diagramm/diagrammiteminternal.h \
    diagramm/ListBuilder.h \
    diagrammdialog.h
SOURCES += CalendarShower.cpp TabletDlgTask.cpp\
    dlgcalendar.cpp \
    Filter.cpp \
    HideDone.cpp \
    Category.cpp \
    PlannedTime.cpp \
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
    iconcache.cpp \
    dlgiconchoose.cpp \
    diagramm/diagramm.cpp \
    diagramm/diagrammitem.cpp \
    diagramm/diagrammiteminternal.cpp \
    diagramm/ListBuilder.cpp \
    diagrammdialog.cpp
FORMS += dlgcalendar.ui \
    PlannedTasks.ui \
    mainwindow.ui \
    CatEdit.ui TabletDlgTask.ui \
    dlgiconchoose.ui \
    diagrammdialog.ui
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

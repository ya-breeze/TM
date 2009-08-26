TEMPLATE = app
TARGET = TM
QT += core \
    gui
HEADERS += LastActs.h \
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
SOURCES += LastActs.cpp \
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
FORMS += mainwindow.ui
RESOURCES += tm.qrc
TRANSLATIONS = tm.ts
QMAKE_CXXFLAGS += -g3

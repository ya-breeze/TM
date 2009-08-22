TEMPLATE = app
TARGET = TM
QT += core \
    gui
HEADERS += Activities.h \
    ChangableObject.h \
    utils.h \
    Activity.h \
    Saver.h \
    Task.h \
    TaskTree.h \
    mainwindow.h
SOURCES += Activities.cpp \
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

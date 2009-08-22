TEMPLATE = app
TARGET = TM
QT += core \
    gui
HEADERS += Saver.h \
    exceptions.h \
    Task.h \
    TaskTree.h \
    mainwindow.h
SOURCES += Saver.cpp \
    Task.cpp \
    TaskTree.cpp \
    main.cpp \
    mainwindow.cpp
FORMS += mainwindow.ui
RESOURCES += tm.qrc
TRANSLATIONS = tm.ts
QMAKE_CXXFLAGS += -g3

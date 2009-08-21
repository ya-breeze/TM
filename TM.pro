TEMPLATE = app
TARGET = TM
QT += core \
    gui
HEADERS += exceptions.h \
    Task.h \
    TaskTree.h \
    mainwindow.h
SOURCES += Task.cpp \
    TaskTree.cpp \
    main.cpp \
    mainwindow.cpp
FORMS += mainwindow.ui
RESOURCES += tm.qrc
TRANSLATIONS = tm.ts

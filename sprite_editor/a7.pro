#-------------------------------------------------
#
# Project created by QtCreator 2018-11-03T14:08:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = a7
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++1z

SOURCES += \
        main.cpp \
        main_window.cpp \
    frame.cpp \
    color.cpp \
    pencil.cpp \
    canvas.cpp \
    sprite.cpp \
    sprite_dialog.cpp \
    canvas_scene.cpp \
    pixel_buffer.cpp \
    fill.cpp \
    eraser.cpp \
    dropper.cpp \
    rectangle.cpp \
    controller.cpp

HEADERS += \
        main_window.h \
    draw_command.h \
    frame.h \
    color.h \
    pencil.h \
    pixel.h \
    canvas.h \
    sprite.h \
    sprite_dialog.h \
    canvas_scene.h \
    pixel_buffer.h \
    fill.h \
    eraser.h \
    dropper.h \
    rectangle.h \
    controller.h

FORMS += \
        main_window.ui \
    sprite_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .clang-format \
    .clang-tidy \
    Images/redo_32.png \
    Images/undo_32.png

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Libraries/gifski-master/target/release/release/ -lgifski
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Libraries/gifski-master/target/release/debug/ -lgifski
else:unix: LIBS += -L$$PWD/Libraries/gifski-master/target/release/ -lgifski

INCLUDEPATH += $$PWD/Libraries/gifski-master
DEPENDPATH += $$PWD/Libraries/gifski-master

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/Libraries/gifski-master/target/release/release/libgifski.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/Libraries/gifski-master/target/release/debug/libgifski.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/Libraries/gifski-master/target/release/release/gifski.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/Libraries/gifski-master/target/release/debug/gifski.lib
else:unix: PRE_TARGETDEPS += $$PWD/Libraries/gifski-master/target/release/libgifski.a

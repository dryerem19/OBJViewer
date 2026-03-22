QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    material.cpp \
    materiallibrary.cpp \
    mesh.cpp \
    model.cpp \
    objectlist.cpp \
    orbitcamera.cpp \
    scene.cpp \
    scenewidget.cpp

HEADERS += \
    mainwindow.h \
    material.h \
    materiallibrary.h \
    mesh.h \
    model.h \
    objectlist.h \
    orbitcamera.h \
    scene.h \
    scenewidget.h \
    vertex.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc

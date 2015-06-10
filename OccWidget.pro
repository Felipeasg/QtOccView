#-------------------------------------------------
#
# Project created by QtCreator 2015-06-01T14:43:09
#
#-------------------------------------------------

QT       += core gui opengl x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OccWidget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    occview.cpp

HEADERS  += mainwindow.h \
    occview.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/include/oce

LIBS += -L/usr/lib64/oce -lTKernel -lPTKernel -lTKMath -lTKService -lTKV3d -lTKOpenGl \
        -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
        -lTKSTEPBase -lTKShapeSchema -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
        -lTKXSBase -lTKPShape -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset \
        -lfreeimage -lX11

RESOURCES += \
    resources.qrc

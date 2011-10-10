#-------------------------------------------------
#
# Project created by QtCreator 2011-10-02T12:49:07
#
#-------------------------------------------------

QT       += core gui

TARGET = BrowseURL
TEMPLATE = app

SOURCES += main.cpp \
    PreferencesDialog.cpp \
    TrayMenu.cpp \
    AboutDialog.cpp \
    Application.cpp \
    DomainModel.cpp \
    Domain.cpp \
    DomainDelegate.cpp

HEADERS  += \
    constants.h \
    PreferencesDialog.h \
    AboutDialog.h \
    Application.h \
    DomainModel.h \
    TrayMenu.h \
    Domain.h \
    DomainDelegate.h

FORMS    += \
    PreferencesDialog.ui \
    AboutDialog.ui

OTHER_FILES += \
    Info.plist \
    heart.svg \
    heart.ico \
    heart.icns \
    osx-open-in-finder.scpt

RESOURCES += \
    resources.qrc

macx {
ICON = heart.icns
QMAKE_INFO_PLIST = Info.plist
APP_QML_FILES.files = osx-open-in-finder.scpt
APP_QML_FILES.path = Contents/Resources
QMAKE_BUNDLE_DATA += APP_QML_FILES
}





























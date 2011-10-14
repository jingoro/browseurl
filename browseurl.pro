QT += core gui

TARGET = BrowseURL
TEMPLATE = app

HEADERS += \
    src/constants.h \
    src/AboutDialog.h \
    src/Application.h \
    src/Domain.h \
    src/DomainDelegate.h \
    src/DomainModel.h \
    src/OsProxy.h \
    src/PreferencesDialog.h \
    src/TrayMenu.h \
    src/MacProxyDelegate.h

SOURCES += \
    src/main.cpp \
    src/AboutDialog.cpp \
    src/Application.cpp \
    src/Domain.cpp \
    src/DomainDelegate.cpp \
    src/DomainModel.cpp \
    src/OsProxy.cpp \
    src/PreferencesDialog.cpp \
    src/TrayMenu.cpp \
    src/MacProxyDelegate.mm

FORMS += \
    forms/AboutDialog.ui \
    forms/PreferencesDialog.ui

OTHER_FILES += \
    resources/Info.plist \
    resources/heart.svg \
    resources/heart.ico \
    resources/heart.icns

RESOURCES += \
    resources/resources.qrc

macx {

OBJECTIVE_SOURCES += \
    src/MacProxy.mm

HEADERS += \
    src/MacProxy.h

LIBS += -framework AppKit

ICON = resources/heart.icns

QMAKE_INFO_PLIST = resources/Info.plist
APP_QML_FILES.files =
APP_QML_FILES.path = Contents/Resources
QMAKE_BUNDLE_DATA += APP_QML_FILES

}






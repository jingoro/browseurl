#include <QtGui>

#include "TrayMenu.h"

TrayMenu::TrayMenu(QWidget *parent) :
    QMenu(parent)
{
    preferencesAction = new QAction(tr("Preferences..."), this);
    connect(preferencesAction, SIGNAL(triggered()), qApp, SLOT(showPreferencesDialog()));
    aboutAction = new QAction(tr("About BrowseURL..."), this);
    connect(aboutAction, SIGNAL(triggered()), qApp, SLOT(showAboutDialog()));
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    addAction(preferencesAction);
    addSeparator();
    addAction(aboutAction);
    addSeparator();
    addAction(quitAction);
}

TrayMenu::~TrayMenu()
{
    delete preferencesAction;
    delete aboutAction;
    delete quitAction;
}

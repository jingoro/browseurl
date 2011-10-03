#include <QtGui>

#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent) :
    QMenu(parent)
{
    aboutAction = new QAction(tr("About BrowseURL..."), this);
    connect(aboutAction, SIGNAL(triggered()), qApp, SLOT(showAboutDialog()));
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    addAction(aboutAction);
    addSeparator();
    addAction(quitAction);
}

TrayMenu::~TrayMenu()
{
    delete aboutAction;
    delete quitAction;
}

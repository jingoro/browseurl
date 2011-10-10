#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QMenu>

class TrayMenu : public QMenu
{
    Q_OBJECT

public:
    explicit TrayMenu(QWidget *parent = 0);
    ~TrayMenu();

private:
    QAction *preferencesAction;
    QAction *aboutAction;
    QAction *quitAction;
};

#endif // TRAYMENU_H

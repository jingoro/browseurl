#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QTime>

#include "aboutdialog.h"
#include "traymenu.h"
#include "constants.h"

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application( int &argc, char ** argv );

public slots:
    void showAboutDialog();

protected:
    bool event( QEvent * );

private:
    void createTrayMenu();
    void openUrl( const QUrl &url );
    bool openUrlThrottle();
    bool getDomainFromUrl( const QUrl &url, QString *domain, QString *domainPath );
    void openPathInExplorer( const QString &path );
    void showError( const QString &message );

    QTime           openUrlTime;
    int             openUrlCount;

    AboutDialog     *aboutDialog;
    TrayMenu        *trayMenu;
    QSystemTrayIcon *trayIcon;
};

#endif // APPLICATION_H

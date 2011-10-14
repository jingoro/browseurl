#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QTime>

#include "constants.h"

#include "AboutDialog.h"
#include "DomainModel.h"
#include "OsProxy.h"
#include "PreferencesDialog.h"
#include "TrayMenu.h"

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application( int &argc, char ** argv );
    ~Application();

public slots:
    void showAboutDialog();
    void showPreferencesDialog();
    void updateSettings();
    void copyLinksFromFiles( const QStringList & files );

protected:
    bool event( QEvent * );

private:
    OsProxy * createOsProxy();
    void createTrayMenu();
    void openUrl( const QUrl &url );
    bool openUrlThrottle();
    bool getDomainFromUrl( const QUrl &url, QString *domain, QString *domainPath );
    void openPathInExplorer( const QString &path );
    void showError( const QString &message );
    QString createCopyLink( const QString &path ) const;

    void readSettings();
    void writeSettings();
    void setDefaultSettings();

    QTime openUrlTime;
    int   openUrlCount;

    AboutDialog       *aboutDialog;
    DomainModel       *domainModel;
    OsProxy           *osProxy;
    PreferencesDialog *preferencesDialog;
    TrayMenu          *trayMenu;
    QSystemTrayIcon   *trayIcon;
};

#endif // APPLICATION_H

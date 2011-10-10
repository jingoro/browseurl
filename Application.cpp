#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileOpenEvent>
#include <QMessageBox>
#include <QProcess>
#include <QUrl>

#include "Application.h"
#include "Domain.h"

Application::Application( int &argc, char **argv ) :
    QApplication(argc, argv)
{
    aboutDialog = 0;
    preferencesDialog = 0;
    openUrlTime = QTime();
    openUrlCount = 0;
    setQuitOnLastWindowClosed(false);
    createTrayMenu();

    // Testing
    showPreferencesDialog();
}

void Application::createTrayMenu()
{
    trayMenu = new TrayMenu( 0 );
    trayIcon = new QSystemTrayIcon( 0 );
    trayIcon->setContextMenu( trayMenu );
    trayIcon->setIcon( QIcon( ":/heart.svg" ) );
    trayIcon->show();
}

void Application::showAboutDialog()
{
    if ( ! aboutDialog ) {
        aboutDialog = new AboutDialog( 0 );
    }
    aboutDialog->show();
}

void Application::showPreferencesDialog()
{
    if ( ! preferencesDialog ) {
        preferencesDialog = new PreferencesDialog( 0 );
    }
    preferencesDialog->show();
}

bool Application::event( QEvent *event )
{
    if ( event->type() == QEvent::FileOpen ) {
        QUrl url = static_cast<QFileOpenEvent *>( event )->url();
        if ( ! url.isEmpty() ) {
            if ( url.scheme() == QString( "file" ) ) {
                copyLink( url.path() );
                return true;
            } else if ( url.scheme() == QString( BROWSEURL_SCHEME ) ) {
                openUrl( url );
                return true;
            }
        }
    }
    return QApplication::event( event );
}

void Application::openUrl( const QUrl &url )
{
    if ( openUrlThrottle() ) {
        qDebug("Throttling...");
        return;
    }

    QString domain;
    QString domainPath;

    if ( ! getDomainFromUrl( url, &domain, &domainPath ) ) {
        showError( tr("No matching domains for URL %1" ).
                   arg( url.toString() ) );
        return;
    }

    QString requestPath = QDir::cleanPath(domainPath + url.path());

    if ( ! requestPath.startsWith(domainPath) ) {
        showError( tr("Invalid request path %1 for URL %2" ).
                   arg( requestPath ).arg( url.toString() ) );
        return;
    }

    if ( ! QFile(requestPath).exists() ) {
        showError( tr("Path %1 does not exist for URL %2" ).
                   arg( requestPath ).arg( url.toString() ) );
        return;
    }

    openPathInExplorer( requestPath );
}

bool Application::openUrlThrottle()
{
    if ( openUrlTime.isNull() ||
         openUrlTime.elapsed() > BROWSEURL_THROTTLE_TIMEOUT ) {
        openUrlCount = 1;
        openUrlTime = QTime();
        openUrlTime.start();
        return false;
    } else {
        openUrlCount ++;
        return ( openUrlCount > BROWSEURL_THROTTLE_COUNT );
    }
}

bool Application::getDomainFromUrl( const QUrl &url,
                                    QString *aDomain, QString *domainPath )
{
    // TODO
    QString testDomain = url.host();
    QList < Domain > domains = preferencesDialog->getDomainModel()->getDomains();

    foreach ( const Domain &domain, domains ) {
        if ( testDomain == domain.getDomain() ) {
            *aDomain = domain.getDomain();
            *domainPath = domain.getLocalPath();
            return true;
        }
    }
    return false;
}

void Application::openPathInExplorer( const QString &path )
{
#if defined( Q_OS_WIN )
    showError( tr( "Unsupported Platform") );
#elif defined( Q_OS_MAC )
    QStringList args;
    args << ( applicationDirPath() + "/../Resources/osx-open-in-finder.scpt" );
    args << path;
    QProcess::execute( QString( "/usr/bin/osascript" ), args );
#else
    // Linux/UNIX ?
    showError( tr( "Unsupported Platform" ) );
#endif
}

void Application::showError( const QString &message )
{
    QMessageBox::critical( 0, tr( "BrowseURL Error" ), message );
}

void Application::copyLink( const QString &path )
{
    qDebug( path.toLatin1() );

    QList < Domain > domains = preferencesDialog->getDomainModel()->getDomains();

    foreach ( const Domain &domain, domains ) {
        if ( path.startsWith( domain.getLocalPath() ) ) {
             QUrl url;
             url.setScheme( QString( BROWSEURL_SCHEME ) );
             url.setHost( domain.getDomain() );
             url.setPath( path.mid( domain.getLocalPath().length() ) );
             QClipboard *clipboard = QApplication::clipboard();
             clipboard->setText( QString( url.toEncoded() ) );
             return;
        }
    }
    showError( tr( "No matching BrowseURL domain for local path %1" ).
               arg( path ) );
}

#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileOpenEvent>
#include <QMessageBox>
#include <QProcess>
#include <QUrl>

#include "application.h"

Application::Application( int &argc, char **argv ) :
    QApplication(argc, argv)
{
    aboutDialog = 0;
    openUrlTime = QTime();
    openUrlCount = 0;
    setQuitOnLastWindowClosed(false);
    createTrayMenu();
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
                                    QString *domain, QString *domainPath )
{
    // TODO
    QString testDomain = url.host();
    if ( testDomain == QString("home") ) {
        *domain     = QString("home");
        *domainPath = QDir::homePath();
        return true;
    }
    if ( testDomain == QString("aerofs") ) {
        *domain     = QString("aerofs");
        *domainPath = QDir::homePath() + "/AeroFS";
        return true;
    }
    if ( testDomain == QString("dropbox") ) {
        *domain     = QString("dropbox");
        *domainPath = QDir::homePath() + "/Dropbox";
        return true;
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
    QClipboard *clipboard = QApplication::clipboard();

    QString testdir = QDir::homePath();

    if ( path.startsWith( testdir ) ) {
        QUrl url;
        url.setScheme( QString( BROWSEURL_SCHEME ) );
        url.setHost( QString( "home" ) );
        url.setPath( path.mid( testdir.length() ) );
        clipboard->setText( QString( url.toEncoded() ) );
    } else {
        showError( tr( "No matching BrowseURL domain for local path %1" ).
                   arg( path ) );
    }
}

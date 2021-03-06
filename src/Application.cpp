#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileOpenEvent>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QStringList>
#include <QUrl>

#include "Application.h"

#if defined( Q_WS_MAC )
#include "MacProxy.h"
#elif defined( Q_OS_WIN )
#error TODO Unsupported platform!
#elif defined( Q_OS_UNIX )
#error TODO Unsupported platform!
#else
#error Unsupported platform!
#endif

Application::Application( int &argc, char **argv ) :
    QApplication( argc, argv ),
    osProxy( createOsProxy() ),
    domainModel( new DomainModel() ),
    aboutDialog( 0 ),
    preferencesDialog( 0 ),
    openUrlTime( QTime() ),
    openUrlCount( 0 )
{
    setApplicationName( BROWSEURL_APPLICATION );
    setOrganizationName( BROWSEURL_ORG_NAME );
    setOrganizationDomain( BROWSEURL_ORG_DOMAIN );
    readSettings();
    setQuitOnLastWindowClosed( false );
    createTrayMenu();
    connect( this, SIGNAL( aboutToQuit() ), this, SLOT( updateSettings() ) );
    connect( osProxy, SIGNAL( filesForCopyLinks( const QStringList & ) ),
             this, SLOT( copyLinksFromFiles( const QStringList & ) ) );
}

Application::~Application()
{
    delete preferencesDialog;
    delete aboutDialog;
    delete domainModel;
    delete osProxy;
}

OsProxy * Application::createOsProxy()
{
#if defined( Q_OS_MAC )
    return new MacProxy( this );
#elif defined( Q_OS_WIN )
    return new WinProxy( this ); // TODO
#elif defined( Q_OS_UNIX )
    return new UnixProxy( this ); // TODO
#else
#error Unsupported platform!
#endif
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
        aboutDialog = new AboutDialog();
    }
    aboutDialog->show();
}

void Application::showPreferencesDialog()
{
    if ( ! preferencesDialog ) {
        preferencesDialog = new PreferencesDialog( osProxy, domainModel );
        connect( preferencesDialog, SIGNAL( rejected() ), this, SLOT( updateSettings() ) );
    }
    preferencesDialog->show();
}

bool Application::event( QEvent *event )
{
    if ( event->type() == QEvent::FileOpen ) {
        QUrl url = static_cast<QFileOpenEvent *>( event )->url();
        if ( ! url.isEmpty() ) {
//            if ( url.scheme() == QString( "file" ) ) {
//                copyLink( url.path() );
//                return true;
//            } else
            if ( url.scheme() == QString( BROWSEURL_SCHEME ) ) {
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
    QList < Domain > domains = domainModel->getDomains();

    foreach ( const Domain &domain, domains ) {
        if ( testDomain == domain.getDomain() ) {
            *aDomain = domain.getDomain();
            *domainPath = domain.getLocalPath();
            return true;
        }
    }
    return false;
}

void Application::openPathInExplorer( const QString & path )
{
    osProxy->browsePath( path );
}

void Application::showError( const QString &message )
{
    QMessageBox::critical( 0, tr( "BrowseURL Error" ), message );
}

void Application::copyLinksFromFiles( const QStringList & files )
{
    QStringList copyLinks;
    foreach ( const QString & file, files ) {
        QString copyLink = createCopyLink( file );
        if ( copyLink.isEmpty() ) {
            showError( tr( "No matching BrowseURL domain for local path %1" ).
                       arg( file ) );
            return;
        }
        copyLinks.append( copyLink );
    }
    QApplication::clipboard()->setText( copyLinks.join( "\n" ) );
}

QString Application::createCopyLink( const QString & path ) const
{
    QList < Domain > domains = domainModel->getDomains();

    foreach ( const Domain & domain, domains ) {
        if ( path.startsWith( domain.getLocalPath() ) ) {
             QUrl url;
             url.setScheme( QString( BROWSEURL_SCHEME ) );
             url.setHost( domain.getDomain() );
             url.setPath( path.mid( domain.getLocalPath().length() ) );
             return QString( url.toEncoded() );
        }
    }
    return QString();
}

void Application::readSettings()
{
    QSettings settings;
    if ( ! settings.value( "domains/size" ).isValid() ) {
        setDefaultSettings();
        return;
    }
    QList < Domain > domains = domainModel->getDomains();
    int size = settings.beginReadArray("domains");
    for ( int i = 0; i < size; i++ ) {
        settings.setArrayIndex( i );
        QString domain = settings.value( "domain" ).toString();
        QString localPath = settings.value( "localPath" ).toString();
        domainModel->addDomain( domain, localPath );
    }
    settings.endArray();
}

void Application::setDefaultSettings()
{
    qDebug( "setDefaultSettings()" );
    domainModel->addDomain( "dropbox", QDir::homePath() + "/Dropbox" );
    domainModel->addDomain( "aerofs",  QDir::homePath() + "/AeroFS" );
    domainModel->addDomain( "home",    QDir::homePath() );
    osProxy->enableAutostart();
#if defined( Q_WS_MAC )
    // TODO
    QProcess::execute( QString( "/System/Library/CoreServices/pbs" ) );
#endif
}

void Application::writeSettings()
{
    QSettings settings;
    QList < Domain > domains = domainModel->getDomains();
    int i = 0;
    settings.beginWriteArray("domains");
    foreach ( const Domain &domain, domains ) {
        settings.setArrayIndex( i );
        settings.setValue( "domain", domain.getDomain() );
        settings.setValue( "localPath", domain.getLocalPath() );
        i++;
    }
    settings.endArray();
}

void Application::updateSettings()
{
    //qDebug("updateSettings");
    writeSettings();
}

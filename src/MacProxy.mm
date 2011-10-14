#include <Cocoa/Cocoa.h>
#include <QString>
#include <QStringList>

#include "MacProxy.h"
#include "MacProxyDelegate.h"

#define DELEGATE ( ( MacProxyDelegate * ) delegate )

MacProxy::MacProxy( QObject * parent ) :
    OsProxy( parent )
{
    pool     = ( void * ) [ [ NSAutoreleasePool alloc ] init ];
    delegate = ( void * ) [ [ MacProxyDelegate alloc ] initWithProxy:this ];
}

MacProxy::~MacProxy()
{
    [ DELEGATE release ];
    [ ( ( NSAutoreleasePool * ) pool ) release ];
}

bool MacProxy::isAutostartEnabled() const
{
    return ( bool ) [ DELEGATE detectLoginItem ];
}

void MacProxy::enableAutostart()
{
    [ DELEGATE addLoginItem ];
}

void MacProxy::removeAutostart()
{
    [ DELEGATE removeLoginItem ];
}

void MacProxy::browsePath( const QString & path )
{
    [ DELEGATE openInFinder:path ];
}

void MacProxy::copyLinksFromFiles( const QStringList & files )
{
    emit filesForCopyLinks( files );
}

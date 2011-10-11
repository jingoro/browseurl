#include <AppKit/AppKit.h>
#include <Cocoa/Cocoa.h>

#include "CocoaProxy.h"

// PIMPL method of protecting C++ code from Cocoa namespace collisions
class CocoaProxy::Private
{

public:

    Private()
    {
        NSApplicationLoad();
        pool = [ [ NSAutoreleasePool alloc ] init ];
    }

    ~Private()
    {
        [ pool release ];
    }

    bool detectLoginItem()
    {
        return detectOrRemoveLoginItem( true );
    }

    void removeLoginItem()
    {
        detectOrRemoveLoginItem( false );
    }

    void addLoginItem()
    {
        if ( detectLoginItem() ) {
            return;
        }

        LSSharedFileListRef loginItemsRef = LSSharedFileListCreate(
                    NULL, kLSSharedFileListSessionLoginItems, NULL );
        if ( ! loginItemsRef ) {
            return;
        }

        NSURL *appUrl = [ NSURL fileURLWithPath:appPath() ];
        LSSharedFileListItemRef item = LSSharedFileListInsertItemURL(
                    loginItemsRef, kLSSharedFileListItemLast, NULL, NULL,
                    ( CFURLRef ) appUrl, NULL, NULL);
        if ( item ) {
            CFRelease( item );
        }
        CFRelease( loginItemsRef );
    }

private:

    bool detectOrRemoveLoginItem( bool detect = true )
    {
        LSSharedFileListRef loginItemsRef = LSSharedFileListCreate(
                    NULL, kLSSharedFileListSessionLoginItems, NULL );
        if ( ! loginItemsRef ) {
            return false;
        }

        bool found = false;
        UInt32 seed = 0U;
        NSString *apath = appPath();

        CFArrayRef loginItems = LSSharedFileListCopySnapshot( loginItemsRef, &seed );
        for ( id item in ( NSArray * ) loginItems ) {
            CFURLRef url;
            OSStatus status = LSSharedFileListItemResolve(
                        (LSSharedFileListItemRef) item,
                        0, &url, NULL );
            NSString *urlPath = [ ( ( NSURL * ) url ) path ];
            if ( [ urlPath compare:apath ] == NSOrderedSame ) {
                if ( ! detect ) { // remove
                    LSSharedFileListItemRemove( loginItemsRef, (LSSharedFileListItemRef) item );
                }
                found = true;
                break;
            }
        }
        CFRelease( loginItems );
        CFRelease( loginItemsRef );
        return found;
    }

    NSString *appPath() const
    {
        return [ [ NSBundle mainBundle ] bundlePath ];
    }

    NSAutoreleasePool* pool;
};

//
// BEGIN CocoaProxy
//

CocoaProxy::CocoaProxy()
{
    p = new CocoaProxy::Private();
}

CocoaProxy::~CocoaProxy()
{
    delete p;
}

bool CocoaProxy::autostartEnabled() const
{
    return p->detectLoginItem();
}

void CocoaProxy::enableAutostart()
{
    p->addLoginItem();
}

void CocoaProxy::disableAutostart()
{
    p->removeLoginItem();
}

#include <AppKit/AppKit.h>
#include <QString>
#include <QStringList>

#include "MacProxyDelegate.h"

@implementation MacProxyDelegate

- ( id ) initWithProxy: ( MacProxy * ) aProxy
{
    self = [ super init ];
    if ( self ) {
        proxy = aProxy;
        [ NSApp setServicesProvider:self ];
    }
    return self;
}

- ( bool ) detectLoginItem
{
    return ( bool ) [ self detectOrRemoveLoginItem:YES ];
}

- ( void ) addLoginItem
{
    if ( [ self detectLoginItem ] ) {
        return;
    }

    LSSharedFileListRef loginItemsRef = LSSharedFileListCreate(
                NULL, kLSSharedFileListSessionLoginItems, NULL );
    if ( ! loginItemsRef ) {
        return;
    }

    NSURL *appUrl = [ NSURL fileURLWithPath: [ self appPath ] ];
    LSSharedFileListItemRef item = LSSharedFileListInsertItemURL(
                loginItemsRef, kLSSharedFileListItemLast, NULL, NULL,
                ( CFURLRef ) appUrl, NULL, NULL);
    if ( item ) {
        CFRelease( item );
    }
    CFRelease( loginItemsRef );
}

- ( void ) removeLoginItem
{
    [ self detectOrRemoveLoginItem:NO ];
}

- ( void ) openInFinder: ( const QString & ) path
{
    NSString *nsPath = [ NSString stringWithUTF8String: ( path.toUtf8().constData() ) ];
    [ [ NSWorkspace sharedWorkspace ] selectFile: nsPath inFileViewerRootedAtPath: @"" ];
    // TODO
    // QStringList args;
    // args << ( applicationDirPath() + "/../Resources/osx-open-in-finder.scpt" );
    // args << path;
    // QProcess::execute( QString( "/usr/bin/osascript" ), args );
}

- ( void ) copyLinksFromFiles: ( NSPasteboard * ) pboard
                     userData: ( NSString * ) data
                        error: ( NSString ** ) error
{
    NSArray * files = [ pboard propertyListForType:NSFilenamesPboardType ];
    NSUInteger count = [ files count ];
    QStringList outFiles;
    for ( NSUInteger i = 0; i < count; i++ ) {
        NSString *nsFile = [ files objectAtIndex: i ];
        QString qFile = QString::fromUtf8( [ nsFile UTF8String ] );
        outFiles.append( qFile );
    }
    proxy->copyLinksFromFiles( outFiles );
}

- ( BOOL ) detectOrRemoveLoginItem: ( BOOL ) detect
{
    LSSharedFileListRef loginItemsRef = LSSharedFileListCreate(
                NULL, kLSSharedFileListSessionLoginItems, NULL );
    if ( ! loginItemsRef ) {
        return NO;
    }

    BOOL found = NO;
    UInt32 seed = 0U;
    NSString *apath = [ self appPath ];

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
            found = YES;
            break;
        }
    }
    CFRelease( loginItems );
    CFRelease( loginItemsRef );
    return found;
}

- ( NSString * ) appPath
{
    return [ [ NSBundle mainBundle ] bundlePath ];
}

@end

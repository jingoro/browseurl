#ifndef MACPROXYDELEGATE_H
#define MACPROXYDELEGATE_H

#import <Cocoa/Cocoa.h>
#include <QString>

#include "MacProxy.h"

@interface MacProxyDelegate : NSObject <NSApplicationDelegate> {
    MacProxy * proxy;
}

- ( id ) initWithProxy: ( MacProxy * ) aProxy;

- ( bool ) detectLoginItem;
- ( void ) addLoginItem;
- ( void ) removeLoginItem;

- ( void ) openInFinder: ( const QString & ) path;

- ( void ) copyLinksFromFiles: ( NSPasteboard * ) pboard
                     userData: ( NSString * ) data
                        error: ( NSString ** ) error;

// Private

- ( BOOL ) detectOrRemoveLoginItem: ( BOOL ) detect;
- ( NSString * ) appPath;

@end

#endif // MACPROXYCOCOA_H

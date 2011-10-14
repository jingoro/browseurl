#include "CocoaServiceHandler.h"

#include <QStringList>

@implementation CocoaServiceHandler

- (id) initWithProxy: ( CocoaProxy * ) aProxy
{
    self = [ super init ];
    if ( self ) {
        proxy = aProxy;
        [ NSApp setServicesProvider:self ];
    }
    return self;
}

- (void) openFilesForCopyLink: ( NSPasteboard * ) pboard
  userData:( NSString * ) data
  error:( NSString ** ) error
{
    NSArray *inFiles = [ pboard propertyListForType:NSFilenamesPboardType ];
    NSUInteger count = [ inFiles count ];
    QStringList outFiles;
    for ( NSUInteger i = 0; i < count; i++ ) {
        NSString *nsFile = [ inFiles objectAtIndex: i ];
        const char *cFile = [ nsFile UTF8String ];
        QString qFile = QString::fromUtf8( cFile );
        outFiles.append( qFile );
    }
    proxy->setFilesForCopyLink( outFiles );
}

@end

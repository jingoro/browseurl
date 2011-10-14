#ifndef MACPROXY_H
#define MACPROXY_H

#include <QString>
#include <QStringList>

#include "OsProxy.h"

class MacProxy : public OsProxy
{
    Q_OBJECT

public:
    MacProxy( QObject * parent = 0 );
    ~MacProxy();

    bool isAutostartEnabled() const;
    void enableAutostart();
    void removeAutostart();

    void browsePath( const QString & path );

    void copyLinksFromFiles( const QStringList & files );

private:
    void * pool;
    void * delegate;
};

#endif // MACPROXY_H

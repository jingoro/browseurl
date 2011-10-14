#ifndef OSPROXY_H
#define OSPROXY_H

#include <QObject>
#include <QString>
#include <QStringList>

class OsProxy : public QObject
{
    Q_OBJECT

public:
    OsProxy( QObject * parent = 0 );

    virtual bool isAutostartEnabled() const = 0;
    virtual void enableAutostart() = 0;
    virtual void removeAutostart() = 0;

    virtual void browsePath( const QString & path ) = 0;

signals:
    void filesForCopyLinks( const QStringList & files );

};

#endif // OSPROXY_H

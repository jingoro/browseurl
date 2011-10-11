#ifndef DOMAIN_H
#define DOMAIN_H

#include <QObject>

class Domain
{
public:
    Domain( const QString &aDomain, const QString &aLocalPath );

    void setDomain( const QString &aDomain );
    void setLocalPath( const QString &aLocalPath );
    QString getDomain() const;
    QString getLocalPath() const;

private:
    QString domain;
    QString localPath;
};

#endif // DOMAIN_H

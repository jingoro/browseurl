#include "Domain.h"

Domain::Domain( const QString &aDomain, const QString &aLocalPath )
{
    setDomain( aDomain );
    setLocalPath( aLocalPath );
}

void Domain::setDomain( const QString &aDomain )
{
    domain = aDomain;
}

void Domain::setLocalPath( const QString &aLocalPath )
{
    localPath = aLocalPath;
}

QString Domain::getDomain() const
{
    return domain;
}

QString Domain::getLocalPath() const
{
    return localPath;
}

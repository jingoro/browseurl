#ifndef COCOAPROXY_H
#define COCOAPROXY_H

#include <QString>

#include "OsProxy.h"

class CocoaProxy : public OsProxy
{
public:
    CocoaProxy();
    ~CocoaProxy();

    bool autostartEnabled() const;
    void enableAutostart();
    void disableAutostart();

private:
    class Private;
    Private *p;
};

#endif // COCOAPROXY_H

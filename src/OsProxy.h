#ifndef OSPROXY_H
#define OSPROXY_H

class OsProxy
{
public:
    virtual bool autostartEnabled() const;
    virtual void enableAutostart();
    virtual void disableAutostart();
};

#endif // OSPROXY_H

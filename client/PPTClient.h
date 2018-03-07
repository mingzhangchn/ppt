#ifndef PPTClient_H
#define PPTClient_H

#include <binder/Parcel.h>
#include <binder/Binder.h>
#include <binder/IBinder.h>
#include <binder/IInterface.h>

namespace android
{

class PPTClient: public virtual RefBase{
public:
    static sp<PPTClient> self();
    int startHlsService();
    int stopHlsService();
        
private:
    PPTClient();
    virtual ~PPTClient();
    sp<IBinder> mPPTService;
    static sp<PPTClient> sPPTClient;
};

};//namespace android

#endif

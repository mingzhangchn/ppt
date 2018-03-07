#ifndef PPTService_H
#define PPTService_H

#include <utils/RefBase.h>
#include <utils/String8.h>
#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>

#include "PPTTsFilterMgr.h"
#include "PPTTsSliceMgr.h"

class PPTService : public BBinder{
public:
    static sp<PPTService> self();
    
    virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);
    
private:
    PPTService();
    virtual ~PPTService();
    sp<PPTTsFilterMgr> mTsFilterMgr;
    sp<PPTTsSliceMgr> mTsSliceMgr;
    static sp<PPTService> sPPTService;
};

#endif

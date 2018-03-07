
#ifndef PPTTsFilterMgr_H
#define PPTTsFilterMgr_H

#include "hi_type.h"
#include "hi_unf_demux.h"
#include <utils/RefBase.h>
#include <utils/Mutex.h>
#include <utils/StrongPointer.h>
#include "PPTChannelManager.h"

using namespace std;
using namespace android;

class PPTTsFilterMgr: public virtual RefBase{
public:
    static sp<PPTTsFilterMgr> self();
    
    int startFilter();
    int stopFilter();
    int getTsFromFilter(char *pBuf, unsigned int *bufLen);

private:
    PPTTsFilterMgr();
    virtual ~PPTTsFilterMgr();
    int initFilter();
    
    sp<PPTChannelManager> mChnMgr;
    
    HI_HANDLE mRecHandle;
    vector<HI_HANDLE> mChnHandles;
    int mDemuxId;
    
    int mStatus;
    
    static sp<PPTTsFilterMgr> sTsFilterMgr;
    static Mutex sTsFilterMutex;
};

#endif
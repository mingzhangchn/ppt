//ppt only user one tuner
#ifndef PPTTunerMananger_H
#define PPTTunerManager_H

#include <utils/RefBase.h>
#include <utils/Mutex.h>
#include <utils/StrongPointer.h>
#include "hi_type.h"
#include "CHDTVClient.h"

using namespace std;
using namespace android;

class PPTTunerManager: public virtual RefBase{
public:
    static sp<PPTTunerManager> self();
    int tune();
    int status();

private:
    PPTTunerManager();
    virtual ~PPTTunerManager();
    int mTunerId;
    int mFreq;
    int mBandWidth;
    int mSym;
    
    static sp<PPTTunerManager> sTunerMgr;
};

#endif
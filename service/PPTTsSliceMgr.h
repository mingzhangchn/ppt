
#ifndef PPTTsSliceMgr_H
#define PPTTsSliceMgr_H

#include <string>
#include "PPTChannelManager.h"
#include "PPTTsFilterMgr.h"
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>
#include <utils/Mutex.h>
#include <utils/Thread.h>
#include <map>

using namespace std;
using namespace android;

class PPTTsSliceMgr: public Thread{
public:
    static sp<PPTTsSliceMgr> self();
    int generatePlayList();
    
protected:
    virtual bool threadLoop(){
        PPTTsSliceMgr::self()->getTsData();
        return false;
    }    
    
private:
    PPTTsSliceMgr();
    virtual ~PPTTsSliceMgr();
    void getTsData();
    
    sp<PPTTsFilterMgr> mTsFilterMgr;
    sp<PPTChannelManager> mChannelMgr;
    
    static sp<PPTTsSliceMgr> sTsSliceMgr;
    static const int TS_PACKET_LEN;
    
    class SliceFileMgr: public virtual RefBase{
    public:
        SliceFileMgr(string path, int sid);
        ~SliceFileMgr();
        int init();
        void addTsPacket(char *buf, int len);
    private:
        string mPath;
        int mServiceId;
        long mStartSeq;
        long mCurSeq;
        char *mPTsBuf;
        int mTsBufLen;
        int mSliceCount;
        static const int SLICE_MAX_LEN;
    };
    
    map<int, sp<SliceFileMgr> > mSliceFileMgrMap;
    
};

#endif
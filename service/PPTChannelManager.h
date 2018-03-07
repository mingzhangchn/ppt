
#ifndef PPTChannelManager_H
#define PPTChannelManager_H

#include <vector>
#include <string>
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>
#include <utils/Mutex.h>

using namespace std;
using namespace android;

typedef struct ChannelInfo_s{
    int serviceId;    
    int vPid;
    int vType;
    int aPid;
    int aType;
}ChannelInfo;

class PPTTsFilterMgr;
class PPTTsSliceMgr;
class PPTChannelManager : public virtual RefBase{
public:
    static  sp<PPTChannelManager> self();
    
    int updateChannelInfo(string npifile);
    int getChannelCount();
    int getServiceId(int pid);
    int getChannelInfo(int sid, ChannelInfo &info);

private:
    friend class PPTTsFilterMgr;
    friend class PPTTsSliceMgr;
    PPTChannelManager();
    virtual ~PPTChannelManager();
    vector<ChannelInfo> mChannelList;

    static sp<PPTChannelManager>  sChannelMgr;
    static Mutex sChannelMutex;
};

#endif

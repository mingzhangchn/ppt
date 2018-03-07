#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <utils/RefBase.h>
#include <binder/ProcessState.h>   
#include <binder/IPCThreadState.h>   
#include "ppt_web_service.h"
#include "PPTChannelManager.h"
#include "PPTTsFilterMgr.h"
#include "PPTTsSliceMgr.h"
#include "PPTTunerManager.h"
#include "PPTService.h"
#include "PPTNctFilter.h"
#include "PPTNctParser.h"
#include "PPTNctSectionManager.h"
#include "PPTDDTFilter.h"
#include "PPTDDTParser.h"
#include "PPTDDTSectionManager.h"

using namespace std;


int main(int argc, char** argv)
{
#if 1
//test
    cout<<"In main88888"<<endl;
    
    upnp_and_web_server_start();
    
    //test_tuner_DTMB();
    
    //ppt_ts_start();
    
    PPTTunerManager::self()->tune();
    
    if (0){
        PPTNctSectionManager nctSecMgr;
        PPTNctFilterStart(&nctSecMgr);
        while(1){
            if (PPTNctSectionRecvFalg()){
                break;
            }
            sleep(1);
        }
        PPTNctFilterStop();
        PPTNctParser nctParser;
        nctParser.parse(&nctSecMgr);
        
        PPTDDTSectionManager DttSecMgr;
        DttSecMgr.setSectionCount(nctParser.getFileBlockCount());
        DttSecMgr.setFileLen(nctParser.getFileLen());
        PPTDDTFilterStart(nctParser.getDdtPid(), nctParser.getDdtTableId(), &DttSecMgr);
        while(1){
            if (PPTDDTSectionRecvFalg()){
                break;
            }
            sleep(1);
        }    
        PPTDDTFilterStop();
        PPTDDTParser ddtParser;
        ddtParser.parse(&DttSecMgr);    
    }
    
    PPTChannelManager::self()->updateChannelInfo("./npi.xml");
    
    PPTTsFilterMgr::self()->startFilter();
    
    PPTTsSliceMgr::self()->generatePlayList();
    
    PPTTsSliceMgr::self()->run();
    
    sleep(1000);    
    PPTTsFilterMgr::self()->stopFilter();
    
    while(1){
        sleep(100);
    }
#else

	sp<ProcessState> proc(ProcessState::self());
    
	PPTService::self();
    
	ProcessState::self()->startThreadPool();
    
	IPCThreadState::self()->joinThreadPool();
    
#endif    
    
    return 0;
}
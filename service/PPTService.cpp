#include <binder/Parcel.h>
#include <binder/IServiceManager.h>
//#include <android/log.h>
#include <utils/Log.h>
#include "PPTService.h"
#include "PPTTsFilterMgr.h"
#include "PPTTsSliceMgr.h"
#include "PPTTunerManager.h"
#include "ppt_web_service.h"

sp<PPTService> PPTService::sPPTService = NULL;

sp<PPTService> PPTService::self()
{
    if (sPPTService != NULL){
        return sPPTService;
    }
    sPPTService = new PPTService();
    
	while(1){
		if( defaultServiceManager() != NULL){
			break;
		}
		LOGE("server manager is not actvite!!!!!!!!!!!!!!!");
		sleep(1);
	};
		
	defaultServiceManager()->addService(String16("NGB.PPT"), sPPTService);   
    
    return sPPTService;
}

PPTService::PPTService()
:mTsFilterMgr(PPTTsFilterMgr::self())
,mTsSliceMgr(PPTTsSliceMgr::self())
{
    //
}

PPTService::~PPTService()
{
    //
}

typedef enum {
    START_HLS_SERVICE,
    STOP_HLS_SERVICE
}PPT_CONTROL_CMD;

status_t PPTService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code){
    case START_HLS_SERVICE:{
        
        LOGI("START_HLS_SERVICE\n");
        upnp_and_web_server_start();
        
        PPTTunerManager::self()->tune();
        
        PPTChannelManager::self()->updateChannelInfo("/mnt/npi.xml");
        
        PPTTsFilterMgr::self()->startFilter();
        
        PPTTsSliceMgr::self()->generatePlayList();
        
        PPTTsSliceMgr::self()->run();        
        break;
    }
    case STOP_HLS_SERVICE:{
       LOGI("STOP_HLS_SERVICE\n");
       
       break; 
    }
        default:break;
    }

    return NO_ERROR;
}

#define LOG_NDEBUG 0
#define LOG_TAG "PPTClient"

#include <stdint.h>
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/time.h>  
#include <binder/Parcel.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <utils/String8.h>
#include "PPTClient.h"
#include "jah_util.h"
    

namespace android
{

typedef enum {
    START_HLS_SERVICE,
    STOP_HLS_SERVICE
}PPT_CONTROL_CMD;

sp<PPTClient> PPTClient::sPPTClient = NULL;

sp<PPTClient> PPTClient::self()
{
    if (sPPTClient!= NULL){
        return sPPTClient;
    }
    sPPTClient = new PPTClient();
    return sPPTClient;
}

int PPTClient::startHlsService()
{
	Parcel data, reply;
	data.writeInt32(0);
	data.writeInt32(714000);

	mPPTService->transact(START_HLS_SERVICE, data, &reply);
    
	return 0;    
}

int PPTClient::stopHlsService()
{
	Parcel data, reply;
	data.writeInt32(0);
	data.writeInt32(0);

	mPPTService->transact(STOP_HLS_SERVICE, data, &reply);
    
	return 0;       
}

PPTClient::PPTClient()
:mPPTService(NULL)
{
	//sp<ProcessState> proc(ProcessState::self());
	//ProcessState::self()->startThreadPool();
    
	while(mPPTService == NULL){
		mPPTService = defaultServiceManager()->getService(String16("NGB.PPT"));
		if(mPPTService != NULL){
			break;
		}
        
		sleep(1);
	};    
}

PPTClient::~PPTClient()
{
    //
}   
 
}
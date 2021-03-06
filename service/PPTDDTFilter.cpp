
#include "PPTDDTFilter.h"
#include "hi_type.h"
#include "CHDTVClient.h"
#include "PPTDDTParser.h"
#include "PPTDDTSectionManager.h"

using namespace android;

static PPTDDTSectionManager *pSectionmanager = NULL;

static DTV_PSI_FILTER_HANDLE handle;
static CHDTVClient* pDTVClient = NULL;
static int sectionRecvFlag = 0;

#define DTVT_TUNER_ID 0

static void filterCallback(int Device, DTV_PSI_FILTER_HANDLE Handle, SectionContainer *pstru_Section)
{
	if(pstru_Section == NULL)
	{
		LOGE("test_FilterCallback:data is null");
		return;
	}
        
    if (pSectionmanager){
        int ret = 0;
        ret = pSectionmanager->addSection((char*)pstru_Section->puc_SectionData, pstru_Section->i_SectionLength);
        if (ret == 1){
            sectionRecvFlag = 1;
        }
    }
}

int PPTDDTFilterStart(int pid, int tableId, PPTDDTSectionManager *pSecMgr)
{
	pDTVClient = CHDTVClient::getInstance();
	PSIFilterParams param;
	memset(&param.data[0], 0, sizeof(param.data));
	memset(&param.dataMask[0], 0, sizeof(param.dataMask));
	param.i_dataLen = 1;

	param.dataMask[0] = 0xff;
	param.i_Pid = pid;
	param.data[0] = tableId;
	param.fn_FilterCallback = filterCallback;

    sectionRecvFlag = 0;
    pSectionmanager = pSecMgr;
    
	LOGI("test_Filter: start..");
	int i_Result = pDTVClient->DTV_PSI_requestFilter(DTVT_TUNER_ID, &handle);
	if(i_Result){
		LOGE("test_Filter: Request FAILED!!!");
		return -1;
	}

	i_Result = pDTVClient->DTV_PSI_StartFilter(DTVT_TUNER_ID, handle, &param);
	if(i_Result){
		LOGE("test_Filter: Start FAILED!!!");
		return -1;
	}

	return 0;
}

int PPTDDTSectionRecvFalg()
{
    return sectionRecvFlag;
}

int PPTDDTFilterStop(void)
{
    if (pDTVClient){
        pDTVClient->DTV_PSI_StopFilter(DTVT_TUNER_ID, handle);
	    pDTVClient->DTV_PSI_releaseFilter(DTVT_TUNER_ID, handle);
    }   
    return 0;
}
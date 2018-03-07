

#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <binder/Parcel.h>
#include <android/log.h>
#include <utils/Log.h>
#include "CHDTVClient.h"

using namespace android;

#define DTVT_TEST_FREQ 		714000
#define DTVT_TUNER_ID 		(0)

int test_tuner_DTMB(void)
{
    
    CHDTVClient* pDTVClient = CHDTVClient::getInstance();
	DTVTCarrierParams paramT;
	paramT.enm_ModeType = DTV_T;
	paramT.ui_bandWidth = 8000;
	paramT.enm_Spectrum = DTV_TUNER_SPECTRUM_AUTO;
	paramT.ui_FrequencyKhz = DTVT_TEST_FREQ;
	int i_result = pDTVClient->DTV_TUNER_SetCarrier(DTVT_TUNER_ID, &paramT);
	if(i_result == 0)
    {
		LOGI("test_tuner: DTV_TUNER_SetCarrier OK!");
	}
	else
	{
		LOGI("test_tuner: DTV_TUNER_SetCarrier Failed!!");
		return 1;
	}

	int i = 0;
	while( i++ < 3)
	{
		CarrierStatus status;
		sleep(1);
		pDTVClient->DTV_TUNER_GetCarrierStatus(DTVT_TUNER_ID, &status);
		if(status.enm_LockStatus == DTV_TUNER_CARRIER_LOCK)
		{
			LOGI("test_tuner: is Lock!!");
			LOGI("test_tuner: spectrum:%d biterror:%d, level:%d, quality:%d snr:%d",
			status.enm_Spectrum, status.i_BitErrorRate, status.i_SignLevel, status.i_SignQuality, status.i_SNR);
			break;
		}
		else
		{
			LOGI("test_tuner: is unLock[%d]!!", i);
		}
	}

	if(i >= 40)
	{
		LOGE("test_tuner: tuning freq is Failed!!!!");
		return 1;
	}

	DTVCarrierParamsBase * tmpParam;
	i_result = pDTVClient->DTV_TUNER_GetCurCarrier(DTVT_TUNER_ID, &tmpParam);
	DTVTCarrierParams* pdvbt_CarrierParams = (DTVTCarrierParams*)tmpParam;
	if(i_result == 0)
	{
		LOGI("test_tuner: DTV_TUNER_GetCurCarrier OK!");
		LOGI("test_tuner: DTV_TUNER_GetCurCarrier--->Freq[%d].", pdvbt_CarrierParams->ui_FrequencyKhz);
	}else
	{
		LOGE("test_tuner: DTV_TUNER_GetCurCarrier Failed!!");
		return 1;
	}

	LOGI("test_tuner: end!");

	return 0;
}


#ifndef CH_DTV_INTER_DATA_H
#define CH_DTV_INTER_DATA_H

#include <stdint.h>
#include <sys/types.h>
#include <utils/String8.h>
#include <binder/Parcel.h>
#include <binder/Binder.h>
#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include "chdtvdata.h"

namespace android
{

#define DTV_SCAN_CALL_BACK  "dtv scan call back"
#define DTV_PSI_SECTION_CALL_BACK  "dtv psi section call back"
#define DTV_PSI_TABLE_CALL_BACK  "dtv psi table call back"
#define DTV_PSI_FILTER_CALL_BACK  "dtv psi filter call back"
#define DTV_EPG_WATCH_CALL_BACK  "dtv epg watch call back"
class IDTVScanCallBack: public IInterface
{
public:
	DECLARE_META_INTERFACE(DTVScanCallBack);
};

class BnDTVScanCallBack: public BnInterface<IDTVScanCallBack>
{
public:
	status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);
};

class BpDTVScanCallBack: public BpInterface<IDTVScanCallBack>
{
public:
	BpDTVScanCallBack(const sp<IBinder>& impl) :
			BpInterface<IDTVScanCallBack>(impl)
	{
	}
};

class IDTVEpgWatchCallBack: public IInterface
{
public:
	DECLARE_META_INTERFACE(DTVEpgWatchCallBack);
};

class BnDTVEpgWatchCallBack: public BnInterface<IDTVEpgWatchCallBack>
{
public:
	EpgWatchCallBack CallBack;
	int demux;

	status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);
	BnDTVEpgWatchCallBack();
	~BnDTVEpgWatchCallBack();
};

class BpDTVEpgWatchCallBack: public BpInterface<IDTVEpgWatchCallBack>
{
public:
	BpDTVEpgWatchCallBack(const sp<IBinder>& impl) :
			BpInterface<IDTVEpgWatchCallBack>(impl)
	{
	}
};

class IDTVSectionCallBack: public IInterface
{
public:
	DECLARE_META_INTERFACE(DTVSectionCallBack);
};

typedef void (*fun_SectionCallback)(int Device, DTV_PSI_CAPTURE_HANDLE Handle, SectionContainer *pstru_Section);

class BnDTVSectionCallBack: public BnInterface<IDTVSectionCallBack>
{
public:
	DTV_PSI_CAPTURE_HANDLE handle;
	fun_SectionCallback fn_SectionCallback;
//	void (*fn_SectionCallback)(int Device, DTV_PSI_CAPTURE_HANDLE Handle, SectionContainer *pstru_Section);
	status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);
	BnDTVSectionCallBack();
	~BnDTVSectionCallBack();
};

class BpDTVSectionCallBack: public BpInterface<IDTVSectionCallBack>
{
public:
	BpDTVSectionCallBack(const sp<IBinder>& impl) :
			BpInterface<IDTVSectionCallBack>(impl)
	{
	}

};

class IDTVTableUpateCallBack: public IInterface
{
public:
	DECLARE_META_INTERFACE(DTVTableUpateCallBack);
};

typedef 	void (*fun_UpdateCallback)(int Device, DTV_PSI_CAPTURE_HANDLE Handle, TableContainer *pstru_Table);

class BnDTVTableUpateCallBack: public BnInterface<IDTVTableUpateCallBack>
{
public:
	DTV_PSI_CAPTURE_HANDLE handle;
	fun_UpdateCallback fn_UpdateCallback;
//	void (*fn_UpdateCallback)(int Device, DTV_PSI_CAPTURE_HANDLE Handle, TableContainer *pstru_Table);
	status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);

	BnDTVTableUpateCallBack();
	~BnDTVTableUpateCallBack();
};

class BpDTVTableUpateCallBack: public BpInterface<IDTVTableUpateCallBack>
{
public:
	BpDTVTableUpateCallBack(const sp<IBinder>& impl) :
			BpInterface<IDTVTableUpateCallBack>(impl)
	{
	}
};

class IDTVFilterCallBack: public IInterface
{
public:
	DECLARE_META_INTERFACE(DTVFilterCallBack);
};

typedef 	void (*fun_FilterCallback)(int Device, DTV_PSI_FILTER_HANDLE Handle, SectionContainer *pstru_Section);

class BnDTVFilterCallBack: public BnInterface<IDTVFilterCallBack>
{
public:
	DTV_PSI_CAPTURE_HANDLE handle;
	PSIFilterParams params;
	fun_FilterCallback fn_FilterCallback;
//	void (*fn_FilterCallback)(int Device, DTV_PSI_FILTER_HANDLE Handle, SectionContainer *pstru_Section);
	status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);
	BnDTVFilterCallBack();
	~BnDTVFilterCallBack();
};

class BpDTVFilterCallBack: public BpInterface<IDTVFilterCallBack>
{
public:
	BpDTVFilterCallBack(const sp<IBinder>& impl) :
			BpInterface<IDTVFilterCallBack>(impl)
	{
	}
};

/*数据监控参数*/
class InterCaptureParams
{
public:
	int i_Pid;
	int i_TableId;
	int i_TableIdEx;
	int i_TableIdEx2;
	int i_TableIdEx3;

	int i_dataLen; /*过滤比较数据长度:0~16*/
	unsigned char data[PSI_FILTER_DEPTH]; /*比较数据*/
	unsigned char mask[PSI_FILTER_DEPTH]; /*需要比较的位*/
	unsigned char patern[PSI_FILTER_DEPTH]; /*不等模式使用的位*/

	bool isUsed;
	
	int i_IsSection;
	sp<BnDTVTableUpateCallBack> ib_UpdateCallback;
	sp<BnDTVSectionCallBack> ib_SectionCallback;
	void writeToParcel(Parcel* outData);
	InterCaptureParams();
	~InterCaptureParams();
};

/*数据监控参数*/
class InterCaptureParamsS
{
public:
	int i_Pid;
	int i_TableId;
	int i_TableIdEx;
	int i_TableIdEx2;
	int i_TableIdEx3;
	bool isUsed;
	int i_dataLen; /*过滤比较数据长度:0~16*/
	unsigned char data[PSI_FILTER_DEPTH]; /*比较数据*/
	unsigned char mask[PSI_FILTER_DEPTH]; /*需要比较的位*/
	unsigned char patern[PSI_FILTER_DEPTH]; /*不等模式使用的位*/
	
	int i_IsSection;
	sp<IBinder> ibTableClient;
	sp<IBinder> ibSectionClient;
	void readFromParcel(const Parcel* inData);
	~InterCaptureParamsS();
};

/*数据监控参数*/
class InterFilterParams
{
public:
	int i_Pid;
	int i_dataLen; /*过滤比较数据长度:0~16*/
	unsigned char data[PSI_FILTER_DEPTH]; /*比较数据*/
	unsigned char mask[PSI_FILTER_DEPTH]; /*需要比较的位*/
	unsigned char patern[PSI_FILTER_DEPTH]; /*不等模式使用的位*/
	sp<BnDTVFilterCallBack> ib_FilterCallback;
	void writeToParcel(Parcel* outData);
	InterFilterParams();
	~InterFilterParams();
};

/*数据监控参数*/
class InterFilterParamsS
{
public:
	int i_Pid;
	int i_dataLen; /*过滤比较数据长度:0~16*/
	unsigned char data[PSI_FILTER_DEPTH]; /*比较数据*/
	unsigned char mask[PSI_FILTER_DEPTH]; /*需要比较的位*/
	unsigned char patern[PSI_FILTER_DEPTH]; /*不等模式使用的位*/
	sp<IBinder> ibFilterClient;
	void readFromParcel(const Parcel* inData);
};

/*搜索参数*/
class InterScanParam
{
public:
	sp<BnDTVScanCallBack> ibCallbackClient;
	int us_MaxMainCarrNum; /*输入 主频点个数*/
	DVBCCarrierParams *pstru_MainCarrList; /*输入 主频点列表*/
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	InterScanParam();
	~InterScanParam();
};

/*搜索参数*/
class InterDTVScanParam
{
public:
	sp<BnDTVScanCallBack> ibCallbackClient;
	int us_MaxMainCarrNum; /*输入 主频点个数*/
	DTVCarrierParamsBase *pstru_MainCarrList; /*输入 主频点列表*/
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	InterDTVScanParam();
	~InterDTVScanParam();
};


/*搜索参数*/
class InterDTVScanParamS
{
public:
	sp<IBinder> ibCallbackClient;
	int us_MaxMainCarrNum; /*输入 主频点个数*/
	DTVCarrierParamsBase *pstru_MainCarrList; /*输入 主频点列表*/
	void readFromParcel(const Parcel* inData);
	~InterDTVScanParamS();
};


enum
{
	SET_CARRIER_LIST = IBinder::FIRST_CALL_TRANSACTION,
	GET_ALL_CARRIER ,
	GET_ALL_CHANNEL_COUNT,
	GET_ALL_CHANNEL,
	GET_ALL_CHANNEL_BY_TYPE_COUNT,
	GET_CHANNEL_BY_TYPE,
	GET_CHANNEL_INFO_BY_3ID,
	GET_CHANNEL_INFO,
//    GET_CHANNEL_BY_NAME,
	SWAP_CHANNEL,
	UPDATE_CHANNEL_ATTRIB,
	DELETE_CHANNEL,
	INSERT_CHANNEL, 	//ADD
	INSERT_CARRIER,		//ADD
	CLEAR_DB,
	GET_DVBC_CARRIER,
	GET_DB_NUMB,
	SET_NETWORK_ID,	
	GET_NETWORK_LIST,
	GET_NETWORK_ID,
	SET_BOUQUET_LIST,
	GET_BOUQUET_LIST,
	SAVE_BEFORE_CURRENT_CHANNEL,
	GET_BEFORE_CURRENT_CHANNEL,
	SAVE_LAST_CHANNEL,
	GET_LAST_CHANNEL,
	GET_TUNERIDOFLASTCHANNEL,
	GET_LAST_CHANNEL1,
	UPDATE_ALL_CHANNEL_INFO,
	SAVE_DB_TO_FLASH,
	REVERT_DB_FRAM_FALSH,
	GET_DB_NVOD_LIST,
	GET_DB_NVOD_NUM,

	EPG_GET_PF_INFO = GET_ALL_CARRIER + 0x100,
	EPG_GET_PF_EXT_TEXT,
	EPG_GET_SCH_INFO,
	EPG_GET_SCH_EXT_TEXT,
	EPG_GET_P_INFO_BY_NAME,
	EPG_GET_P_INFO_BY_TYPE,
	EPG_GET_F_INFO_BY_NAME,
	EPG_GET_F_INFO_BY_TYPE,
	EPG_GET_SCH_INFO_BY_NAME,
	EPG_GET_SCH_INFO_BY_TYPE,
	EPG_GET_TYPE_BY_ID,
	EPG_START_WATCH,
	EPG_STOP_WATCH,
	EPG_START,
	EPG_STOP,
	EPG_SET_CALLBACK,
	EPG_CLEAR_CALLBACK,

	PSI_GET_TABLE = EPG_GET_PF_INFO + 0x100,
	PSI_GET_PMT_TABLE,
	PSI_START_CAPTURE,
	PSI_STOP_CAPTURE,
	PSI_RESET_CACHE,
	PSI_PARSE_PMT,
	PSI_GET_TDT_TIME,
	PSI_GET_FREE_FILTER_NUMBER,
	PSI_ALLOC_FILTER,
	PSI_FREE_FILTER,
	PSI_START_FILTER,
	PSI_STOP_FILTER,

	TUNER_GET_NUMBER = PSI_GET_TABLE + 0x100,
	TUNER_GET_SYSTEM_TYPE,
	TUNER_SET_DVBC_IF,
	TUNER_GET_CUR_DVBC_IF,
	TUNER_GET_STATUS,
	TUNER_SET_CARRIER,
	TUNER_GET_CUR_CARRIER,

	NVOD_START = TUNER_GET_NUMBER + 0x100,
	NVOD_GET_REF_SERVICE_LIST,
	NVOD_GET_REF_EVENT_LIST,
	NVOD_GET_TIMESHIFT_EVENT_LIST,
	NVOD_STOP,
	
	SCAN_ENTER_NIT = NVOD_START + 0x100,
	SCAN_ENTER_MANUAL,
	SCAN_ENTER_BAND,
	SCAN_UPDATE_RESULT_TO_DB,
	SCAN_EXIT,

	MEDIA_SET_DATA_SOURCE =  SCAN_EXIT+100,
	MEDIA_START,
	MEDIA_STOP,
	MEDIA_SET_STOP_MODE,
	MEDIA_GET_STOP_MODE,
	MEDIA_SELECT_STREAM,
	MEDIA_SET_VIDEO_DISPLAY,
	MEDIA_GET_VIDEO_DISPLAY,
	MEDIA_SET_VIDEO_AREA,
	MEDIA_GET_VIDEO_AREA,
	MEDIA_SET_CLIP,
	MEDIA_GET_CLIP,
	MEDIA_SET_PAUSE_MODE,
	MEDIA_GET_PAUSE_MODE,	
	MEDIA_PAUSE,
	MEDIA_RESUME,
	MEDIA_BIND_DEMUX,
	MEDIA_UNBIND_DEMUX,
	MEDIA_SET_WINDOW,
	MEDIA_SET_VOLUME,
	MEDIA_GET_WINDOSHANDLE,
	MEDIA_GET_OutputMatchMethod,
	MEDIA_SET_OutputMatchMethodE,
	
};

};//end namespace

#endif

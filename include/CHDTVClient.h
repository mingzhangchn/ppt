#ifndef CH_DTV_CLIENT_H
#define  CH_DTV_CLIENT_H

#include <stdint.h>
#include <sys/types.h>
#include <utils/String8.h>
#include <binder/Parcel.h>

#include "chdtvdata.h"
#include "chdtvInterdata.h"
//#include "../include/CHDTVClientInter.h"

namespace android
{

#define  DTV_MAX_SECTION_CAPTURE  64

class CHDTVClient
{
protected:
	InterDTVScanParam* m_pInterDTVScanParam;			//added by chendelin	20131221
	InterCaptureParams* m_pCaptureCallbacks[DTV_MAX_SECTION_CAPTURE];
	InterFilterParams* m_pFilterCallbacks[DTV_MAX_SECTION_CAPTURE];

	sp<IBinder> m_pRemoteService;

public:

	bool serverIsDie;

	static CHDTVClient* getInstance();

	CHDTVClient();
	~CHDTVClient();

	long get_current_time();

    /* 这里通过内部类DeathNotifier来继承IBinder::DeathRecipient，实现binderDied */  
    class DeathNotifier: public IBinder::DeathRecipient  
    {  
 	 public:  
       	DeathNotifier() {}  
        	virtual ~DeathNotifier();  
        	virtual void binderDied(const wp<IBinder>& who);  
    };  

    /* 定义1个DeathNotifier类型的变量  */   
    	sp<DeathNotifier>                  sDeathNotifier;  
	

	/*set/store carrierlist*/
	DTV_RESULT_e DTV_DB_SetCarrierList(int ri_databaseID, DVBCCarrierInfo *rpstru_CarrierDataList, int rui_CarrierNum);
	
	DTV_RESULT_e DTV_DB_GetAllCarrierList(int ri_databaseID, DVBCCarrierInfo **rppstru_CarrierDataList, int *rpui_CarrierNum);

	DTV_RESULT_e DTV_DB_GetAllChannelCount(int ri_databaseID, int* rpi_ChannelCount);

	DTV_RESULT_e DTV_DB_GetAllChannelData(int ri_databaseID, DTVChannelInfo **rppstru_ChannelDataList, int *rpi_ChannelNumb);

	DTV_RESULT_e DTV_DB_GetAllNvodList(int ri_databaseID, DTVNvodList **rppstru_NvodList, int *rpui_NvodNumb);

	DTV_RESULT_e DTV_DB_GetAllNvodCount(int ri_databaseID, int *rpi_NvodNumb);

	DTV_RESULT_e DTV_DB_GetChannelCountByType(int ri_databaseID, int ri_ChannelType, int* rpi_ChannelCount);

	DTV_RESULT_e DTV_DB_GetAllChannelListByType(int ri_databaseID, int ri_ChannelType, DTVChannelInfo **rpstru_ChannelDataList, int* rpi_ChannelNumb);

	DTV_RESULT_e DTV_DB_GetChannelDataBy3ID(int ri_databaseID, int ri_TsID, int ri_OrgNetID, int ri_ServiceID, DTVChannelInfo *rpstru_ChannelData);

	DTV_RESULT_e DTV_DB_GetChannelData(int ri_databaseID, int ri_ChannelID, DTVChannelInfo *rpstru_ChannelData);

//    DTV_RESULT_e  DTV_DB_GetChannelDataByName(int  ri_databaseID, char *rpc_namestring, DTVChannelInfo *rpstru_ChannelData);

	DTV_RESULT_e DTV_DB_SwapChannel(int ri_databaseID, int ri_SrcChannelIndex, int ri_DstChannelIndex);

	/*更新节目信息*/
	DTV_RESULT_e DTV_DB_UpdateChannelData(int ri_databaseID, DTVChannelInfo *rpstru_ChannelData);

	DTV_RESULT_e DTV_DB_InsertChannel(int ri_databaseID,  int ri_StartIndex, DTVChannelInfo* rpstru_ChannelDataList, int ri_Number) ;

	DTV_RESULT_e DTV_DB_InsertCarrier(int ri_databaseID, int ri_StartIndex, DVBCCarrierInfo* rpstru_CarrierInfoList, int ri_Number) ;

	/*删除某个节目*/
	DTV_RESULT_e DTV_DB_DeleteChannel(int ri_databaseID, int ri_ChannelIndex,int deletetype);

	/*清空节目数据库*/
	DTV_RESULT_e DTV_DB_ClearDB(int ri_databaseID);

	/*获取频点信息*/
	DTV_RESULT_e DTV_DB_GetDVBCCarrier(int ri_databseID, int ri_CarrierIndex, DVBCCarrierInfo *rpstru_CarrierInfo);

	/*获取数据库个数*/
	DTV_RESULT_e DTV_DB_GetDBNumb(int * rpi_DBNumb);
	
	/*保存网络列表*/
	DTV_RESULT_e DTV_DB_SetNetworkList(int ri_databaseID, int ri_Numb, NetworkInfo *rpstru_NetworkList);
	
	/*获取网络列表*/
	DTV_RESULT_e DTV_DB_GetNetworkList(int ri_databaseID, int* rpi_Numb, NetworkInfo ** rppstru_NetworkList);

	/*获取网络列表*/
	DTV_RESULT_e DTV_DB_GetNetworkID(int ri_databaseID, int* rpi_NetworkID);
	
	/*保存Bouquet列表*/
	DTV_RESULT_e DTV_DB_SetBouquetList(int ri_databaseID, int ri_BouquetNumb, BouquetInfo * rpstru_BouqetInfoList);

	/*获取Bouquet列表*/
	DTV_RESULT_e DTV_DB_GetBouquetList(int ri_databaseID, int* rpi_BouquetNumb, BouquetInfo ** rppstru_BouqetInfoList);

	DTV_RESULT_e DTV_DB_SaveBeforeCurrentChannel(int ri_databaseID,int type, int ri_ChannelID);
	DTV_RESULT_e DTV_DB_GetBeforeCurrentChannel(int ri_databaseID,int type, int* rpi_ChannelID);

	/*保存最后一个节目*/
	DTV_RESULT_e DTV_DB_SaveLastChannel(int ri_databaseID,int type, int ri_ChannelID);

	DTV_RESULT_e DTV_DB_GetLastChannel(int ri_databaseID,int type, int* rpi_ChannelID);
	DTV_RESULT_e DTV_DB_GetTunerIDOfLastChannel(int ri_databaseID,int* rpi_TunerID);

	DTV_RESULT_e DTV_DB_GetLastChannel(int ri_databaseID,int* rpi_ChannelID);

	/*更新所有节目信息*/
	DTV_RESULT_e DTV_DB_UpdateAllChannelData(int ri_databaseID, DTVChannelInfo *rpstru_ChannelData, int ri_Number);

	/*将数据保存到中*/
	DTV_RESULT_e DTV_DB_SaveDB(int ri_databaseID);

	/*从FLASH中读出数据*/
	DTV_RESULT_e DTV_DB_RevertDB(int ri_databaseID);

	/*启动*/
	DTV_RESULT_e DTV_EPG_Start(int ri_Device, EPGSearchInfo *rpstru_SearchInfo);

	/*停止*/
	DTV_RESULT_e DTV_EPG_Stop(int ri_Device);
		
	/*获取当前随后节目信息*/
	DTV_RESULT_e DTV_EPG_GetPfInfo(int ri_DeMux, int ri_TsID, int ri_OrgNetId, int ri_ServiceID, unsigned int rui_Language, EPGPfEventInfo *rpstru_Pf);

	/*获取当前随后扩展信息*/
	DTV_RESULT_e DTV_EPG_GetPfExtendText(int ri_DeMux, int ri_Timeout, int ri_TsID, int ri_OrgNetId, int ri_ServiceID, int ri_EventID, int ri_Language, String8 *rpstr_Text);

	/*获取周报信息*/
	DTV_RESULT_e DTV_EPG_GetSchInfo(int ri_DeMux, int ri_TsID, int ri_OrgNetId, int ri_ServiceID, int rui_Language, EPGSCHEventInfo *rpstru_Sch);

	/*获取当前随后扩展信息*/
	DTV_RESULT_e DTV_EPG_GetSchExtendText(int ri_DeMux, int ri_Timeout, int ri_TsID, int ri_OrgNetId, int ri_ServiceID, int ri_EventID, int ri_Language, String8 *rpuc_Text);

	/*获取根据名字获取当前节目信息*/
	DTV_RESULT_e DTV_EPG_GetPresentInfoByName(int ri_DeMux, unsigned int rui_Language, String8* name, int * rpi_Numb, EPGSingleEventInfo **rppstru_EventList);

	/*获取根据名字获取随后节目信息*/
	DTV_RESULT_e DTV_EPG_GetFollowInfoByName(int ri_DeMux, unsigned int rui_Language, String8* rpc_name, int * rpi_Numb, EPGSingleEventInfo **rppstru_EventList);

	/*获取根据名字获取周报节目信息*/
	DTV_RESULT_e DTV_EPG_GetSchByName(int ri_DeMux, unsigned int rui_Language, String8* rpc_name, int * rpi_Numb, EPGSingleEventInfo **rppstru_EventList);

	/*获取根据分类(niddle)获取当前节目信息*/
	DTV_RESULT_e DTV_EPG_GetPresentInfoByType(int ri_DeMux, unsigned int rui_Language, int ri_Type, int * rpi_Numb, EPGSingleEventInfo **rppstru_EventList);

	/*获取根据分类(niddle)获取随后节目信息*/
	DTV_RESULT_e DTV_EPG_GetFollowInfoByType(int ri_DeMux, unsigned int rui_Language, int ri_Type, int * rpi_Numb, EPGSingleEventInfo** rppstru_EventList);

	/*获取根据分类(niddle)获取周报节目信息*/
	DTV_RESULT_e DTV_EPG_GetSchByType(int ri_DeMux, unsigned int rui_Language, int ri_Type, int * rpi_Numb, EPGSingleEventInfo **rppstru_EventList);

	/*获取根据eventid获取事件类型*///xyk
	DTV_RESULT_e DTV_EPG_GetEventTypeByID(int ri_DeMux, int ri_TsID, int ri_OrgNetId, int ri_ServiceID, int ri_EventID, int *rpi_Type);

	DTV_RESULT_e DTV_EPG_StartWatch(int ri_DeMux, EpgWatchCallBack rpn_Call);

	DTV_RESULT_e DTV_EPG_StopWatch(int ri_DeMux, EpgWatchCallBack rpn_Call);

	DTV_RESULT_e DTV_EPG_SetSearchCallBack(int ri_DeMux, EpgWatchCallBack rpn_Call);

	/*****************************************************************************************************************
	 * 注意:
	 *	本组接口中有超时参数时，考虑ibinder通讯的限制，建议不要草超过2秒。
	 *     如果要从cache中取数据，请将timeout设置为0
	 *     如果要从流中取数据，建议用表格监控
	 *****************************************************************************************************************/

	/*获取指定Table*/
    DTV_RESULT_e DTV_PSI_GetTable(int ri_Device,  int ri_Pid, 
  		 int ri_TableId, 			//byte 0
		 int ri_TableIdEx, 			//byte 3-4
		 int ri_TableIdEx2, 		//byte 8-9
		 int ri_TableIdEx3, 		//byte 10-11
		 int ri_Timeout,
		 int ri_SectionNo,
		 TableContainer *rpstru_Table);

	/*从码流中获取PMTtable*/
	DTV_RESULT_e DTV_PSI_GetPmtTable(int ri_Device, int ri_Pid, int ri_ServicId, int ri_Timeout, TableContainer *rpstru_Table);

	/*开始数据监控*/
	DTV_RESULT_e DTV_PSI_StartCapture(int ri_Device, CaptureParams *rpstru_Params, DTV_PSI_CAPTURE_HANDLE *rpstru_Handle);

	/*停止数据监控*/
	DTV_RESULT_e DTV_PSI_StopCapture(int ri_Device, DTV_PSI_CAPTURE_HANDLE rh_CaptureHandle);

	/*强制清除数据缓存*/
	DTV_RESULT_e DTV_PSI_ResetCache(int ri_Device);

	/*解析PMT表格*/
	DTV_RESULT_e DTV_PSI_ParsePmtTable(TableContainer *rpstru_PmtTable, PmtContent *rpstru_ParseResult);

	/*解析TDT表格*/
	DTV_RESULT_e DTV_PSI_getCurTDTTime(int ri_Device, DTVTime* rpstur_Time);

	/*获取空闲过滤器个数*/
	DTV_RESULT_e DTV_PSI_getUnusedFilterNumber(int ri_Device, int* rpi_freeFilterNumber);

	/*申请过滤器*/
	DTV_RESULT_e DTV_PSI_requestFilter(int ri_Device, DTV_PSI_FILTER_HANDLE* rph_FilterHander);

	/*释放过滤器*/
	DTV_RESULT_e DTV_PSI_releaseFilter(int ri_Device, DTV_PSI_FILTER_HANDLE rh_FilterHander);

	/*启动过滤器*/
	DTV_RESULT_e DTV_PSI_StartFilter(int ri_Device, DTV_PSI_FILTER_HANDLE rh_FilterHander, PSIFilterParams* rpstru_Param);

	/*停止过滤器*/
	DTV_RESULT_e DTV_PSI_StopFilter(int ri_Device, DTV_PSI_FILTER_HANDLE rh_FilterHander);

	/*解析描述子*/
	DTV_RESULT_e DTV_PSI_ParseTable(TableContainer *pTable, PsiDescript *pDes);

	
	/**************************************************************************************************************/

	/**获取节目个数如果只有一个则返回1, ID编号从0开始**/
	int DTV_TUNER_GetTunerNumber();

	/*获取TUNER的传输类型*/
	DTV_DELIVERY_SYSTEM_TYPE_e DTV_TUNER_GetTunerDeliverySystemType(int ri_Device);

	/*设置频点*/
	DTV_RESULT_e DTV_TUNER_SetDVBCCarrier(int ri_Device, DVBCCarrierParams *rpstru_CarrierParams);

	/*获取当前频点信息*/
	DTV_RESULT_e DTV_TUNER_GetCurDVBCCarrier(int ri_Device, DVBCCarrierParams *rpstru_CarrierParams);

	/*获取tuner状态*/
	DTV_RESULT_e DTV_TUNER_GetCarrierStatus(int ri_Device, CarrierStatus *rpstru_CarrierStatus);

	DTV_RESULT_e DTV_TUNER_SetCarrier(int ri_Device, DTVCarrierParamsBase *rpstru_CarrierParams);

	/*获取当前频点信息*/
	DTV_RESULT_e DTV_TUNER_GetCurCarrier(int ri_Device, DTVCarrierParamsBase **rppstru_CarrierParams);


	/************************************************************************************************************/

	/*启动*/
	DTV_RESULT_e DTV_NVOD_Start(int ri_Device);

	/*获取参考业务列表*/
	DTV_RESULT_e DTV_NVOD_GetRefServiceList(int ri_Device, NVODRefSIDContainer **rppstru_RefServiceList, int* rpi_Number);

	/*获取参考事件列表*/
	DTV_RESULT_e DTV_NVOD_GetRefEventList(int ri_Device, unsigned short rus_RefServiceID, NVODRefEventContainer **rppstru_RefEventList, int* rpi_Number);

	/*获取时移事件列表*/
	DTV_RESULT_e DTV_NVOD_GetShiftEventList(int ri_Device, unsigned short rus_RefServiceID, unsigned short rus_RefEventID, NVODTimeShiftEventContainer **rppstru_EventList, int* rpi_Number);
	/*停止*/
	DTV_RESULT_e DTV_NVOD_Stop(int ri_Device);

	/************************************************************************************************************/
	/*将搜索结果更新到数据库中，但不保存到存储器中*/
	DTV_RESULT_e DTV_SCAN_UpdateInfo(int ri_DeviceID);

	DTV_RESULT_e DTV_SCAN_EnterNitScan(int ri_DeviceID, DTVScanParam *rpstru_ScanParam);

	DTV_RESULT_e DTV_SCAN_EnterManualScan(int ri_DeviceID, DTVScanParam *rpstru_ScanParam);

	DTV_RESULT_e DTV_SCAN_EnterBandScan(int ri_DeviceID, DTVScanParam *rpstru_ScanParam);



	/*结束搜索，正常结束或用户取消搜索都需要调用该接口结束*/
	DTV_RESULT_e DTV_SCAN_ExitScan(int ri_DeviceID);

	/***********************************************MEDIA**********************************************/
	int Media_SetDataSource(const char *url);
	
	int Media_Start(void);

	int Media_Stop(void);

	int Media_SetStopMode(int ri_StopMode);
	int Media_GetStopMode(int* rpi_StopMode);
	int Media_SelectStream(int ri_StreamId,int type);
	int Media_SetVideoDisplay(int flag);
	int Media_GetVideoDisplay(int *flag);
	int Media_SetVideoArea(int x,int y,int width,int height);
	int Media_GetVideoArea(int *x,int* y,int *width,int* height);
	int Media_SetClip(int x,int y,int width,int height);
	int Media_GetClip(int *x,int* y,int *width,int* height);
	int Media_SetPauseMode(int ri_PauseMode);
	int Media_GetPauseMode(int* rpi_PauseMode);
	int Media_Pause(void);
	int Media_Resume(void);

	int Media_BindDemux(void);

	int Media_UnBindDemux(void);

	int Media_SetWindow(int x,int y,int w,int h);
	
	int Media_SetVolume(int Mute,int Volume);

	unsigned int Media_GetWindowsHandle(void);
	int Media_getOutputMatchMethod(void);
	int Media_setOutputMatchMethod(long mode); 

private:
	void DTV_PSI_ParsePmt(SectionContainer  *pstru_SectionList, PsiDescript *pDes);

	void DTV_PSI_ParseSdt(SectionContainer  *pstru_SectionList, PsiDescript *pDes);

	void DTV_PSI_ParseNit(SectionContainer  *pstru_SectionList, PsiDescript *pDes);

	void DTV_PSI_ParseBat(SectionContainer  *pstru_SectionList, PsiDescript *pDes);

	void DTV_PSI_ParseEit(SectionContainer  *pstru_SectionList, PsiDescript *pDes);

	void DTV_PSI_ParseTot(SectionContainer  *pstru_SectionList, PsiDescript *pDes);

	CH_PSI_TYPE_e DTV_PSI_GetTableType(unsigned char TableID);


};

}; // namespace android

#endif

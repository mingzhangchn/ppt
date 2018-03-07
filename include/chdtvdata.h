#ifndef CH_DTV_DATA_H
#define CH_DTV_DATA_H

#include <stdint.h>
#include <sys/types.h>
#include <utils/String8.h>
#include <binder/Parcel.h>
#include "jah_util.h"

#define DBGE(...) LOGE("%s#%d ", __FUNCTION__, __LINE__);LOGE(__VA_ARGS__);
#define DBGI(...) LOGI("%s#%d ", __FUNCTION__, __LINE__);LOGI(__VA_ARGS__);
namespace android
{

//#define sleep(x) x

typedef enum
{
	DTV_OK,
	DTV_INVALID_PARAM,
	DTV_FAIL,
	DTV_TIMEOUT
} DTV_RESULT_e;

typedef enum
{
	DTV_S,
	DTV_C,
	DTV_T,
	INVALID_SYSTEM_TYPE = -1
} DTV_DELIVERY_SYSTEM_TYPE_e;

/*声道模式*/
typedef enum
{
	DTV_DB_AUDIO_STEREO = 0,
	DTV_DB_AUDIO_LEFT,
	DTV_DB_AUDIO_RIGHT,
	DTV_DB_AUDIO_MONO,
	DTV_DB_AUDIO_NUM
} DTV_DB_AUDIO_MODE_e;

/*频谱模式（-C/-T使用）*/
typedef enum
{
	DTV_TUNER_SPECTRUM_AUTO,      				///< 自动
	DTV_TUNER_SPECTRUM_NORMAL,    				///< 正常
	DTV_TUNER_SPECTRUM_INVERTED,  				///< 频谱反转
	DTV_TUNER_SPECTRUM_MAX        				///< 非法判断
} DTV_TUNER_SPECTRUM_e;

typedef enum
{
	STREAMTYPE_UNKOWN = 0x0,
	STREAMTYPE_11172_VIDEO = 0x01,
	STREAMTYPE_13818_VIDEO = 0x02,
	STREAMTYPE_11172_AUDIO = 0x03,
	STREAMTYPE_13818_AUDIO = 0x04,
	STREAMTYPE_13818_PRIVATE = 0x05,
//	STREAMTYPE_13818_PES_PRIVATE = 0x06,
	STREAMTYPE_13818_PES_PRIVATE_AC3 = 0x06, //modified by zdj 20131031
	STREAMTYPE_13522_MHPEG = 0x07,
	STREAMTYPE_13818_DSMCC = 0x08,
	STREAMTYPE_ITU_222_1 = 0x09,
	STREAMTYPE_13818_A = 0x0a,
	STREAMTYPE_13818_B = 0x0b,
	STREAMTYPE_13818_C = 0x0c,
	STREAMTYPE_13818_D = 0x0d,
	STREAMTYPE_13818_AUX = 0x0e,
	STREAMTYPE_AAC_AUDIO = 0x0f,
	STREAMTYPE_MPEG4_AUDIO = 0x11,
	STREAMTYPE_H264_VIDEO = 0x1b,
	STREAMTYPE_AVS_VIDEO = 0x42,
	STREAMTYPE_AC3_AUDIO = 0x81,
	STREAMTYPE_DTS_AUDIO = 0x82,
	STREAMTYPE_DRA_AUDIO = 0x90,
	STREAMTYPE_H265_VIDEO = 0x24,
} DTV_DB_STREAM_TYPE_e;

/*EPG更新类型*/
typedef enum
{
	DTV_EPG_UPDATE_PF,	///PF更新
	DTV_EPG_UPDATE_SCH,	///SCH更新
	DTV_EPG_UPDATE_ALL,   ///所有更新完
	DTV_EPG_UPDATE_COUNTMX,   ///达到最大值
	DTV_EPG_UPDATE_TIMEOUT,  ///超时
	DTV_EPG_UPDATE_MAX
} DTV_EPG_UPDATETYPE_e;

#define PSI_FILTER_DEPTH 			16
#define EPG_MAX_CONTENT_ITEMS 	16
#define DTV_MAX_STREAM_NUM		40	

class EPGUpdateInfo;

/*EPG监控回调函数*/
typedef void (*EpgWatchCallBack)(int ri_DeMux, EPGUpdateInfo *rpstru_UpdateEpgInfo);

/*时间数据结构定义*/
class DTVTime
{
public:
	unsigned short us_mjd; /*简化儒略日*/
	unsigned char uc_hours; /*小时*/
	unsigned char uc_mins; /*分钟*/
	unsigned char uc_secs; /*秒*/
	int i_BcdZone; /*时区*/
	void getDayMember(int*rpi_Year, int* rpi_Month, int* rpi_Day, int* rpi_WeekDay);
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~DTVTime();
};

class CaInfo
{
public:
	int cas_id;/* CA厂商ID，对应K-LAD的，vendor_SysID */
	int ecm_pid;/* 节目ID */
	int emm_pid;/* EMM PID optional */
	
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
};

class StreamInfo
{
public:
	int i_StreamPid;
	int i_StreamType;
	int i_ComponentTag;
	int association_tag;
	char uc_Language[4];
	int us_CaSystemID;
	int i_MaxBitrate;			//单位：bps
	int i_VideoWidth;			//单位：像素
	int i_VideoHeight;			//单位：像素
	int i_CA_num;
	CaInfo stru_CaData[8];					//2015年12月1日新加

	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	StreamInfo();
};

/*节目信息*/
class DTVChannelInfo
{
public:
	int i_ChannelID;
	int i_CarrierIndex;
	int i_LogicNO;
	int i_TsID;
	int i_OrgNetId;
	int i_ServiceID;
	int i_ChannelType;
	int i_Category;
	int i_CaFlag;
	int i_PmtPid;
	int i_PcrPid;
	int i_VideoPid;
	DTV_DB_STREAM_TYPE_e enm_VideoType;		//视频编码格式
	int i_AudioPid;
	DTV_DB_STREAM_TYPE_e enm_AudioType;		//音频编码格式
	int i_TxtPid;
	int i_SubtitlePid;
	int i_CaSystemID;
	String8 str_ChannelName;
	String8 str_ProviderName;
	int i_AudioTrack;
	int i_bouquetIdNumb;
	int pi_bouquetId[2]; 				//搜索时全赋0，由应用修改
	int i_eitScheduleFlag;
	int i_eitPresentFollowingFlag;
	int uc_VolumeComPensat;
	int i_run_status;
	int i_IsDelete;
	int i_IsFavorite;
	int i_IsLocked;
	int i_IsSkiped;
	int i_IsHide;
	int i_SelectFlag;
	int i_DeltVolume;
	int i_SupportPlaybacke;
	int uc_StreamNum;
	StreamInfo *pstru_Stream;
	int uc_Reserved;

	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	DTVChannelInfo()
	{
		int i = 0;
		i_ChannelID = 0;
		pstru_Stream = NULL;
		uc_StreamNum = 0;
		str_ChannelName = String8("null");
		str_ProviderName = String8("null");
		i_bouquetIdNumb = 0;
		for (i = 0; i < 2; i ++)
		{
			pi_bouquetId[i] = 0xffff;
		}
	}
	~DTVChannelInfo();
};

class DTVNvodList
{
public:
	int i_NvodChID;
	int i_ServiceID;
	int i_EitScheduleFlag;
	int i_EitPresentFollowingFlag;
	int i_FreeCAMode;
	int i_ServiceType; 				//0x4 nvod参考节目 ;0x5 nvod时移节目
	int i_RefTsID;
	int i_RefOrgNetId;
	int i_RefServiceID;
	String8 str_ServiceName;
	String8 str_ProviderName;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	DTVNvodList()
	{
		str_ServiceName = String8("null");
		str_ProviderName = String8("null");
	};
};

typedef enum
{
    DTV_TUNER_FE_POLARIZATION_H,       	  /**<Horizontal Polarization*/           /**<CNcomment:水平极化*/
    DTV_TUNER_FE_POLARIZATION_V,        	 /**<Vertical Polarization*/             /**<CNcomment:垂直极化*/
    DTV_TUNER_FE_POLARIZATION_L,        	 /**<Left-hand circular Polarization*/   /**<CNcomment:左旋圆极化*/
    DTV_TUNER_FE_POLARIZATION_R,         	/**<Right-hand circular Polarization*/  /**<CNcomment:右旋圆极化*/
    DTV_TUNER_FE_POLARIZATION_INVALID,      /**<Invalid value*/                     /**<CNcomment:非法边界值*/
} DTV_TUNER_FE_POLARIZATION_E;

/*频点信息*/
class DVBCCarrierInfo
{
public:
	DTV_DELIVERY_SYSTEM_TYPE_e	enm_ModeType;
	int i_CarrierIndex;
	int i_TsID;
	int i_NetID;
	int i_OrgNetId;
	int i_FrequencyKHz;
	int i_SymbolRateKb;
	int i_QAM;
//dtmb 增加T和S的
	int							i_bandWidth;
//abs
	DTV_TUNER_FE_POLARIZATION_E enm_DVBTPrio;

	String8 str_name;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	DVBCCarrierInfo()
	{
		str_name = String8("null");
	}
	~DVBCCarrierInfo();
};

/*Bouquet信息*/
class BouquetInfo
{
public:
	int i_NetworkID;
	int i_BouquetID;
	String8 str_Name;
	String8 str_Shortname;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	BouquetInfo();
	~BouquetInfo();
};

/*网络信息*/
class NetworkInfo
{
public:
	int i_NetworkID;
	String8 str_Name;
	String8 str_Shortname;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	NetworkInfo();
	~NetworkInfo();
};

/************epg数据****************************/
/*EPG最长名称*/
#define  DTV_EPG_MAX_NAME_LENGTH		(32)

/*家长分级结构*/
class EPGParating
{
public:
	unsigned int ui_Country_Code;
	int ui_Rating;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~EPGParating();
};

/*内容分级*/

class EPGNibble
{
public:
	EPGNibble();
	int i_NibbleNum;
	unsigned char uc_NibbleLevel[EPG_MAX_CONTENT_ITEMS];
	unsigned char uc_UserNibble[EPG_MAX_CONTENT_ITEMS];
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
};
/*基本事件信息结构*/
class EPGEventInfo
{
public:
	int i_SectionNumTag;
	int i_EventID;
	String8 uc_EventName;
	DTVTime stru_StartTime;
	unsigned int ui_Language;
	struct
	{
		int i_Hour;
		int i_Minute;
		int i_Second;
	} stru_Duration;
	String8 str_ShortText;
	int i_freeCA;
	EPGParating stru_Parating;
	EPGNibble stru_nibbles;
	int i_runningStatus;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	EPGEventInfo();
	~EPGEventInfo();
};

/*单个事件信息(用于多个业务事件混合存放场景)*/
class EPGSingleEventInfo
{
public:
	unsigned short us_TsID;
	unsigned short us_OrgNetId;
	unsigned short us_ServiceID;
	EPGEventInfo stru_Event_Info;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	virtual ~EPGSingleEventInfo();
};

class EPGUpdateInfo
{
public:
	unsigned short us_TsID;
	unsigned short us_OrgNetId;
	unsigned short us_ServiceID;
	DTV_EPG_UPDATETYPE_e enm_Type;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
};

class EPGSearchCallBackInfo
{
public:
	unsigned short us_TsID;
	unsigned short us_OrgNetId;
	unsigned short us_ServiceID;
	unsigned short us_Type;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
};

/*当前后续信息结构*/
class EPGPfEventInfo
{
public:
	unsigned short us_TsID;
	unsigned short us_OrgNetId;
	unsigned short us_ServiceID;
	unsigned char uc_VersionNum;
	EPGEventInfo stru_Pf_Info[2];
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	EPGPfEventInfo();
	~EPGPfEventInfo();
};

/*周报信息结构*/
class EPGSCHEventInfo
{
public:
	unsigned short us_TsID;
	unsigned short us_OrgNetId;
	unsigned short us_ServiceID;
	unsigned char uc_VersionNum;
	int i_EventNumb;
	EPGEventInfo *pstru_Sch_Info;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	EPGSCHEventInfo();
	~EPGSCHEventInfo();

};

/*Section数据结构*/
class SectionContainer
{
public:
	unsigned char *puc_SectionData;
	int i_SectionNumber;
	int i_SectionLength;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~SectionContainer();
	SectionContainer();
};

/*TABLE数据结构*/
class TableContainer
{
public:
	int i_SectionNumb;
	SectionContainer *pstru_SectionList;
	int i_Pid;
	int i_TableId;
	int i_Syntax;
	int i_TableIdEx;
	int i_TableIdEx2;
	int i_TableIdEx3;
	int i_Version;
	int i_LastSectionNumber;
	int i_type;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	TableContainer();
	~TableContainer();
};

typedef struct
{
	unsigned char len;
	unsigned char tag;
}des_header_t;

class DescriptData
{
public:
	
	des_header_t header;
	unsigned char *data;

	
	DescriptData(){data = NULL;header.len = 0;}
	~DescriptData(){if(data)delete[] data;data=NULL;}
};

typedef struct
{
	unsigned int invalid_bytes2 : 16;
	unsigned int descriptors_loop_length : 12;
	unsigned int free_CA_mode : 1;
	unsigned int running_status : 3;

	unsigned int invalid_bytes1 : 8;
	unsigned int EIT_present_following_flag : 1;
	unsigned int EIT_schedule_flag : 1;
	unsigned int reserved_futrue_use : 6;
	unsigned int service_id : 16;	
}sdt_service_header_t;

typedef struct
{
	unsigned int invalid_bytes3 : 8;
	unsigned int reserved_futrue_use2 : 8;
	unsigned int original_network_id : 16;

	unsigned int invalid_bytes2 : 8;
	unsigned int last_section_number : 8;
	unsigned int section_number : 8;
	unsigned int current_next_indicator : 1;
	unsigned int version_number : 5;
	unsigned int reserved2 : 2;
	
	unsigned int transport_stream_id : 16;
	unsigned int section_length : 12;
	unsigned int reserved1 : 2;
	unsigned int reserved_futrue_use1 : 1;
	unsigned int section_sytax_indicator : 1;

	unsigned int invalid_bytes1 : 24;
	unsigned int table_id : 8;
}sdt_des_header_t;

typedef struct
{
	unsigned int ES_info_length : 12;
	unsigned int reserved2 : 4;
	unsigned int elementary_PID : 13;
	unsigned int reserved1 : 3;

	unsigned int invalid_bytes1 : 24;
	unsigned int stream_type : 8;	
}pmt_stream_header_t;

typedef struct
{
	unsigned int program_info_length : 12;
	unsigned int reserved4 : 4;
	unsigned int PCR_PID : 13;
	unsigned int reserved3 : 3;

	unsigned int invalid_bytes2 : 8;
	unsigned int last_section_number : 8;
	unsigned int section_number : 8;
	unsigned int current_next_indicator : 1;
	unsigned int version_number : 5;
	unsigned int reserved2 : 2;
	
	unsigned int program_number : 16;
	unsigned int section_length : 12;
	unsigned int reserved1 : 3;
	unsigned int section_sytax_indicator : 1;

	unsigned int invalid_bytes1 : 24;
	unsigned int table_id : 8;	
}pmt_des_header_t;

typedef struct
{
	unsigned int invalid_bytes1 : 16;
	unsigned int transport_descriport_length : 12;
	unsigned int reserved_future_use : 4;
	unsigned int original_network_id : 16;
	unsigned int transport_stream_id : 16;	
}nit_ts_header_t;

typedef struct
{
	unsigned int invalid_bytes3 : 16;
	unsigned int network_descriptor_length : 12;
	unsigned int reserved_future_use2 : 4;

	unsigned int invalid_bytes2 : 8;
	unsigned int last_section_number : 8;
	unsigned int section_number : 8;
	unsigned int current_next_indicator : 1;
	unsigned int version_number : 5;
	unsigned int reserved2 : 2;
	
	unsigned int network_id : 16;
	unsigned int section_length : 12;
	unsigned int reserved1 : 2;
	unsigned int reserved_future_use : 1;
	unsigned int section_sytax_indicator : 1;

	unsigned int invalid_bytes1 : 24;
	unsigned int table_id : 8;	
}nit_des_header_t;

typedef struct
{
	unsigned int invalid_bytes3 : 16;
	unsigned int boquet_descriptor_length : 12;
	unsigned int reserved_future_use2 : 4;

	unsigned int invalid_bytes2 : 8;
	unsigned int last_section_number : 8;
	unsigned int section_number : 8;
	unsigned int current_next_indicator : 1;
	unsigned int version_number : 5;
	unsigned int reserved2 : 2;
	
	unsigned int boquet_id : 16;
	unsigned int section_length : 12;
	unsigned int reserved1 : 2;
	unsigned int reserved_future_use1 : 1;
	unsigned int section_sytax_indicator : 1;

	unsigned int invalid_bytes1 : 24;
	unsigned int table_id : 8;	
}bat_des_header_t;

typedef struct
{
	unsigned int invalid_bytes1 : 16;
	unsigned int transport_descriport_length : 12;
	unsigned int reserved_future_use : 4;
	unsigned int original_network_id : 16;
	unsigned int transport_stream_id : 16;	
}bat_ts_header_t;

class SdtServiceDes
{
public:
	
	sdt_service_header_t header;
	unsigned int des_nums : 8;//activity
	DescriptData *pDes;


	
	SdtServiceDes();
	~SdtServiceDes();
};



class SdtDes
{
public:
	
	sdt_des_header_t header;
	unsigned int service_nums : 8;//activity;
	SdtServiceDes *pService;
	unsigned int CRC_32;
	
	SdtDes();
	~SdtDes();
};

class PmtStreamDes
{
public:
	
	pmt_stream_header_t header;
	unsigned int des_nums : 8;
	DescriptData *pDes;
	
	PmtStreamDes();
	~PmtStreamDes();
};



class PmtDes
{
public:
	
	pmt_des_header_t header;
	unsigned int des_nums : 8;
	DescriptData *pDes;
	unsigned int stream_nums : 8;
	PmtStreamDes *pStream;
	unsigned int CRC_32;

	

	PmtDes();
	~PmtDes();
};


class NitTsDes
{
public:
	
	nit_ts_header_t header;
	unsigned int des_nums : 8;
	DescriptData *pDes;

	NitTsDes();
	~NitTsDes();
};


typedef struct
{
	unsigned short transport_stream_loop_length : 12;
	unsigned short reserved_future_use : 4;
}des_length_12bit_t;

class NitDes
{
public:
	
	nit_des_header_t header;
	unsigned int des_nums : 8;
	DescriptData *pDes;
	des_length_12bit_t len;
	unsigned int ts_nums : 8;
	NitTsDes *pTs;
	unsigned int CRC_32;

	NitDes();
	~NitDes();
};



class BatTsDes
{
public:
	
	bat_ts_header_t header;
	unsigned int des_nums : 8;
	DescriptData *pDes;


	
	BatTsDes();
	~BatTsDes();
};


class BatDes
{
public:
	
	bat_des_header_t header;
	unsigned int des_nums : 8;
	DescriptData *pDes;
	des_length_12bit_t len;
	unsigned int ts_nums : 8;
	BatTsDes *pTs;
	unsigned int CRC_32;


	BatDes();
	~BatDes();
};

typedef struct
{
	unsigned int invalid_bytes3 : 16;
	unsigned int last_table_id : 8;
	unsigned int segment_last_section_number : 8;

	unsigned int original_network_id : 16;
	unsigned int transport_stream_id : 16;
	

	unsigned int invalid_bytes2 : 8;
	unsigned int last_section_number : 8;
	unsigned int section_number : 8;
	unsigned int current_next_indicator : 1;
	unsigned int version_number : 5;
	unsigned int reserved2 : 2;
	
	unsigned int service_id : 16;
	unsigned int section_length : 12;
	unsigned int reserved1 : 2;
	unsigned int reserved_future_use1 : 1;
	unsigned int section_sytax_indicator : 1;

	unsigned int invalid_bytes1 : 24;
	unsigned int table_id : 8;	
}eit_des_header_t;



typedef struct
{
	unsigned int invalid_bytes2 : 16;
	unsigned int descriptors_loop_length : 12;
	unsigned int free_CA_mode : 1;
	unsigned int running_status : 3;
	
	unsigned int duration : 24;
	unsigned int second : 8;
	
	unsigned int minute : 8;
	unsigned int hours : 8;
	unsigned int mjd : 16;
	
	unsigned int invalid_bytes1 : 16;
	unsigned int event_id : 16;	
}eit_event_header_t;

class EitEvent
{
public:	
	eit_event_header_t header;
	unsigned int des_nums : 8;
	DescriptData *pDes;

	EitEvent();
	~EitEvent();
};

class EitDes
{
public:
	
	eit_des_header_t header;
	unsigned int event_nums : 8;
	EitEvent *pEvent;
	unsigned int CRC_32;

	EitDes();
	~EitDes();
};

typedef struct
{
	unsigned int invalid_bytes3 : 16;
	des_length_12bit_t len;

	unsigned int invalid_bytes2 : 8;
	unsigned int second : 8;
	unsigned int minute : 8;
	unsigned int hours : 8;

	unsigned int mjd : 16;
	unsigned int section_length : 12;
	unsigned int reserved1 : 2;
	unsigned int reserved_future_use1 : 1;
	unsigned int section_sytax_indicator : 1;

	unsigned int invalid_bytes1 : 24;
	unsigned int table_id : 8;	
}tot_header_t;

//[73 70 27 db 70 27 db d2 12 23 19 f0 f0 1c 58 1a]

class TotDes
{
public:	
	tot_header_t header;
	unsigned int des_nums : 8;
	DescriptData *pDes;
	unsigned int CRC_32;

	TotDes();
	~TotDes();
};

typedef enum
{
	E_CH_PSI_OTHER,
	E_CH_PSI_PMT,
	E_CH_PSI_SDT,
	E_CH_PSI_NIT,
	E_CH_PSI_BAT,
	E_CH_PSI_EIT,
	E_CH_PSI_TOT,
	E_CH_PSI_TYPE_MAX,
}CH_PSI_TYPE_e;

typedef union
{
	BatDes *bat;
	PmtDes *pmt;
	SdtDes *sdt;
	NitDes *nit;
	EitDes *eit;
	TotDes *tot;
}PsiDes_u;

class PsiDescript
{
public:
	
	CH_PSI_TYPE_e type;
	PsiDes_u psi;
	PsiDescript();
	~PsiDescript();
};
typedef void* DTV_PSI_CAPTURE_HANDLE;

typedef void* DTV_PSI_FILTER_HANDLE;

/*数据监控参数*/
class CaptureParams
{
public:
	int i_Pid;
	int i_TableId;
	int i_TableIdEx;
	int i_TableIdEx2;
	int i_TableIdEx3;

	int i_dataLen; /*过滤比较数据长度:0~16*/
	unsigned char data[PSI_FILTER_DEPTH]; /*比较数据*/
	unsigned char dataMask[PSI_FILTER_DEPTH]; /*需要比较的位*/
	
	void (*fn_UpdateCallback)(int Device, DTV_PSI_CAPTURE_HANDLE Handle, TableContainer *pstru_Table);
	void (*fn_SectionCallback)(int Device, DTV_PSI_CAPTURE_HANDLE Handle, SectionContainer *pstru_Section);
	~CaptureParams()
	{
	}
};

/*数据监控参数*/
class PSIFilterParams
{
public:
	int i_Pid;
	int i_dataLen; /*过滤比较数据长度:0~16*/
	unsigned char data[PSI_FILTER_DEPTH]; /*比较数据*/
	unsigned char dataMask[PSI_FILTER_DEPTH]; /*需要比较的位*/
//	unsigned char  patern[PSI_FILTER_DEPTH];			/*不等模式使用的位*/
	unsigned char negData[PSI_FILTER_DEPTH];
	unsigned char negMask[PSI_FILTER_DEPTH];
	void (*fn_FilterCallback)(int Device, DTV_PSI_FILTER_HANDLE Handle, SectionContainer *pstru_Section);
	~PSIFilterParams()
	{
	}
};

/*PMT表格内容相关定义*/

/*CA描述子*/
class PmtCaDes
{
public:
	int i_CaSystemID;
	int i_CaPid;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~PmtCaDes();
};

/*单元流描述子*/
class PmtStream
{
public:
	int i_EsPid;
	int i_StreamType;
	unsigned int ui_Language;

	int i_MaxBitrate;
	int i_VideoWidth;
	int i_VideoHeight;

	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~PmtStream(); 				//{};
};

/*PMT表格内容*/
class PmtContent
{
public:
#define DTV_PMT_MAX_CA_NUM		8
#define DTV_PMT_MAX_STREAM_NUM	8
	
	int i_ProgNumber;
	int i_PcrPID;
	int i_SectionVersion;
	int i_SectionNum;
	int i_CaDesNum;
	int i_StreamNum;
	unsigned int ui_CRC32;
	PmtCaDes stru_CaDes[DTV_PMT_MAX_CA_NUM];
	PmtStream stru_Stream[DTV_PMT_MAX_STREAM_NUM];
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~PmtContent();
};

/*************turner****************/
/*Tuner锁定状态*/
typedef enum
{
	DTV_TUNER_CARRIER_LOCK,
	DTV_TUNER_CARRIER_UNLOCK,
	DTV_TUNER_TURNING
} DTV_TUNER_LOCK_STATUS_e;

/*QAM调制模式分类（-C使用）*/
typedef enum
{
	DTV_TUNER_QAM_4, /* QAM 4*/
	DTV_TUNER_QAM_8, /*QAM 8*/
	DTV_TUNER_QAM_16, /* QAM 16*/
	DTV_TUNER_QAM_32, /* QAM 32*/
	DTV_TUNER_QAM_64, /* QAM 64*/
	DTV_TUNER_QAM_128, /* QAM 128*/
	DTV_TUNER_QAM_256, /* QAM 256*/
	DTV_TUNER_QAM_512, /* QAM 512*/
	DTV_TUNER_QAM_1024, /* QAM 1024*/
} DTV_TUNER_QAM_MODE_e;



typedef enum
{
 	DTV_TUNER_MOD_QAM_4, /* QAM 4*/
	DTV_TUNER_MOD_QAM_8, /*QAM 8*/
	DTV_TUNER_MOD_QAM_16, /* QAM 16*/
	DTV_TUNER_MOD_QAM_32, /* QAM 32*/
	DTV_TUNER_MOD_QAM_64, /* QAM 64*/
	DTV_TUNER_MOD_QAM_128, /* QAM 128*/
	DTV_TUNER_MOD_QAM_256, /* QAM 256*/
	DTV_TUNER_MOD_QAM_512, /* QAM 512*/
	DTV_TUNER_MOD_QAM_1024, /* QAM 1024*/

	DTV_TUNER_MOD_BPSK, /**QPSK*/  
	DTV_TUNER_MOD_QPSK,    
	DTV_TUNER_MOD_DQPSK,
	DTV_TUNER_MOD_8PSK,    
	DTV_TUNER_MOD_16APSK, 
	DTV_TUNER_MOD_32APSK,
	DTV_TUNER_MOD_8VSB, 
	DTV_TUNER_MOD_16VSB,
	DTV_TUNER_MOD_S_AUTO,
	DTV_TUNER_MOD_INVALID              /**<Invalid Modulation mode*/       /**<CNcomment:非法的调制类型枚举值*/
}DTV_TUNER_MOD_TYPE_E;

typedef enum
{
    DTV_TUNER_TER_MODE_BASE = 0,     /**< the channel is base mode*/         /**<CNcomment:通道中仅支持base信号*/
    DTV_TUNER_TER_MODE_LITE,        	 /**< the channel is lite mode*/         /**<CNcomment:通道中需要支持lite信号*/
    DTV_TUNER_TER_MODE_INVALID      	/**<Invalid value*/            /**<CNcomment:非法边界值*/
} DTV_TUNER_TER_MODE_E;

typedef enum
{
    DTV_TUNER_TS_PRIORITY_NONE = 0,      /**<no priority mode*/          /**<CNcomment:无优先级模式*/
    DTV_TUNER_TS_PRIORITY_HP,            	/**<high priority mode*/        /**<CNcomment:高优先级模式*/
    DTV_TUNER_TS_PRIORITY_LP,           	 /**<low priority mode*/         /**<CNcomment:低优先级模式*/
    DTV_TUNER_TS_PRIORITY_INVALID          	 /**<Invalid value*/             /**<CNcomment:非法边界值*/
} DTV_TUNER_TS_PRIORITY_E;

/*频点参数数据结构（-C使用）*/
class DTVCarrierParamsBase
{
public:
	DTV_DELIVERY_SYSTEM_TYPE_e	enm_ModeType;
	unsigned int 			ui_FrequencyKhz;
	virtual void writeToParcel(Parcel* outData){};
	virtual void readFromParcel(const Parcel* inData){};
	DTVCarrierParamsBase(){};
	DTVCarrierParamsBase(DTV_DELIVERY_SYSTEM_TYPE_e enm_Type){enm_ModeType = enm_Type;};
	~DTVCarrierParamsBase(){};
};

/*频点参数数据结构（-C使用）*/
class DVBCCarrierParams : public DTVCarrierParamsBase
{
public:
	unsigned int ui_SymbolRateKbps;
	DTV_TUNER_MOD_TYPE_E enm_QAMMode;
	DTV_TUNER_SPECTRUM_e enm_Spectrum;
	virtual void writeToParcel(Parcel* outData);
	virtual void readFromParcel(const Parcel* inData);
	DVBCCarrierParams(){};
	~DVBCCarrierParams();
};

/*频点参数数据结构（-S使用）*/
class DTVSCarrierParams : public DTVCarrierParamsBase
{
public:
	unsigned int 					ui_SymbolRateKbps;
	DTV_TUNER_FE_POLARIZATION_E     enm_Polar;
	DTV_TUNER_MOD_TYPE_E			enm_mod;
	int   							i_scrambleValue;
	DTVSCarrierParams(){};
	virtual void writeToParcel(Parcel* outData);
	virtual void readFromParcel(const Parcel* inData);
	~DTVSCarrierParams();
};

/*频点参数数据结构（-T使用）*/
class DTVTCarrierParams : public DTVCarrierParamsBase
{
public:
	unsigned  int              			ui_bandWidth;
	DTV_TUNER_TER_MODE_E       	enm_ChannelMode;      //dvb-t2
	DTV_TUNER_TS_PRIORITY_E    	enm_DVBTPrio;         //dvb-t
	DTV_TUNER_MOD_TYPE_E 		enm_QAMMode;
	DTV_TUNER_SPECTRUM_e 		enm_Spectrum;
	unsigned char        				bReverse;
	DTVTCarrierParams(){};
	virtual void writeToParcel(Parcel* outData);
	virtual void readFromParcel(const Parcel* inData);
	~DTVTCarrierParams();
};

/*  频点属性数据结构*/
class CarrierStatus
{
public:
	int i_SignLevel;			//0~100
	int i_SignQuality;		//0~100
	int i_BitErrorRate;		//单位为10的-9次方
	int i_SNR;
	DTV_TUNER_LOCK_STATUS_e enm_LockStatus;
	DTV_TUNER_SPECTRUM_e enm_Spectrum;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~CarrierStatus();
};

/*NVOD参考业务最长名称*/
#define  DTV_NVOD_MAX_NAME_LENGTH		(32)

/*NVOD短事件最长名称*/
#define  DTV_NVOD_MAX_SHORTTEXT_LENGTH (256) 

/*时移事件结构*/
class NVODTimeShiftEventContainer
{
public:
	unsigned short us_TsID;
	unsigned short us_OrgNetID;
	unsigned short us_ServiceID;
	unsigned short us_EventID;
	DTVTime stru_StartTime;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~NVODTimeShiftEventContainer();
};

/*参考事件结构*/
class NVODRefEventContainer
{
public:
	unsigned short us_ReferenceEventID;
	String8 str_EventName;
	unsigned int ui_Language;
	String8 str_ShortText;
	unsigned char uc_VersionNum;
	struct
	{
		unsigned char uc_Hour;
		unsigned char uc_Minute;
		unsigned char uc_Second;
	} stru_Duration;
//	int 	i_TimeshiftEventNumb;
//	NVODTimeShiftEventContainer *pstru_EventList;
	NVODRefEventContainer(){str_EventName = " "; str_ShortText = " ";};
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~NVODRefEventContainer();
};

/*参考业务结构*/
class NVODRefSIDContainer
{
public:
	unsigned short us_TsID;
	unsigned short us_OrgNetID;
	unsigned short us_ReferenceServiceID;
	unsigned char uc_ServiceName[DTV_NVOD_MAX_NAME_LENGTH];
	unsigned char uc_ProviderName[DTV_NVOD_MAX_NAME_LENGTH];
//	int i_RefEventNumb;
//	NVODRefEventContainer  *pstru_RefEventList;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~NVODRefSIDContainer();
};

/************************搜索数据对象*****************************************/

/*搜索状态*/
typedef enum
{
	DTV_STATUS_NIT_BEGIN = 0, /*开始搜索NIT表*/
	DTV_STATUS_NIT_DONE, /*NIT表搜索完成*/
	DTV_STATUS_TUNERSET_DONE, /*设置频点完成(未开始节目数据搜索)*/
	DTV_STATUS_ONE_SERVICE, /*当前频点节目*/
	DTV_STATUS_SERVICE_DONE, /*当前频点节目搜索完成*/
	DTV_STATUS_SCAN_BEGIN, /*节目搜索开始*/
	DTV_STATUS_SCAN_DONE, /*所有节目搜索完成*/
	DTV_STATUS_SCAN_FAIL, /*节目搜索失败*/
	DTV_STATUS_TUNER_FAIL, /*因为信号问题搜索失败*/
	DTV_STATUS_USER_STOP, /*APP取消搜索*/
	DTV_STATUS_DB_SAVE_DONE,
	DTV_STATUS_DB_SAVE_FAIL,
	DTV_STATUS_NUM
} DTV_SCAN_STATUS_e;

/*搜索到的部分数据值*/
class ScanStatusInfo
{
public:
	
	DTV_SCAN_STATUS_e enm_Status; /*当前节目搜索状态*/
	int us_CarrCount; /*频点总数*/
	int us_CurCarrID; /*频点索引号*/
	int us_CurCarrChCount; /*当前频点中的节目个数*/
	int us_ChannelCount; /*总的节目个数*/
	int us_TVCount; /*总的电视节目个数*/
	int us_RadioCount; /*总的广播节目个数*/
	int us_BatCount;	/*总的BAT个数*/
	bool b_TunerLockStatus; /*TUNER锁定状态*/
	DTVChannelInfo ChannelInfo; /*节目信息，只包含基本信息(当状态信息为DTV_STATUS_ONE_SERVICE时传递该对象)*/
	DVBCCarrierInfo CurCarrierInfo; /**当前频点信息**/
	DVBCCarrierInfo *pAllCarrierInfo; /*总的频点列表*/
	BouquetInfo    CurBouquetInfo;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~ScanStatusInfo();
	ScanStatusInfo();
};

class DTVScanParam
{
public:
	DTV_RESULT_e (*OSDCallback)(ScanStatusInfo *rpstru_ScanStatus); /*回调通知*/
	int us_MaxMainCarrNum; /*输入 主频点个数*/
	DTVCarrierParamsBase *pstru_MainCarrList; /*输入 主频点列表*/
	~DTVScanParam(){};
};

class EPGTSInfo
{
public:
	int i_netID;
	int i_orgNetID;
	int i_tsID;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	EPGTSInfo()
	{
		i_netID = 0;
		i_orgNetID = 0;
		i_tsID = 0;
	};
	~EPGTSInfo();
};

class EPGSearchInfo
{
public:
	int i_type;
	int i_mask;
	int i_timeout;
	int i_maxnum;
	int i_startDay;
	int i_searchDay;
	int i_netID;
	int i_orgNetID;
	int i_tsID;
	int i_ServiceID;	
	int i_tsNum;
	EPGTSInfo m_tsinfo[64];
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	EPGSearchInfo()
	{
		i_type = 0;
		i_mask = 0;
		i_timeout = 0;
		i_maxnum = 0;
		i_startDay = 0;
		i_searchDay = 0;
		i_tsNum = 0;
		i_netID = 0;
		i_orgNetID = 0;
		i_tsID = 0;
		i_ServiceID = 0;	
	}
	~EPGSearchInfo();
};

};//end namespace

#endif

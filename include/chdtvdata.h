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

/*����ģʽ*/
typedef enum
{
	DTV_DB_AUDIO_STEREO = 0,
	DTV_DB_AUDIO_LEFT,
	DTV_DB_AUDIO_RIGHT,
	DTV_DB_AUDIO_MONO,
	DTV_DB_AUDIO_NUM
} DTV_DB_AUDIO_MODE_e;

/*Ƶ��ģʽ��-C/-Tʹ�ã�*/
typedef enum
{
	DTV_TUNER_SPECTRUM_AUTO,      				///< �Զ�
	DTV_TUNER_SPECTRUM_NORMAL,    				///< ����
	DTV_TUNER_SPECTRUM_INVERTED,  				///< Ƶ�׷�ת
	DTV_TUNER_SPECTRUM_MAX        				///< �Ƿ��ж�
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

/*EPG��������*/
typedef enum
{
	DTV_EPG_UPDATE_PF,	///PF����
	DTV_EPG_UPDATE_SCH,	///SCH����
	DTV_EPG_UPDATE_ALL,   ///���и�����
	DTV_EPG_UPDATE_COUNTMX,   ///�ﵽ���ֵ
	DTV_EPG_UPDATE_TIMEOUT,  ///��ʱ
	DTV_EPG_UPDATE_MAX
} DTV_EPG_UPDATETYPE_e;

#define PSI_FILTER_DEPTH 			16
#define EPG_MAX_CONTENT_ITEMS 	16
#define DTV_MAX_STREAM_NUM		40	

class EPGUpdateInfo;

/*EPG��ػص�����*/
typedef void (*EpgWatchCallBack)(int ri_DeMux, EPGUpdateInfo *rpstru_UpdateEpgInfo);

/*ʱ�����ݽṹ����*/
class DTVTime
{
public:
	unsigned short us_mjd; /*��������*/
	unsigned char uc_hours; /*Сʱ*/
	unsigned char uc_mins; /*����*/
	unsigned char uc_secs; /*��*/
	int i_BcdZone; /*ʱ��*/
	void getDayMember(int*rpi_Year, int* rpi_Month, int* rpi_Day, int* rpi_WeekDay);
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~DTVTime();
};

class CaInfo
{
public:
	int cas_id;/* CA����ID����ӦK-LAD�ģ�vendor_SysID */
	int ecm_pid;/* ��ĿID */
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
	int i_MaxBitrate;			//��λ��bps
	int i_VideoWidth;			//��λ������
	int i_VideoHeight;			//��λ������
	int i_CA_num;
	CaInfo stru_CaData[8];					//2015��12��1���¼�

	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	StreamInfo();
};

/*��Ŀ��Ϣ*/
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
	DTV_DB_STREAM_TYPE_e enm_VideoType;		//��Ƶ�����ʽ
	int i_AudioPid;
	DTV_DB_STREAM_TYPE_e enm_AudioType;		//��Ƶ�����ʽ
	int i_TxtPid;
	int i_SubtitlePid;
	int i_CaSystemID;
	String8 str_ChannelName;
	String8 str_ProviderName;
	int i_AudioTrack;
	int i_bouquetIdNumb;
	int pi_bouquetId[2]; 				//����ʱȫ��0����Ӧ���޸�
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
	int i_ServiceType; 				//0x4 nvod�ο���Ŀ ;0x5 nvodʱ�ƽ�Ŀ
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
    DTV_TUNER_FE_POLARIZATION_H,       	  /**<Horizontal Polarization*/           /**<CNcomment:ˮƽ����*/
    DTV_TUNER_FE_POLARIZATION_V,        	 /**<Vertical Polarization*/             /**<CNcomment:��ֱ����*/
    DTV_TUNER_FE_POLARIZATION_L,        	 /**<Left-hand circular Polarization*/   /**<CNcomment:����Բ����*/
    DTV_TUNER_FE_POLARIZATION_R,         	/**<Right-hand circular Polarization*/  /**<CNcomment:����Բ����*/
    DTV_TUNER_FE_POLARIZATION_INVALID,      /**<Invalid value*/                     /**<CNcomment:�Ƿ��߽�ֵ*/
} DTV_TUNER_FE_POLARIZATION_E;

/*Ƶ����Ϣ*/
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
//dtmb ����T��S��
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

/*Bouquet��Ϣ*/
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

/*������Ϣ*/
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

/************epg����****************************/
/*EPG�����*/
#define  DTV_EPG_MAX_NAME_LENGTH		(32)

/*�ҳ��ּ��ṹ*/
class EPGParating
{
public:
	unsigned int ui_Country_Code;
	int ui_Rating;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~EPGParating();
};

/*���ݷּ�*/

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
/*�����¼���Ϣ�ṹ*/
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

/*�����¼���Ϣ(���ڶ��ҵ���¼���ϴ�ų���)*/
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

/*��ǰ������Ϣ�ṹ*/
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

/*�ܱ���Ϣ�ṹ*/
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

/*Section���ݽṹ*/
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

/*TABLE���ݽṹ*/
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

/*���ݼ�ز���*/
class CaptureParams
{
public:
	int i_Pid;
	int i_TableId;
	int i_TableIdEx;
	int i_TableIdEx2;
	int i_TableIdEx3;

	int i_dataLen; /*���˱Ƚ����ݳ���:0~16*/
	unsigned char data[PSI_FILTER_DEPTH]; /*�Ƚ�����*/
	unsigned char dataMask[PSI_FILTER_DEPTH]; /*��Ҫ�Ƚϵ�λ*/
	
	void (*fn_UpdateCallback)(int Device, DTV_PSI_CAPTURE_HANDLE Handle, TableContainer *pstru_Table);
	void (*fn_SectionCallback)(int Device, DTV_PSI_CAPTURE_HANDLE Handle, SectionContainer *pstru_Section);
	~CaptureParams()
	{
	}
};

/*���ݼ�ز���*/
class PSIFilterParams
{
public:
	int i_Pid;
	int i_dataLen; /*���˱Ƚ����ݳ���:0~16*/
	unsigned char data[PSI_FILTER_DEPTH]; /*�Ƚ�����*/
	unsigned char dataMask[PSI_FILTER_DEPTH]; /*��Ҫ�Ƚϵ�λ*/
//	unsigned char  patern[PSI_FILTER_DEPTH];			/*����ģʽʹ�õ�λ*/
	unsigned char negData[PSI_FILTER_DEPTH];
	unsigned char negMask[PSI_FILTER_DEPTH];
	void (*fn_FilterCallback)(int Device, DTV_PSI_FILTER_HANDLE Handle, SectionContainer *pstru_Section);
	~PSIFilterParams()
	{
	}
};

/*PMT���������ض���*/

/*CA������*/
class PmtCaDes
{
public:
	int i_CaSystemID;
	int i_CaPid;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~PmtCaDes();
};

/*��Ԫ��������*/
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

/*PMT�������*/
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
/*Tuner����״̬*/
typedef enum
{
	DTV_TUNER_CARRIER_LOCK,
	DTV_TUNER_CARRIER_UNLOCK,
	DTV_TUNER_TURNING
} DTV_TUNER_LOCK_STATUS_e;

/*QAM����ģʽ���ࣨ-Cʹ�ã�*/
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
	DTV_TUNER_MOD_INVALID              /**<Invalid Modulation mode*/       /**<CNcomment:�Ƿ��ĵ�������ö��ֵ*/
}DTV_TUNER_MOD_TYPE_E;

typedef enum
{
    DTV_TUNER_TER_MODE_BASE = 0,     /**< the channel is base mode*/         /**<CNcomment:ͨ���н�֧��base�ź�*/
    DTV_TUNER_TER_MODE_LITE,        	 /**< the channel is lite mode*/         /**<CNcomment:ͨ������Ҫ֧��lite�ź�*/
    DTV_TUNER_TER_MODE_INVALID      	/**<Invalid value*/            /**<CNcomment:�Ƿ��߽�ֵ*/
} DTV_TUNER_TER_MODE_E;

typedef enum
{
    DTV_TUNER_TS_PRIORITY_NONE = 0,      /**<no priority mode*/          /**<CNcomment:�����ȼ�ģʽ*/
    DTV_TUNER_TS_PRIORITY_HP,            	/**<high priority mode*/        /**<CNcomment:�����ȼ�ģʽ*/
    DTV_TUNER_TS_PRIORITY_LP,           	 /**<low priority mode*/         /**<CNcomment:�����ȼ�ģʽ*/
    DTV_TUNER_TS_PRIORITY_INVALID          	 /**<Invalid value*/             /**<CNcomment:�Ƿ��߽�ֵ*/
} DTV_TUNER_TS_PRIORITY_E;

/*Ƶ��������ݽṹ��-Cʹ�ã�*/
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

/*Ƶ��������ݽṹ��-Cʹ�ã�*/
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

/*Ƶ��������ݽṹ��-Sʹ�ã�*/
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

/*Ƶ��������ݽṹ��-Tʹ�ã�*/
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

/*  Ƶ���������ݽṹ*/
class CarrierStatus
{
public:
	int i_SignLevel;			//0~100
	int i_SignQuality;		//0~100
	int i_BitErrorRate;		//��λΪ10��-9�η�
	int i_SNR;
	DTV_TUNER_LOCK_STATUS_e enm_LockStatus;
	DTV_TUNER_SPECTRUM_e enm_Spectrum;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~CarrierStatus();
};

/*NVOD�ο�ҵ�������*/
#define  DTV_NVOD_MAX_NAME_LENGTH		(32)

/*NVOD���¼������*/
#define  DTV_NVOD_MAX_SHORTTEXT_LENGTH (256) 

/*ʱ���¼��ṹ*/
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

/*�ο��¼��ṹ*/
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

/*�ο�ҵ��ṹ*/
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

/************************�������ݶ���*****************************************/

/*����״̬*/
typedef enum
{
	DTV_STATUS_NIT_BEGIN = 0, /*��ʼ����NIT��*/
	DTV_STATUS_NIT_DONE, /*NIT���������*/
	DTV_STATUS_TUNERSET_DONE, /*����Ƶ�����(δ��ʼ��Ŀ��������)*/
	DTV_STATUS_ONE_SERVICE, /*��ǰƵ���Ŀ*/
	DTV_STATUS_SERVICE_DONE, /*��ǰƵ���Ŀ�������*/
	DTV_STATUS_SCAN_BEGIN, /*��Ŀ������ʼ*/
	DTV_STATUS_SCAN_DONE, /*���н�Ŀ�������*/
	DTV_STATUS_SCAN_FAIL, /*��Ŀ����ʧ��*/
	DTV_STATUS_TUNER_FAIL, /*��Ϊ�ź���������ʧ��*/
	DTV_STATUS_USER_STOP, /*APPȡ������*/
	DTV_STATUS_DB_SAVE_DONE,
	DTV_STATUS_DB_SAVE_FAIL,
	DTV_STATUS_NUM
} DTV_SCAN_STATUS_e;

/*�������Ĳ�������ֵ*/
class ScanStatusInfo
{
public:
	
	DTV_SCAN_STATUS_e enm_Status; /*��ǰ��Ŀ����״̬*/
	int us_CarrCount; /*Ƶ������*/
	int us_CurCarrID; /*Ƶ��������*/
	int us_CurCarrChCount; /*��ǰƵ���еĽ�Ŀ����*/
	int us_ChannelCount; /*�ܵĽ�Ŀ����*/
	int us_TVCount; /*�ܵĵ��ӽ�Ŀ����*/
	int us_RadioCount; /*�ܵĹ㲥��Ŀ����*/
	int us_BatCount;	/*�ܵ�BAT����*/
	bool b_TunerLockStatus; /*TUNER����״̬*/
	DTVChannelInfo ChannelInfo; /*��Ŀ��Ϣ��ֻ����������Ϣ(��״̬��ϢΪDTV_STATUS_ONE_SERVICEʱ���ݸö���)*/
	DVBCCarrierInfo CurCarrierInfo; /**��ǰƵ����Ϣ**/
	DVBCCarrierInfo *pAllCarrierInfo; /*�ܵ�Ƶ���б�*/
	BouquetInfo    CurBouquetInfo;
	void writeToParcel(Parcel* outData);
	void readFromParcel(const Parcel* inData);
	~ScanStatusInfo();
	ScanStatusInfo();
};

class DTVScanParam
{
public:
	DTV_RESULT_e (*OSDCallback)(ScanStatusInfo *rpstru_ScanStatus); /*�ص�֪ͨ*/
	int us_MaxMainCarrNum; /*���� ��Ƶ�����*/
	DTVCarrierParamsBase *pstru_MainCarrList; /*���� ��Ƶ���б�*/
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

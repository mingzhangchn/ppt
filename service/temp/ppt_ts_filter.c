
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_unf_demux.h"

#include "ppt_ts_filter.h"

#define DEMUX_ID        0
#define TS_PORT         HI_UNF_DMX_PORT_TSI_0
#define MAX_CHANNEL_COUNT 64
#define PAT_PID         0


typedef struct{
    HI_HANDLE   RecHandle;
    HI_CHAR     FileName[256];
    HI_BOOL     ThreadRunFlag;
    pthread_t   Thread_id;
} TsFileInfo;

static void SaveRecDataLoop(TsFileInfo *arg);
static int DmxRecordInit();
static void*ts_start(void *args);
static TsFileInfo   TsRecInfo;

int ppt_ts_start()
{
    sprintf(TsRecInfo.FileName, "%s.ts", "/var/rec");
    TsRecInfo.ThreadRunFlag = HI_TRUE;
    pthread_create(&TsRecInfo.Thread_id, NULL, ts_start, (void*)&TsRecInfo);
    return 0;
}

int ppt_ts_stop()
{
    TsRecInfo.ThreadRunFlag = HI_FALSE;
    pthread_join(TsRecInfo.Thread_id, NULL);
    
    return 0;
}

static void*ts_start(void *args)
{
    HI_S32                  ret;
    HI_UNF_DMX_REC_ATTR_S   RecAttr;
    HI_HANDLE               RecHandle;
    HI_HANDLE               ChanHandle[MAX_CHANNEL_COUNT];
    HI_U32                  ChanCount       = 0;
    HI_BOOL                 RecordStatus    = HI_FALSE;
    TsFileInfo              *pTsRecInfo = (TsFileInfo*)args;
    HI_U32                  i;

    DmxRecordInit();
    
    RecAttr.u32DmxId        = DEMUX_ID;
    RecAttr.u32RecBufSize   = 4 * 1024 * 1024;
    RecAttr.enRecType       = HI_UNF_DMX_REC_TYPE_SELECT_PID;
    RecAttr.bDescramed      = HI_TRUE;
    RecAttr.enIndexType     = HI_UNF_DMX_REC_INDEX_TYPE_NONE;
    ret = HI_UNF_DMX_CreateRecChn(&RecAttr, &RecHandle);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_CreateRecChn failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        return NULL;
    }
    
    ret = HI_UNF_DMX_AddRecPid(RecHandle, PAT_PID, &ChanHandle[ChanCount]);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        goto exit;
    }
    ++ChanCount;

    ret = HI_UNF_DMX_AddRecPid(RecHandle, 51, &ChanHandle[ChanCount]);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        goto exit;
    }
    ++ChanCount;
    
    ret = HI_UNF_DMX_AddRecPid(RecHandle, 52, &ChanHandle[ChanCount]);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        goto exit;
    }
    ++ChanCount;

    ret = HI_UNF_DMX_AddRecPid(RecHandle, 53, &ChanHandle[ChanCount]);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        goto exit;
    }
    ++ChanCount;

    ret = HI_UNF_DMX_AddRecPid(RecHandle, 54, &ChanHandle[ChanCount]);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        goto exit;
    }
    ++ChanCount;   

    ret = HI_UNF_DMX_AddRecPid(RecHandle, 205, &ChanHandle[ChanCount]);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        goto exit;
    }
    ++ChanCount; 

    ret = HI_UNF_DMX_AddRecPid(RecHandle, 206, &ChanHandle[ChanCount]);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        goto exit;
    }
    ++ChanCount;     

    
    ret = HI_UNF_DMX_StartRecChn(RecHandle);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_StartRecChn failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        goto exit;
    }

    RecordStatus = HI_TRUE;

    pTsRecInfo->RecHandle = RecHandle;

    SaveRecDataLoop(pTsRecInfo);

exit :
    if (RecordStatus){
        ret = HI_UNF_DMX_StopRecChn(RecHandle);
        if (HI_SUCCESS != ret){
            printf("[%s - %u] HI_UNF_DMX_StopRecChn failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        }
    }

    for (i = 0; i < ChanCount; i++){
        ret = HI_UNF_DMX_DelRecPid(RecHandle, ChanHandle[i]);
        if (HI_SUCCESS != ret){
            printf("[%s - %u] HI_UNF_DMX_DelRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        }
    }

    ret = HI_UNF_DMX_DestroyRecChn(RecHandle);
    if (HI_SUCCESS != ret){
        printf("[%s - %u] HI_UNF_DMX_DestroyRecChn failed 0x%x\n", __FUNCTION__, __LINE__, ret);
    }

    return NULL;
}

static void SaveRecDataLoop(TsFileInfo *Ts)
{
    HI_S32      ret;
    FILE       *RecFile = HI_NULL;

    RecFile = fopen(Ts->FileName, "w");
    if (!RecFile){
        perror("fopen error");

        Ts->ThreadRunFlag = HI_FALSE;

        return ;
    }

    printf("[%s] open file %s\n", __FUNCTION__, Ts->FileName);

    while (Ts->ThreadRunFlag){
        HI_UNF_DMX_REC_DATA_S RecData;

        ret = HI_UNF_DMX_AcquireRecData(Ts->RecHandle, &RecData, 100);

        if (HI_SUCCESS != ret){
            if (HI_ERR_DMX_TIMEOUT == ret){
                continue;
            }

            if (HI_ERR_DMX_NOAVAILABLE_DATA == ret){
                continue;
            }

            printf("[%s] HI_UNF_DMX_AcquireRecData failed 0x%x\n", __FUNCTION__, ret);

            break;
        }
        printf("get data len[%d]\n", RecData.u32Len);
        if (RecData.u32Len != fwrite(RecData.pDataAddr, 1, RecData.u32Len, RecFile)){
            perror("[SaveRecDataThread] fwrite error");

            break;
        }
        ret = HI_UNF_DMX_ReleaseRecData(Ts->RecHandle, &RecData);

        if (HI_SUCCESS != ret){
            printf("[%s] HI_UNF_DMX_ReleaseRecData failed 0x%x\n", __FUNCTION__, ret);

            break;
        }

    }

    fclose(RecFile);

    Ts->ThreadRunFlag = HI_FALSE;

    return;
}


static int DmxRecordInit()
{
    HI_S32 ret = HI_FAILURE;
	HI_UNF_DMX_PORT_ATTR_S portAttr;

	printf("%s starting ...\n", __FUNCTION__);

	ret = HI_UNF_DMX_Init();
	if(HI_SUCCESS != ret)
	{
		printf("[%s][%d] err ret=0x%x\n",__FUNCTION__,__LINE__,ret);
		return -1;
	}

	ret = HI_UNF_DMX_DetachTSPort(0);
	if(HI_SUCCESS != ret)
	{
		printf("[%s][%d] err ret=0x%x\n",__FUNCTION__,__LINE__,ret);
		return -1;
	}

	ret = HI_UNF_DMX_GetTSPortAttr(HI_UNF_DMX_PORT_TSI_0, &portAttr);
	if(HI_SUCCESS != ret)
	{
		printf("[%s][%d] err ret=0x%x\n",__FUNCTION__,__LINE__,ret);
		return -1;
	}

	portAttr.enPortType = HI_UNF_DMX_PORT_TYPE_SERIAL_NOSYNC;
#if defined(HI_BOARD_HI3716DVlDMA_HI3716DV100)
	portAttr.u32SerialBitSelector = 1;
#elif defined(HI_BOARD_HI3796MDMO1A_HI3796MV100)
	portAttr.u32SerialBitSelector = 0;
#else
#endif
	ret = HI_UNF_DMX_SetTSPortAttr(HI_UNF_DMX_PORT_TSI_0, &portAttr);
	if(HI_SUCCESS != ret)
	{
		printf("[%s][%d] err ret=0x%x\n",__FUNCTION__,__LINE__,ret);
		return -1;
	}

	ret = HI_UNF_DMX_AttachTSPort(DEMUX_ID, HI_UNF_DMX_PORT_TSI_0);
	if(HI_SUCCESS != ret)
	{
		printf("[%s][%d] err ret=0x%x\n",__FUNCTION__,__LINE__,ret);
		return -1;
	}
    
    return -1;
}

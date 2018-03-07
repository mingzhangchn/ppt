#include "PPTTsFilterMgr.h"
#include <unistd.h>
#include <iostream>
#include <fstream> 
#include "hi_type.h"
#include "hi_unf_demux.h"

sp<PPTTsFilterMgr> PPTTsFilterMgr::sTsFilterMgr = NULL;
Mutex PPTTsFilterMgr::sTsFilterMutex;

using namespace std;
using namespace android;

sp<PPTTsFilterMgr> PPTTsFilterMgr::self()
{
    Mutex::Autolock _l(sTsFilterMutex);
    if (sTsFilterMgr != NULL) {
        return sTsFilterMgr;
    }
    sTsFilterMgr = new PPTTsFilterMgr();
    return sTsFilterMgr;
}

PPTTsFilterMgr::PPTTsFilterMgr()
:mChnMgr(PPTChannelManager::self())
,mDemuxId(0)
,mStatus(0)
{
    initFilter();
}

PPTTsFilterMgr::~PPTTsFilterMgr()
{
    if (mStatus){
        stopFilter();
    }
}

int PPTTsFilterMgr::startFilter()
{
    Mutex::Autolock _l(sTsFilterMutex);
    
    if (mStatus){
        cout<<"Filter already start!"<<endl;
        return 0;
    }
    
    HI_UNF_DMX_REC_ATTR_S   RecAttr;
    RecAttr.u32DmxId        = mDemuxId;
    RecAttr.u32RecBufSize   = 4 * 1024 * 1024;
    RecAttr.enRecType       = HI_UNF_DMX_REC_TYPE_SELECT_PID;
    RecAttr.bDescramed      = HI_TRUE;
    RecAttr.enIndexType     = HI_UNF_DMX_REC_INDEX_TYPE_NONE;
    if(HI_UNF_DMX_CreateRecChn(&RecAttr, &mRecHandle) != HI_SUCCESS){
        cout << "HI_UNF_DMX_CreateRecChn"<<endl; 
        return -1;
    }

    HI_HANDLE  ChanHandle;
    for (vector<ChannelInfo>::iterator it = mChnMgr->mChannelList.begin(); 
    it != mChnMgr->mChannelList.end(); 
    ++it){
        if ((*it).vPid > 0 && (*it).vPid < 0x1fff){
            if (HI_UNF_DMX_AddRecPid(mRecHandle, (*it).vPid, &ChanHandle) == HI_SUCCESS){
                mChnHandles.push_back(ChanHandle);
            }
        }
        
        if ((*it).aPid > 0 && (*it).aPid < 0x1fff){
            if (HI_UNF_DMX_AddRecPid(mRecHandle, (*it).aPid, &ChanHandle) == HI_SUCCESS){
                mChnHandles.push_back(ChanHandle);
            }
        }        
    }  
    
    if (HI_UNF_DMX_StartRecChn(mRecHandle)){
        printf("[%s - %u] HI_UNF_DMX_StartRecChn failed\n", __FUNCTION__, __LINE__);
        return -1;
    }  
    
    mStatus = 1;
    
    return 0;
}

int PPTTsFilterMgr::stopFilter()
{
    Mutex::Autolock _l(sTsFilterMutex);
    
    if (mStatus == 0){
        cout<<"filter not start yet!"<<endl;
        return 0;
    }
    
    if (HI_UNF_DMX_StopRecChn(mRecHandle) == HI_SUCCESS){
        for(vector<HI_HANDLE>::iterator it = mChnHandles.begin(); 
        it != mChnHandles.end();
        ++it){
            HI_UNF_DMX_DelRecPid(mRecHandle, *it);
        }
        
        HI_UNF_DMX_DestroyRecChn(mRecHandle);
        
        mStatus = 0;
        
        return 0;
    }    
    
    cout<<"stopFilter error!"<<endl;
    
    return -1;
}

int PPTTsFilterMgr::initFilter()
{
    HI_S32 ret = HI_FAILURE;
	
	ret = HI_UNF_DMX_Init();
	if(HI_SUCCESS != ret){
		printf("[%s][%d] err ret=0x%x\n",__FUNCTION__,__LINE__,ret);
		return -1;
	}

	ret = HI_UNF_DMX_DetachTSPort(0);
	if(HI_SUCCESS != ret){
		printf("[%s][%d] err ret=0x%x\n",__FUNCTION__,__LINE__,ret);
		return -1;
	}

    HI_UNF_DMX_PORT_ATTR_S portAttr;
	ret = HI_UNF_DMX_GetTSPortAttr(HI_UNF_DMX_PORT_TSI_0, &portAttr);
	if(HI_SUCCESS != ret){
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
	if(HI_SUCCESS != ret){
		printf("[%s][%d] err ret=0x%x\n",__FUNCTION__,__LINE__,ret);
		return -1;
	}

	ret = HI_UNF_DMX_AttachTSPort(mDemuxId, HI_UNF_DMX_PORT_TSI_0);
	if(HI_SUCCESS != ret){
		printf("[%s][%d] err ret=0x%x\n",__FUNCTION__,__LINE__,ret);
		return -1;
	}
    
    return 0;  
}


int PPTTsFilterMgr::getTsFromFilter(char *pBuf, unsigned int *bufLen)
{
#if 1
    if (mStatus){
        HI_UNF_DMX_REC_DATA_S RecData;
        HI_S32 ret = HI_UNF_DMX_AcquireRecData(mRecHandle, &RecData, 100); 
        if (HI_SUCCESS == ret){
            if (*bufLen >= RecData.u32Len){
                memcpy(pBuf, RecData.pDataAddr, RecData.u32Len);
                *bufLen = RecData.u32Len;
                //cout << RecData.u32Len<<endl;
            }
            else{
                cout<<"Buf is too short!"<<endl;
                *bufLen = 0;
            }
            HI_UNF_DMX_ReleaseRecData(mRecHandle, &RecData);
            
            return 0;
        }
    }
    
    return -1;
    
#else
        
    static ifstream in;
    if (!in.is_open())
    {
       char  testTs[] = "/mnt/test.ts";
       cout<<"use test ts:"<<testTs <<endl;
       in.open(testTs,  ios::in); 
    }
    
    if (in.is_open()){
         if (!in.eof()){
            in.read(pBuf, 48128); 
            usleep(120000);
         }
         else{
             in.seekg(ios::beg);
         }  
    }  
    
    return 0;
    
#endif
}



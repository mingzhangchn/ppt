
#include "PPTDDTSectionManager.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


PPTDDTSectionManager::PPTDDTSectionManager()
:mSectionLen(0)
,mPSections(NULL)
,mSectionCount(0)
,mFileLen(0)
{
    //
}

PPTDDTSectionManager::~PPTDDTSectionManager()
{
    if (mPSections){
        free(mPSections);
        mPSections = NULL;
    }
    
    for (std::map<int,DataItem>::iterator it=mSectionMap.begin(); it!=mSectionMap.end(); ++it){
        free(it->second.ptr);
    }
    
    mSectionMap.clear();
}

int PPTDDTSectionManager::addSection(const char* pData, int len)
{
    int secCount = (int)pData[5]<<8 | pData[5];
    std::map<int, DataItem>::iterator it=mSectionMap.begin();
    for (; it!=mSectionMap.end(); ++it){
        if(it->first == secCount){
            break;
        }
    }  
    
    if (it == mSectionMap.end()){
        DataItem item;
        item.len = len;
        item.ptr = (char*)malloc(len);
        memcpy(item.ptr, pData  + 8, len - 8 - 4);
        mSectionMap.insert ( std::pair<int,DataItem>(secCount, item));
        mSectionLen += len - 8 - 4;
    }
    

    if (mSectionMap.size() != (unsigned int)(mSectionCount)){//full
        return 0;      
    }
     
    mPSections = (char*)malloc(mSectionLen);
    int copyLen = 0;
    for (std::map<int, DataItem>::iterator it=mSectionMap.begin(); it != mSectionMap.end(); ++it){
        memcpy(mPSections + copyLen, it->second.ptr, it->second.len);
        copyLen += it->second.len;
    }    
        
    return 1;
}

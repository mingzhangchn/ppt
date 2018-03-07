
#include "PPTNctSectionManager.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

PPTNctSectionManager::PPTNctSectionManager()
:mSectionLen(0)
,mPSections(0)
{
    //
}

PPTNctSectionManager::~PPTNctSectionManager()
{
    if (mPSections){
        free(mPSections);
        mPSections = NULL;
    }
    
    for (std::map<int, DataItem>::iterator it=mSectionMap.begin(); it!=mSectionMap.end(); ++it){
        free(it->second.ptr);
    }
    
    mSectionMap.clear();
}

int PPTNctSectionManager::addSection(const char* pData, int len)
{
    int sectionNum = pData[6];
    std::map<int,DataItem>::iterator it=mSectionMap.begin();
    for (; it!=mSectionMap.end(); ++it){
        if(it->first == sectionNum){
            break;
        }
    }  
    
    if (it == mSectionMap.end()){
        DataItem item;
        item.len = len;
        item.ptr = (char*)malloc(len);
        memcpy(item.ptr, pData  + 8, len - 8 - 4);
        mSectionMap.insert ( std::pair<int,DataItem>(sectionNum, item));
        mSectionLen += len - 8 - 4;
    }
    
    unsigned int lastSection = pData[7];
    if (mSectionMap.size() != (lastSection + 1)){//full
        return 0;      
    }
     
    mPSections = (char*)malloc(mSectionLen);
    int copyLen = 0;
    for (std::map<int,DataItem>::iterator it=mSectionMap.begin(); it != mSectionMap.end(); ++it){
        memcpy(mPSections + copyLen, it->second.ptr, it->second.len);
        copyLen += it->second.len;
    }    
        
    return 1;
}

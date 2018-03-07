
#include "PPTNctParser.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

PPTNctParser::PPTNctParser()
:mDdtPid(0)
,mDdtTableId(0)
,mFileLen(0)
,mFileBlockCount(0)
,mFlag(0)
{
    //
}

PPTNctParser::~PPTNctParser()
{
    //
}

int PPTNctParser::parse(PPTNctSectionManager *pSecMgr)
{
    char *ptr = pSecMgr->mPSections;
    int len = pSecMgr->mSectionLen;
    
    ptr += 2;
    len = len -2;
    
    while (len > 0){
        if (ptr[0] == 0xc1){
            if (strncmp(&ptr[2], "npi", 3) == 0){
                mDdtPid = (int)(ptr[20]) << 8 | ptr[21];
                mDdtTableId = ptr[22];
                mFileLen = (int)(ptr[11])<<24 | (int)(ptr[12])<<16 | (int)(ptr[13])<<8 | ptr[14];
                mFileBlockCount = (int)(ptr[15])<<8 | ptr[16];
                mFlag = 1;
                break;
            }            
        }
        ptr = ptr + (ptr[1] + 2);//next descriptor
        len = len - (ptr[1] + 2);
    }
    return 0;
}

#include "PPTDDTParser.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

PPTDDTParser::PPTDDTParser()
:mDdtPid(0)
,mDdtTableId(0)
,mFileLen(0)
,mFileBlockCount(0)
,mFlag(0)
{
    //
}

PPTDDTParser::~PPTDDTParser()
{
    //
}

int PPTDDTParser::parse(PPTDDTSectionManager *pMgr)
{
    char *ptr = pMgr->mPSections;
    int len =  pMgr->mSectionLen;
    FILE *fp = fopen("/mnt/npi.tar.gz", "w+");
    if (fp){
        fwrite(ptr, 1, len, fp);
        fclose(fp);
        system("tar -zxf /mnt/npi.tar.gz -C /mnt/");
    }
    
    return 0;
}
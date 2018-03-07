#ifndef PPTDDTParser_H
#define PPTDDTParser_H

#include "PPTDDTSectionManager.h"

class PPTDDTParser {
public:
    PPTDDTParser();
    virtual ~PPTDDTParser();
    
    int parse(PPTDDTSectionManager *pSecMgr);
    int getFlag(){return mFlag;}
private:
    int mDdtPid;
    int mDdtTableId;
    int mFileLen;
    int mFileBlockCount;
    int mFlag;
};

#endif

#ifndef PPTNctParser_H
#define PPTNctParser_H

#include "PPTNctSectionManager.h"

class PPTNctParser {
public:
    PPTNctParser();
    virtual ~PPTNctParser();
    
    int parse(PPTNctSectionManager *pSecMgr);
    int getFlag(){return mFlag;}
    int getDdtPid(){return mDdtPid;}
    int getDdtTableId(){return mDdtTableId;}
    int getFileLen(){return mFileLen;}
    int getFileBlockCount(){return mFileBlockCount;}
private:
    int mDdtPid;
    int mDdtTableId;
    int mFileLen;
    int mFileBlockCount;
    int mFlag;
};

#endif

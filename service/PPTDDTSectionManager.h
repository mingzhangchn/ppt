#ifndef PPTDDTSectionManager_H
#define PPTDDTSectionManager_H

#include <map>

class PPTDDTParser;
class PPTDDTSectionManager{
public:
    PPTDDTSectionManager();
    virtual ~PPTDDTSectionManager();
    
    int addSection(const char* pData, int len);
    void setSectionCount(int count){mSectionCount = count;}
    void setFileLen(int len){mFileLen = len;}

private:
    friend class PPTDDTParser;
    int mSectionLen;
    char *mPSections;
    int mSectionCount;
    int mFileLen;
    typedef struct{
        int len;
        char *ptr;
    }DataItem;
    std::map<int, DataItem> mSectionMap;
};

#endif
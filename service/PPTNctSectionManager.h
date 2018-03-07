#ifndef PPTNctSectionManager_H
#define PPTNctSectionManager_H

#include <map>

class PPTNctParser;
class PPTNctSectionManager{
public:
    PPTNctSectionManager();
    virtual ~PPTNctSectionManager();
    
    int addSection(const char* pData, int len);

private:
    friend class PPTNctParser;
    int mSectionLen;
    char *mPSections;
    typedef struct{
        int len;
        char *ptr;
    }DataItem;
    std::map<int, DataItem> mSectionMap;
};

#endif
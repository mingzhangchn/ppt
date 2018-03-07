
#include "PPTTsSliceMgr.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <android/log.h>
#include <utils/Log.h>
#include "ppt_create_psiTable.h"

#include "jah_util.h"

sp<PPTTsSliceMgr> PPTTsSliceMgr::sTsSliceMgr = NULL;
const int PPTTsSliceMgr::TS_PACKET_LEN = 188;

using namespace std;
using namespace android;

sp<PPTTsSliceMgr> PPTTsSliceMgr::self()
{
    if (sTsSliceMgr != NULL){
        return sTsSliceMgr;
    }
    sTsSliceMgr = new PPTTsSliceMgr();
    return sTsSliceMgr;
}

PPTTsSliceMgr::PPTTsSliceMgr()
:mTsFilterMgr(PPTTsFilterMgr::self())
,mChannelMgr(PPTChannelManager::self())
{
    //
}

PPTTsSliceMgr::~PPTTsSliceMgr()
{
    mSliceFileMgrMap.clear();
}

int PPTTsSliceMgr::generatePlayList()
{
    mSliceFileMgrMap.clear();
    for (vector<ChannelInfo>::iterator it = mChannelMgr->mChannelList.begin(); 
    it != mChannelMgr->mChannelList.end(); 
    ++it){  
        char path[1024] = {0};
        snprintf(path, sizeof(path), "/tmp/www/hls/%d", (*it).serviceId);
        cout << path<<endl;
        sp<SliceFileMgr> fileMgr = new SliceFileMgr(path, (*it).serviceId);
        mSliceFileMgrMap.insert(pair<int, sp<SliceFileMgr> >((*it).serviceId, fileMgr));
    }   
    
    {//x.xml
        xmlDocPtr doc = NULL;
        xmlNodePtr items = NULL;
        xmlNodePtr cate = NULL;
        xmlNodePtr item = NULL;    
        doc = xmlNewDoc((const xmlChar *)"1.0");
        items = xmlNewNode(NULL, (const xmlChar *)"items");
        xmlDocSetRootElement(doc, items);
        cate = xmlNewChild(items, NULL, (const xmlChar *)"cate", (const xmlChar *)" ");
        xmlNewProp(cate, (const xmlChar *)"title", (const xmlChar *)"channels");
        
        for (vector<ChannelInfo>::iterator it = mChannelMgr->mChannelList.begin(); 
        it != mChannelMgr->mChannelList.end(); 
        ++it){   
            char path[1024] = {0};
            snprintf(path, sizeof(path), "hls/%d/ppt.m3u8", (*it).serviceId);
            char serviceStr[8] = {0};
            snprintf(serviceStr, sizeof(serviceStr), "%d", (*it).serviceId);
            item = xmlNewChild(cate, NULL, (const xmlChar *)"item", (const xmlChar *)"");
            xmlNewProp(item, (const xmlChar *)"cover", (const xmlChar *)"");
            xmlNewProp(item, (const xmlChar *)"title", (const xmlChar *)serviceStr);
            xmlNewProp(item, (const xmlChar *)"link", (const xmlChar *)path);
            xmlNewProp(item, (const xmlChar *)"serviceid", (const xmlChar *)serviceStr);
            xmlNewProp(item, (const xmlChar *)"servicename", (const xmlChar *)serviceStr);
        }
        xmlSaveFormatFileEnc("/tmp/www/x.xml", doc, "UTF-8", 1);
    }
    
    {//status.xml
        xmlDocPtr doc = NULL;
        xmlNodePtr vstatus = NULL;
        xmlNodePtr status = NULL;    
        doc = xmlNewDoc((const xmlChar *)"1.0");
        vstatus = xmlNewNode(NULL, (const xmlChar *)"vstatus");
        xmlDocSetRootElement(doc, vstatus);
        xmlNewProp(vstatus, (const xmlChar *)"locked", (const xmlChar *)"1");
        xmlNewProp(vstatus, (const xmlChar *)"kicked", (const xmlChar *)"0");
        xmlNewProp(vstatus, (const xmlChar *)"osd", (const xmlChar *)"0");
        xmlNewProp(vstatus, (const xmlChar *)"search", (const xmlChar *)"0");
        xmlNewProp(vstatus, (const xmlChar *)"npiver", (const xmlChar *)"170727");
        
        for (vector<ChannelInfo>::iterator it = mChannelMgr->mChannelList.begin(); 
        it != mChannelMgr->mChannelList.end(); 
        ++it){   
            char serviceStr[8] = {0};
            snprintf(serviceStr, sizeof(serviceStr), "%d", (*it).serviceId);
            status = xmlNewChild(vstatus, NULL, (const xmlChar *)"status", (const xmlChar *)"");
            xmlNewProp(status, (const xmlChar *)"channel", (const xmlChar *)serviceStr);
            xmlNewProp(status, (const xmlChar *)"msgtype", (const xmlChar *)"0");
            xmlNewProp(status, (const xmlChar *)"seqtype", (const xmlChar *)"1");
            xmlNewProp(status, (const xmlChar *)"mediatype", (const xmlChar *)"0");
        }
        xmlSaveFormatFileEnc("/tmp/www/status.xml", doc, "UTF-8", 1);
    }    
    
    return 0;
}

void PPTTsSliceMgr::getTsData()
{
    cout <<"get ts data"<<endl;
    LOGI("start getTsData\n");
    
    char buf[48128*10];
    while(1){
        unsigned int len = sizeof(buf);
        if (mTsFilterMgr->getTsFromFilter(buf, &len) != 0){
            continue;
        }
        for (char *pbTs = buf; (pbTs <= buf + len - TS_PACKET_LEN); pbTs += TS_PACKET_LEN){
            int CurrentPID  = ((pbTs[1] & 0x1f) << 8) + pbTs[2];
            if ((CurrentPID > 0 ) && (CurrentPID < 0x1fff)){
                //cout<<CurrentPID<<endl;
                int sid = mChannelMgr->getServiceId(CurrentPID);
                if (sid != -1){
                    //mbox_add_ts_to_circleBuf_2(0, sid, pbTs, TS_PACKET_LEN);
                    //cout <<"sid:"<<sid<<endl;
                    map<int,sp<SliceFileMgr> >::iterator it = mSliceFileMgrMap.find(sid);
                    if (it != mSliceFileMgrMap.end()){
                        it->second->addTsPacket(pbTs, TS_PACKET_LEN);
                    }
                    
                }
            }
        }        
    }
}

const int PPTTsSliceMgr::SliceFileMgr::SLICE_MAX_LEN = 188*30000;

PPTTsSliceMgr::SliceFileMgr::SliceFileMgr(string path, int sid)
:mPath(path)
,mServiceId(sid)
,mStartSeq(1)
,mCurSeq(0)
,mPTsBuf(NULL)
,mTsBufLen(0)
,mSliceCount(3)
{
    init();
}

PPTTsSliceMgr::SliceFileMgr::~SliceFileMgr()
{
    if (mPTsBuf){
        free(mPTsBuf);
    }
}

int PPTTsSliceMgr::SliceFileMgr::init()
{
    string cmd = "mkdir -p " + mPath;
    system(cmd.c_str());
    return 0;
}

void PPTTsSliceMgr::SliceFileMgr::addTsPacket(char *buf, int len)
{
    if(!mPTsBuf){
        mPTsBuf = (char*)malloc(SLICE_MAX_LEN);
    }    
    
    if (mPTsBuf){
        if (mTsBufLen + len > SLICE_MAX_LEN){//buf full
            mCurSeq++;
            cout<<"channel:"<<mServiceId<<", slice:"<<mCurSeq<<endl;
            LOGI("channel:%d,slice:%d\n", mServiceId, mCurSeq);
            //write ts slice
            char tsSlicePath[1024] = {0}; 
            snprintf(tsSlicePath, sizeof(tsSlicePath), "%s/%08ld.ts", mPath.c_str(), mCurSeq); 
            ofstream ofile;
            ofile.open(tsSlicePath);
            if(ofile.is_open()){
                ofile.write(mPTsBuf, mTsBufLen);
                ofile.close();
            }
            
            //delete old file
            if (mCurSeq - mStartSeq >= mSliceCount){
                memset(tsSlicePath, 0, sizeof(tsSlicePath));
                snprintf(tsSlicePath, sizeof(tsSlicePath), "%s/%08ld.ts", mPath.c_str(), mStartSeq); 
                remove(tsSlicePath);
                mStartSeq++;
                //cout<<"mStartSeq:"<<mStartSeq<<endl;
            }
            
            //update m3u8
            char m3u8Buf[1024] = {0};
            snprintf(m3u8Buf, sizeof(m3u8Buf), 
            "#EXTM3U\r\n"
            "#EXT-X-VERSION:3\r\n"
    		"#EXT-X-TARGETDURATION:3\r\n"
    		"#EXT-X-MEDIA-SEQUENCE:%08ld\r\n\r\n", mStartSeq);
            
            for (long i = mStartSeq; i <= mCurSeq; ++i){
                snprintf(m3u8Buf + strlen(m3u8Buf), sizeof(m3u8Buf) - strlen(m3u8Buf),"#EXTINF:3,\r\n%08ld.ts\r\n\r\n", i);
            }
            
            char m3u8Path[1024] = {0}; 
            snprintf(m3u8Path, sizeof(m3u8Path), "%s/ppt.m3u8", mPath.c_str()); 
            ofile.open(m3u8Path);
            if(ofile.is_open()){
                ofile.write(m3u8Buf, strlen(m3u8Buf));
                ofile.close();
            }
            
            mTsBufLen = 0;
        }
        
        if (mTsBufLen + len <= SLICE_MAX_LEN){
            if (mTsBufLen == 0){//add pat pmt
                ChannelInfo info;
                PPTTsSliceMgr::self()->mChannelMgr->getChannelInfo(mServiceId, info);
                char psiBuf[188*2] = {0};
                int len = sizeof(psiBuf);
                GeneratePATPMT(info.vPid, info.vType, info.aPid, info.aType, (unsigned char*)psiBuf, len);
                memcpy(mPTsBuf + mTsBufLen, psiBuf, len);
                mTsBufLen += len;
            }
            memcpy(mPTsBuf + mTsBufLen, buf, len);
            mTsBufLen += len;
        } 
    }

}
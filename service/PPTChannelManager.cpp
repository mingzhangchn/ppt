#include "PPTChannelManager.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>
#include <iostream>

sp<PPTChannelManager> PPTChannelManager::sChannelMgr = NULL;
Mutex PPTChannelManager::sChannelMutex;

using namespace std;
using namespace android;

sp<PPTChannelManager> PPTChannelManager::self()
{
    Mutex::Autolock _l(sChannelMutex);
    if (sChannelMgr != NULL) {
        return sChannelMgr;
    }
    sChannelMgr = new PPTChannelManager();
    return sChannelMgr;
}

PPTChannelManager::PPTChannelManager()
{
    //
}

PPTChannelManager::~PPTChannelManager()
{
    //
}

int PPTChannelManager::updateChannelInfo(string npifile)
{
    Mutex::Autolock _l(sChannelMutex);
    xmlDocPtr doc = xmlReadFile(npifile.c_str(), NULL, 256); 
    if (!doc){
        cout<<"xmlReadFile error"<<endl;
        return -1;
    }
    xmlNodePtr cur =  xmlDocGetRootElement(doc);
    if (!cur){
        cout<<"xmlDocGetRootElement error"<<endl;
        xmlFreeDoc(doc);
        return -1;
    }    

    mChannelList.clear();
    
    if ((xmlStrcmp(cur->name, (const xmlChar *)"npi") == 0 )
        && cur->xmlChildrenNode
        &&(xmlStrcmp(cur->xmlChildrenNode->name, (const xmlChar *)"ts") == 0 )){

        xmlNodePtr serviceNode = cur->xmlChildrenNode->xmlChildrenNode;
        while(serviceNode){
            if (!xmlStrcmp(serviceNode->name, (const xmlChar *)"service")){
                ChannelInfo TempChannelInfo;
                memset(&TempChannelInfo, 0, sizeof(TempChannelInfo));
                
                xmlChar* attr_value = xmlGetProp(serviceNode, (const xmlChar *)"id");
                if (attr_value){
                    TempChannelInfo.serviceId = atoi((const char*)attr_value);
                    cout<<"sid:"<<TempChannelInfo.serviceId<<endl;
                    xmlFree(attr_value);
                }
                else{
                    continue;
                }
                
                xmlNodePtr infoNode = serviceNode->xmlChildrenNode;
                while(infoNode){
                    if (!xmlStrcmp(infoNode->name, (const xmlChar *)"video")){
                        xmlNodePtr tmpNode = infoNode->xmlChildrenNode;
                        while(tmpNode){
                            if (!xmlStrcmp(tmpNode->name, (const xmlChar *)"pid")){
                                xmlChar* value = xmlNodeListGetString(doc, tmpNode->xmlChildrenNode, 1);
                                if (value){
                                    TempChannelInfo.vPid = atoi((const char*)value);
                                    cout<<"vpid:"<<TempChannelInfo.vPid<<endl;
                                    xmlFree(value);
                                }
                            }
                            else if(!xmlStrcmp(tmpNode->name, (const xmlChar *)"type")){
                                xmlChar* value = xmlNodeListGetString(doc, tmpNode->xmlChildrenNode, 1); 
                                if (value){
                                    TempChannelInfo.vType = atoi((const char*)value);
                                    cout<<"vType:"<<TempChannelInfo.vType<<endl;
                                    xmlFree(value);  
                                }                               
                            }
                            else if(!xmlStrcmp(tmpNode->name, (const xmlChar *)"ecmpid"))
                            {
                                //
                            }
                            tmpNode = tmpNode->next;
                        }
                    }
                    else if (!xmlStrcmp(infoNode->name, (const xmlChar *)"audio")){
                        xmlNodePtr tmpNode = infoNode->xmlChildrenNode;
                        while(tmpNode){
                            if (!xmlStrcmp(tmpNode->name, (const xmlChar *)"pid")){
                                xmlChar* value = xmlNodeListGetString(doc, tmpNode->xmlChildrenNode, 1); 
                                if (value){
                                    TempChannelInfo.aPid = atoi((const char*)value);
                                    cout<<"apid:"<<TempChannelInfo.aPid<<endl;
                                    xmlFree(value);
                                }
                            }
                            else if(!xmlStrcmp(tmpNode->name, (const xmlChar *)"type")){
                                xmlChar* value = xmlNodeListGetString(doc, tmpNode->xmlChildrenNode, 1); 
                                if (value){
                                    TempChannelInfo.aType = atoi((const char*)value);
                                    cout<<"aType:"<<TempChannelInfo.aType<<endl;
                                    xmlFree(value);   
                                }                                
                            }
                            else if(!xmlStrcmp(tmpNode->name, (const xmlChar *)"ecmpid"))
                            {
                                //
                            }
                            tmpNode = tmpNode->next;
                        }                        
                    }
                    infoNode = infoNode->next;
                }
                if (TempChannelInfo.aPid > 0 && TempChannelInfo.vPid > 0){
                    mChannelList.push_back(TempChannelInfo);
                } 
            } 
            serviceNode = serviceNode->next;
        } 
    } 
    return 0;
}

int PPTChannelManager::getChannelCount()
{
    return mChannelList.size();
}

int PPTChannelManager::getServiceId(int pid)
{
    Mutex::Autolock _l(sChannelMutex);
    int sid = -1;
    for (vector<ChannelInfo>::iterator it = mChannelList.begin(); 
    it != mChannelList.end(); 
    ++it){
        if ((*it).vPid == pid || (*it).aPid == pid){
            sid = (*it).serviceId;
            break;
        }
    }
    
    return sid;
}

int PPTChannelManager::getChannelInfo(int sid, ChannelInfo& info)
{
    Mutex::Autolock _l(sChannelMutex);
    int ret = -1;
    for (vector<ChannelInfo>::iterator it = mChannelList.begin(); 
    it != mChannelList.end(); 
    ++it){
        if (sid == (*it).serviceId){
            info.vPid = (*it).vPid;
            info.vType = (*it).vType;
            info.aPid = (*it).aPid;
            info.aType = (*it).aType;
            info.serviceId = sid;
            ret = 0;
            break;
        }
    }
    
    return ret;    
}

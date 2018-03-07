/*UPNP*/
#include "upnp/upnpapi.h"
#include "upnp/webserver.h"
#include "upnp/miniserver.h"
#include "upnp/httpparser.h"
#include "upnp/sock.h"
#include "upnp/upnp.h"
#include "upnp/ithread.h"
#include "upnp/ixml.h"
#include "upnp/upnptools.h"

#include "ppt_log.h"
#include "ppt_web_service.h"

#define IP_ADD "192.168.3.56"
#define WEB_SERVER_PORT 8080
#define WEB_SERVER_ROOT_PATH "/tmp/www/"
#define DEV_DESC_DOC_NAME   "vgdevicedesc.xml"

static UpnpDevice_Handle UpnpDeviceHandle = -1;

static void RspToClient(http_parser_t *parser, http_message_t *req, SOCKINFO *info);
static int CreateUpnpDevDescXml();
static int VgDeviceCallbackEventHandler(Upnp_EventType EventType, void *Event, void *Cookie);

int upnp_and_web_server_start()
{
    PPT_RET_CHECK(UpnpInit(NULL, WEB_SERVER_PORT));
    
    PPT_RET_CHECK(web_server_init());
    
    SetHTTPGetCallback((MiniServerCallback)RspToClient);
    
    PPT_RET_CHECK(CreateUpnpDevDescXml());
    
    PPT_RET_CHECK(UpnpSetWebServerRootDir(WEB_SERVER_ROOT_PATH));
    
    char DescDocUrl[256] = {0};
    snprintf( DescDocUrl, sizeof(DescDocUrl), "http://%s:%d/%s", IP_ADD, WEB_SERVER_PORT, DEV_DESC_DOC_NAME);
    PPT_RET_CHECK(UpnpRegisterRootDevice(DescDocUrl, VgDeviceCallbackEventHandler,  &UpnpDeviceHandle, &UpnpDeviceHandle));
    
    PPT_RET_CHECK(UpnpSendAdvertisement(UpnpDeviceHandle, 100));
    
    return 0;
}

static void RspToClient(http_parser_t *parser, http_message_t *req, SOCKINFO *info)
{
    PPT_I("req url[%s]\n", req->urlbuf);
    
    web_server_callback(parser, req, info);
    return;
}

#define UPNP_ACTION_STR_BUF_LEN     20
static int UPnPAction(IXML_Document *in, IXML_Document **out, const char **errorString, unsigned int PeerIp)
{
    char loadstr[UPNP_ACTION_STR_BUF_LEN] = {0};
    char Mac_str[UPNP_ACTION_STR_BUF_LEN] = {0};
    char portstr[UPNP_ACTION_STR_BUF_LEN] = {0};
    int load = 1;

    snprintf(Mac_str, sizeof(Mac_str), "00:00:00:00:00:00");
    snprintf(loadstr, sizeof(loadstr), "%d", load);
    snprintf(portstr, sizeof(portstr), "%d", WEB_SERVER_PORT);

    UpnpAddToActionResponse(out, "UPnPAction", "urn:nstv:serviceId:playtv", "ipaddr", IP_ADD);
    UpnpAddToActionResponse(out, "UPnPAction", "urn:nstv:serviceId:playtv", "mac", Mac_str);
    UpnpAddToActionResponse(out, "UPnPAction", "urn:nstv:serviceId:playtv", "port", portstr);
    //UpnpAddToActionResponse(out, "UPnPAction", "urn:nstv:serviceId:playtv", "remainPayload", loadstr);
    //UpnpAddToActionResponse(out, "UPnPAction", "urn:nstv:serviceId:playtv", "encrypt", ENCRYPT_METHOD);
    UpnpAddToActionResponse(out, "UPnPAction", "urn:nstv:serviceId:playtv", "remainPayload", 0);
    UpnpAddToActionResponse(out, "UPnPAction", "urn:nstv:serviceId:playtv", "encrypt", 0);

    V_INFO("discoveried by the client.\n");
     
    return 0;
}

static int VgDeviceHandleActionRequest(struct Upnp_Action_Request *ca_event)
{
    struct sockaddr_in *peer = (struct sockaddr_in *)&ca_event->CtrlPtIPAddr;
    V_INFO("VgDeviceHandleActionRequest\n%s\n %s\n %s\n", ca_event->DevUDN, ca_event->ServiceID, ca_event->ActionName);
    UPnPAction(ca_event->ActionRequest, &ca_event->ActionResult, NULL, peer->sin_addr.s_addr);

    return 0;
}

static int VgDeviceCallbackEventHandler(Upnp_EventType EventType, void *Event, void *Cookie)
{
    switch (EventType) {
    case UPNP_EVENT_SUBSCRIPTION_REQUEST:
        break;
    case UPNP_CONTROL_GET_VAR_REQUEST:
        break;
    case UPNP_CONTROL_ACTION_REQUEST:
        VgDeviceHandleActionRequest((struct Upnp_Action_Request *)Event);
        break;
        /* ignore these cases, since this is not a control point */
    case UPNP_DISCOVERY_ADVERTISEMENT_ALIVE:
    case UPNP_DISCOVERY_SEARCH_RESULT:
    case UPNP_DISCOVERY_SEARCH_TIMEOUT:
    case UPNP_DISCOVERY_ADVERTISEMENT_BYEBYE:
    case UPNP_CONTROL_ACTION_COMPLETE:
    case UPNP_CONTROL_GET_VAR_COMPLETE:
    case UPNP_EVENT_RECEIVED:
    case UPNP_EVENT_RENEWAL_COMPLETE:
    case UPNP_EVENT_SUBSCRIBE_COMPLETE:
    case UPNP_EVENT_UNSUBSCRIBE_COMPLETE:
        break;
    default:
        V_INFO("Error in VgDeviceCallbackEventHandler: unknown event type %d\n", EventType);
    }

    return 0;    
}

char gUpnpSdkNLSuuid[] = "1111111111111111111111111";
#define UPNP_DEV_DESC_XML_BUF_LEN   128
static int CreateUpnpDevDescXml()
{
    system("mkdir -p /tmp/www");
    char filename[] = "/tmp/www/vgdevicedesc.xml";
    FILE *fp;
    char buf[UPNP_DEV_DESC_XML_BUF_LEN]={0};
  
    char uuid_pre[] = "uuid:NBP_VG-";
    strncpy(buf, uuid_pre, sizeof(buf));
    strncat(buf, gUpnpSdkNLSuuid, sizeof(buf)-strlen(uuid_pre));
         
    fp = fopen(filename, "w");
    if(fp == NULL){
        PPT_E("open file error\n");
        return -1;
    }
    fputs("<?xml version=\"1.0\"?>\n", fp);
    fputs("<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n", fp);
    fputs("\t<specVersion>\n", fp);
    fputs("\t\t<major>1</major>\n", fp);
    fputs("\t\t<minor>0</minor>\n", fp);
    fputs("\t</specVersion>\n", fp);
    fputs("\t<device>\n", fp);
    fputs("\t\t<deviceType>urn:nstv:device:nbpvg:1306.1316</deviceType>\n", fp);
    fputs("\t\t<friendlyName>NBP Video Gateway</friendlyName>\n", fp);
    fputs("\t\t<manufacturer>novel-supertv</manufacturer>\n", fp);
    fputs("\t\t<manufacturerURL>http://www.novel-supertv.com</manufacturerURL>\n", fp);
    fputs("\t\t<modelDescription>NBP Video Gateway 1306.1316</modelDescription>\n", fp);
    fputs("\t\t<modelName>NBP_VG</modelName>\n", fp);
    fputs("\t\t<modelNumber>1.0.1306.1316</modelNumber>\n", fp);
    fputs("\t\t<modelURL>http://www.novel-supertv.com/</modelURL>\n", fp);
    fputs("\t\t<serialNumber>123456789001</serialNumber>\n", fp);
    fprintf(fp, "\t\t<UDN>%s</UDN>\n", buf);
    fputs("\t\t<UPC>123456789</UPC>\n", fp);
    fputs("\t\t<serviceList>\n", fp);
    fputs("\t\t\t<service>\n", fp);
    fputs("\t\t\t\t<serviceType>urn:nstv:service:play:1306.1316</serviceType>\n", fp);
    fputs("\t\t\t\t<serviceId>urn:nstv:serviceId:playtv</serviceId>\n", fp);
    fputs("\t\t\t\t<controlURL>CONTROLURL</controlURL>\n", fp);
    fputs("\t\t\t\t<eventSubURL>EVENTURL</eventSubURL>\n", fp);
    fputs("\t\t\t\t<SCPDURL>/vgcontrolscpd.xml</SCPDURL>\n", fp);
    fputs("\t\t\t</service>\n", fp);
    fputs("\t\t</serviceList>\n", fp);
    fputs("\t\t<presentationURL>/vgdevicepres.html</presentationURL>\n", fp);
    fputs("\t</device>\n", fp);
    fputs("</root>\n", fp);

    fclose(fp);

    return 0;
}

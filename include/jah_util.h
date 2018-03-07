#ifndef __JAH_UTIL_H__
#define __JAH_UTIL_H__

#include <stdio.h>

#ifdef __cplusplus
#include <android/log.h>
#include <utils/Logger.h>
extern "C"
{
#endif


#define LOG_DEBUG

#undef LOGE
#undef LOGD
#undef LOGE
#define LOGE HI_LOGE
#define LOGD HI_LOGD
#define LOGI HI_LOGE

//#define LOGE 		//ALOGE
//#define LOGD		//ALOGD
//#define LOGE		//ALOGE
#undef LOG_INFO
#undef LOG_ERROR
#undef LOG_ASSERT


#ifdef LOG_DEBUG
#define LOG_INFO(...)      do{LOGE(__VA_ARGS__);printf(__VA_ARGS__);}while(0)
#define LOG_ERROR(...)     do{LOGE(__VA_ARGS__);printf(__VA_ARGS__);}while(0)
#define LOG_ASSERT(value)  do{if(!(value))LOG_INFO("Assertion failed: \"%s\", file %s, line %d\n", #value,__FILE__,__LINE__);}while(0)
#define LOG_ARRAY(a,len)   do{int i;for(i=0;i<len;i++)LOG_INFO(" 0x%02x",(unsigned int)((a)[i]));}while(0)
#else
#define LOG_INFO(...)      ((void)0)
#define LOG_ERROR(...)     ((void)0)
#define LOG_ASSERT(value)  ((void)0)
#define LOG_ARRAY(a,len)   ((void)0)
#endif


void *jah_malloc(size_t size);
void jah_free(void *ptr);
unsigned long jah_crc32(void *pdata, int size);

#ifdef __cplusplus
}
#endif

#endif




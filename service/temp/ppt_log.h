#ifndef ppt_log_H_
#define ppt_log_H_

#define PPT_RET_CHECK(x) do{if(x !=0) printf("Line [%d] retcheck error\n", __LINE__);}while(0)
#define PPT_D printf
#define PPT_E printf
#define PPT_I printf


#define V_INFO printf

#endif
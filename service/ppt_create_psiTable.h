#ifndef __CREATE_PSI_TABLE_H__
#define __CREATE_PSI_TABLE_H__


#ifdef __cplusplus
extern "C" {
#endif

#define TS_HEAD_LEN (5)
#define TABLE_LEN   (188)

#define PAT_PRONUM  (TS_HEAD_LEN + 8) 	//13
#define PMTPID      (TS_HEAD_LEN + 14)	//19
#define PAT_CRC     (TS_HEAD_LEN + 16)	//21
#define PMT_PRONUM  (TS_HEAD_LEN + 3)	//8
#define PMT_CRC     (TS_HEAD_LEN + 26)	//31

#define V_TYPE	    (TS_HEAD_LEN + 16)	//21
#define ES_PID_V    (TS_HEAD_LEN + 17)	//22
#define A_TYPE	    (TS_HEAD_LEN + 21)	//26
#define ES_PID_A    (TS_HEAD_LEN + 22)	//27

extern unsigned char PAT_DATA_TABLE[TABLE_LEN];
extern unsigned char PMT_DATA_TABLE[TABLE_LEN];

/*GeneratePMT*/
extern void GeneratePMT(unsigned short Vpid, unsigned char Vtype, unsigned short Apid,unsigned char Atype);
extern void GeneratePMT1(unsigned short Vpid, unsigned char Vtype, unsigned short Apid,unsigned char Atype,unsigned char uCount);
extern int GeneratePATPMT(unsigned short Vpid, unsigned char Vtype, unsigned short Apid,unsigned char Atype, unsigned char *buf, int bufLen);
extern int GeneratePATPMT2(int pmtPid, unsigned char *buf, int bufLen);
extern int BufInfoAdd(int pid, char *data, int len);
extern int BufInfoInit(int count, int *pid);

#ifdef __cplusplus
}
#endif
#endif /*__CREATE_PSI_TABLE_H__*/

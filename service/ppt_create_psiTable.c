/************************************************************************************/
/*
 * **********************************************************************************
 *	Copyright(C) 2012  joysee	ALL RIGHTS RESERVED 
 *  Please review the terms of the license agreement before using
 *  this file. If you are not an authorized user, please destroy this
 *  source code file and notify CHINA DTV immediately that you
 *  inadvertently received an unauthorized copy.
 * **********************************************************************************
 * \file createpsitable.c
 *
 * h file	.
 * File description        : create PAT/PMT table Code
 * author				   : tpwang																 
 * 		
 * History:																  
 *			----------------------------------------------------				 
 *			V0.01, 2013.08.15, tpwang, initial version	
 *			V0.02, 2013.08.15, tpwang, last modification							  
 ************************************************************************************/
#include "ppt_create_psiTable.h"
#include<stdio.h>
#include <string.h>

unsigned char PAT_DATA_TABLE[TABLE_LEN] = 
{
/*0*/   0x47,0x40,0x00,0x1A,0x00,0x00,0xB0,0x11,0x00,0x07,
/*1*/	0xCD,0x00,0x00,0x00,0x00,0xE0,0x10,0x00,0x02,0xE0,
/*2*/	0x02,0x6F,0xB0,0x77,0xAD,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*3*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*4*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*5*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*6*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*7*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*8*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 
/*9*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*10*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*11*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*12*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*13*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*14*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  
/*15*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*16*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*17*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,      
/*18*/  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF                                                                   
};

unsigned char PMT_DATA_TABLE[TABLE_LEN] = 
{
/*0*/   0x47,0x40,0x02,0x18,0x00,0x02,0xB0,0x1B,0x00,0x02,
/*1*/	0xCF,0x00,0x00,0xFF,0xFE,0xF0,0x04,0x0B,0x02,0xC0,
/*2*/	0x1F,0x02,0xE2,0x00,0xF0,0x00,0x03,0xE2,0x8A,0xF0,
/*3*/	0x00,0x31,0xFC,0x67,0x2C,0xFF,0xFF,0xFF,0xFF,0xFF,
/*4*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*5*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*6*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*7*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*8*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 
/*9*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*10*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*11*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*12*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*13*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*14*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  
/*15*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*16*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,                             
/*17*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 	
/*18*/  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

static unsigned int CRC( unsigned char const* pData, unsigned short nDataLen )
{
    unsigned int crc = 0;
    unsigned char data = 0;
    unsigned short i = 0;
    int temp = 0;
    int  j =0;
	unsigned char bitbase[9] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00 };
	if( 0 == pData || 0 == nDataLen )
		return 0;
	crc = 0xFFFFFFFF;
	for(  i = 0; i < nDataLen; ++ i )
	{
		data = pData[i];
		for( j = 0; j <= 7; ++ j )
		{
			temp = ((crc & 0x80000000) != 0) ^ ((data & bitbase[j]) != 0);
			crc  <<= 1;
			if( temp != 0 )
				crc ^= 0x04C11DB7; // 0x04C11DB7 000 0100 1100 0001 0001 1101 1011 0111
		}
	}
	return crc;
}

void GeneratePMT(unsigned short Vpid, unsigned char Vtype, unsigned short Apid,unsigned char Atype)
{
    int i = 0;
    unsigned int crc = 0;   
    int pmt_section_len ;

    pmt_section_len = (PMT_DATA_TABLE[TS_HEAD_LEN+1] & 0x0F) << 8 | PMT_DATA_TABLE[TS_HEAD_LEN+2];
	PMT_DATA_TABLE[ES_PID_A] = ((PMT_DATA_TABLE[ES_PID_A] & 0xE0 )| ((Apid >> 8) & 0x1F));
	PMT_DATA_TABLE[ES_PID_A+1] = Apid & 0xFF;
	
	PMT_DATA_TABLE[ES_PID_V] = ((PMT_DATA_TABLE[ES_PID_V] & 0xE0) | ((Vpid >> 8) & 0x1F));
	PMT_DATA_TABLE[ES_PID_V+1] = Vpid & 0xFF;
	
	PMT_DATA_TABLE[A_TYPE] = Atype;
	PMT_DATA_TABLE[V_TYPE] = Vtype;
	crc = CRC(&PMT_DATA_TABLE[TS_HEAD_LEN],pmt_section_len-1);
	for(i = 0;i < 4;i ++)
	{	
		PMT_DATA_TABLE[PMT_CRC+i] = (crc >> (3 - i) * 8) & 0xFF;
	}
}

int GeneratePATPMT(unsigned short Vpid, unsigned char Vtype, unsigned short Apid,unsigned char Atype, unsigned char *buf, int bufLen)
{
    if (bufLen < 188*2 || !buf){
        printf("Input error\n");
        return -1;
    }
    
    int i = 0;
    unsigned int crc = 0;   
    int pmt_section_len ;
    int pat_section_len ;
    unsigned char pmtTable[188] = {0};
    unsigned char patTable[188] = {0};
    unsigned char pmtPid = 0x20;
    
    for(pmtPid = 0x20; pmtPid < 0xff; ++pmtPid){
        if((Vpid != pmtPid) && (Apid != pmtPid)){
            break;
        }
    }

    //pat
    memcpy(patTable, PAT_DATA_TABLE, 188);
    patTable[PMTPID] = ((pmtTable[PMTPID] & 0xE0 )| ((pmtPid >> 8) & 0x1F));
    patTable[PMTPID+1] = pmtPid & 0xff;
    pat_section_len = (patTable[TS_HEAD_LEN+1] & 0x0F) << 8 | patTable[TS_HEAD_LEN+2];
	crc = CRC(&patTable[TS_HEAD_LEN], pat_section_len-1);
	for(i = 0;i < 4;i ++)
	{	
		patTable[PAT_CRC+i] = (crc >> (3 - i) * 8) & 0xFF;
	}    

    //pmt
    memcpy(pmtTable, PMT_DATA_TABLE, 188);
    pmt_section_len = (pmtTable[TS_HEAD_LEN+1] & 0x0F) << 8 | pmtTable[TS_HEAD_LEN+2];
	pmtTable[ES_PID_A] = ((pmtTable[ES_PID_A] & 0xE0 )| ((Apid >> 8) & 0x1F));
	pmtTable[ES_PID_A+1] = Apid & 0xFF;
	
	pmtTable[ES_PID_V] = ((pmtTable[ES_PID_V] & 0xE0) | ((Vpid >> 8) & 0x1F));
	pmtTable[ES_PID_V+1] = Vpid & 0xFF;
	
	pmtTable[A_TYPE] = Atype;
	pmtTable[V_TYPE] = Vtype;

	/* PCR_PID */
    pmtTable[13] = (pmtTable[1] & 0xE0) | (Vpid >> 8);
	pmtTable[14] = Vpid & 0xff;

    //pmt pid
    pmtTable[1] = ((pmtTable[1] & 0xE0 )| ((pmtPid >> 8) & 0x1F));
    pmtTable[2] = pmtPid & 0xff;
    
	crc = CRC(&pmtTable[TS_HEAD_LEN],pmt_section_len-1);
	for(i = 0;i < 4;i ++)
	{	
		pmtTable[PMT_CRC+i] = (crc >> (3 - i) * 8) & 0xFF;
	}

    memcpy(buf, patTable, 188);
    memcpy(buf + 188, pmtTable, 188);
    
    return 0;
}

#define MBOX_MAX_PROGRAMME 36
typedef struct PmtBufInfo_{
    int flag;
    int pid;
    int len;
    char data[0];
}PmtBufInfo;

typedef struct{
    int count;
    char buf[MBOX_MAX_PROGRAMME][1024];
}BufInfo;

static BufInfo gInfo;
int BufInfoInit(int count, int *pid)
{
    memset(&gInfo, 0, sizeof(BufInfo));
    gInfo.count = count;
    int i = 0; 
    for (i = 0; i < count; ++i){
        PmtBufInfo *pInfo = (PmtBufInfo *)gInfo.buf[i];
        pInfo->pid = pid[i];
    }    
    return 0;
}

int BufInfoAdd(int pid, char *data, int len)
{
    int i = 0; 
    for (i = 0; i < MBOX_MAX_PROGRAMME; ++i){
        PmtBufInfo *pInfo = (PmtBufInfo *)gInfo.buf[i];
        if (pInfo->pid == pid){
            if (pInfo->flag == 0){
                memcpy(pInfo->data, data, len);
                pInfo->flag = 1;
            }

            break;
        }
    }
    return 0;
}

int GeneratePATPMT2(int pmtPid, unsigned char *buf, int bufLen)
{
    if (bufLen < 188*2 || !buf){
        printf("Input error\n");
        return -1;
    }
    
    int i = 0;
    unsigned int crc = 0;   
    int pat_section_len ;
    unsigned char pmtTable[188] = {0};
    unsigned char patTable[188] = {0};
    
    //pat
    memcpy(patTable, PAT_DATA_TABLE, 188);
    patTable[PMTPID] = ((pmtTable[PMTPID] & 0xE0 )| ((pmtPid >> 8) & 0x1F));
    patTable[PMTPID+1] = pmtPid & 0xff;
    pat_section_len = (patTable[TS_HEAD_LEN+1] & 0x0F) << 8 | patTable[TS_HEAD_LEN+2];
	crc = CRC(&patTable[TS_HEAD_LEN], pat_section_len-1);
	for(i = 0;i < 4;i ++)
	{	
		patTable[PAT_CRC+i] = (crc >> (3 - i) * 8) & 0xFF;
	}    

    //pmt
    memcpy(pmtTable, PMT_DATA_TABLE, 188);
    pmtTable[1] = ((pmtTable[1] & 0xE0 )| ((pmtPid >> 8) & 0x1F));
    pmtTable[2] = pmtPid & 0xff;  

    for (i = 0; i < gInfo.count; ++i){
        PmtBufInfo *pInfo = (PmtBufInfo *)gInfo.buf[i];
        if (pInfo->pid == pmtPid){
            if (pInfo->flag == 1 && pInfo->len < (188 - TS_HEAD_LEN)){
                memcpy(&pmtTable[TS_HEAD_LEN], pInfo->data, pInfo->len);
            }
            else{
                printf("Use GeneratePATPMT!\n");
                return -1;
            }
        }
    } 

    if (i < gInfo.count){
        memcpy(buf, patTable, 188);
        memcpy(buf + 188, pmtTable, 188);
        return 0;
    }
    
    return -1;
}

void GeneratePMT1(unsigned short Vpid, unsigned char Vtype, unsigned short Apid,unsigned char Atype,unsigned char uCount)
{
    int i = 0;
	int index = 0;
    unsigned int crc = 0;   
	unsigned short section_len = 0;

	/* table_id*/
	PMT_DATA_TABLE[5]   = 0x2;
	
	/* section length */
	/* program number */
	/* version number */
	/* section number */
	/* last_section_number */
		
	/* PCR_PID */
	PMT_DATA_TABLE[13] = (Vpid >> 8) | 0xe0;
	PMT_DATA_TABLE[14] = Vpid;
	
	/*program info length*/
	PMT_DATA_TABLE[15]=0x0;
	PMT_DATA_TABLE[16]=0x0;

	index = 16;
	section_len = 9;
	
	if(Vpid > 0 && Vpid < 0x1fff)
	{
		// ES:VIDEO
		PMT_DATA_TABLE[index+1] = Vtype;
		PMT_DATA_TABLE[index+2] = (Vpid >> 8) | 0xe0;
		PMT_DATA_TABLE[index+3] = Vpid;
		PMT_DATA_TABLE[index+4] = 0x00;
		PMT_DATA_TABLE[index+5] = 0x00;
		
		index+=5;	// index=21
		section_len += 5;
	}
	if(Apid > 0 && Apid < 0x1fff)
	{
		// ES:AUDIO
		PMT_DATA_TABLE[index+1] = Atype;
		PMT_DATA_TABLE[index+2] = (Apid >> 8) | 0xE0;
		PMT_DATA_TABLE[index+3] = Apid;
		PMT_DATA_TABLE[index+4] = 0x00;
		PMT_DATA_TABLE[index+5] = 0x00;
		
		index+=5;	// index=26
		section_len += 5;
	}
	
	/* section length */
	PMT_DATA_TABLE[6] = (section_len >> 8) | 0xE0;
	PMT_DATA_TABLE[7] = section_len;
	
	crc = CRC(&PMT_DATA_TABLE[5],section_len-1);
	for(i = 1;i <= 4;i ++)
	{	
		PMT_DATA_TABLE[index+1] = (crc >> (4 - i) * 8) & 0xFF;
	}
	
	index +=4;	// index=30
	for( i = index+1;i < 188;i++){
		PMT_DATA_TABLE[i] = 0xFF;
	}
}

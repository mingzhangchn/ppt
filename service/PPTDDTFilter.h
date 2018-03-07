#ifndef PPTDDTFilter_H
#define PPTDDTFilter_H

#include "PPTDDTSectionManager.h"

int PPTDDTFilterStart(int pid, int tableId, PPTDDTSectionManager *pSecMgr);
int PPTDDTSectionRecvFalg();
int PPTDDTFilterStop(void);

#endif





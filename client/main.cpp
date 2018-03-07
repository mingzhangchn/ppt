
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h> 
#include "PPTClient.h"



using namespace android;

int main()
{
    PPTClient::self()->startHlsService();
    
    
    return 0;
}
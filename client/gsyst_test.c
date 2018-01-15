#include "gsyst.h"


int motors_init() {
    #ifndef __ARM_ARCH_4T__
    // Disable auto-detection of the brick
    //(you have to set the correct address below)
    ev3_brick_addr = EV3_BRICK_ADDR;
    #endif
    if ( ev3_init() == -1 ) return ( 1 );
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );
    printf( "Found tacho motors:\n" );
    return 0;
}


int main(int argc, char const *argv[])
{
	motors_init();

	printf("Ready to test grab..\n");
	Sleep(500);
	printf("instruct=1\n");
	grab(1);
	Sleep(5000);
	printf("instruct=-1\n");
	grab(-1);
	Sleep(5000);




	return 0;
}
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <pthreadUtils.h>
#include <Pipes.h>
#include <pipeHandler.h>
#ifndef SW
#include "SockPipes.h"
#include "vhdlCStubs.h"
#else
#include "aa_c_model.h"
#endif

void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

	
int main(int argc, char* argv[])
{
	int _err_ = 0;
	signal(SIGINT,  Exit);
  	signal(SIGTERM, Exit);

#ifdef SW
	start_daemons(stdout, 0);	
#endif

	
//	uint8_t idx;
//	uint8_t jdx;
//	for(idx = 1; idx < 30; idx++)
//	{
//	  for(jdx = 1; jdx < 15; jdx++)
//		{
		float x,y;
		float z;
		
		x = 50.7898;
		y=0.23455;
	
		for (int i=1; i<80; i++){
			x=x+0.25;
			y=y+0.75;
		
	
#ifdef SW
		fp_divider(x,y,&z);
#else
		z=fp_divider(x,y);
#endif
		

		//if (z != (x/y))
		//{
			fprintf(stderr, "Expected Value: %f / %f = %f, Divider Output: %f\n", x,y,x/y,z);
		//	_err_ = 1;
		//}

		/*write_uint8("input_data", x);
		write_uint8("input_data", y);
		z = read_uint8("output_data");
		if(z != x*y)
		{
			fprintf(stderr, "Error: output_data = %d, expected %d.\n", z,x*y);
			_err_ = 1;
		}
		*/
		}
//		}	
//	fprintf(stderr,"All done: %s\n", _err_ ? "Failure :-(" : "Success :-)");
	return(0);
}

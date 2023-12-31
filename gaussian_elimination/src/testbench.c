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
#define ROW_SIZE 16
#define COL_SIZE 17
int all_done, all_read = 0;
float matrix[ROW_SIZE][COL_SIZE]=
{{167.68,-24.52,26.36,8.55,2.06,36.09,24.52,-40.77,-55.13,-55.12,-55.8,33.57,97.91,-31.86,90.69,26.37,0.46}
,{-24.52,196.67,16.67,71.32,39.74,-46.63,-33.15,-44.94,-74.43,-6.92,41.06,-46.42,5.41,25.57,-31.02,81.78,5.61}
,{26.36,16.67,114.46,21.02,-65.13,50.51,15.14,-47.57,-63.1,1.32,25.87,46.06,-26.57,-44.99,35.55,-5.15,0.55}
,{8.55,71.32,21.02,327.37,-129.07,121.36,-54.96,-45.51,-29.56,-57.12,9.03,-19.93,0.04,188.4,37.1,41.78,-3.22}
,{2.06,39.74,-65.13,-129.07,253.21,-132.72,5.21,42.54,23.95,4.0,-30.93,-23.24,-14.71,-110.4,18.43,30.27,2.97}
,{36.09,-46.63,50.51,121.36,-132.72,231.04,18.98,-16.48,-51.71,-0.85,14.06,17.42,28.64,85.67,56.66,10.39,-0.41}
,{24.52,-33.15,15.14,-54.96,5.21,18.98,75.79,-6.17,-56.85,-2.72,-37.68,50.27,11.36,-64.61,4.45,-4.77,0.04}
,{-40.77,-44.94,-47.57,-45.51,42.54,-16.48,-6.17,102.3,67.99,36.98,45.65,-49.65,-70.63,-11.61,-14.43,-26.8,-0.69}
,{-55.13,-74.43,-63.1,-29.56,23.95,-51.71,-56.85,67.99,262.2,44.62,26.69,-80.36,2.11,-14.73,-29.01,-23.01,-0.91}
,{-55.12,-6.92,1.32,-57.12,4.0,-0.85,-2.72,36.98,44.62,67.32,50.71,1.78,-35.35,-34.23,-12.83,-4.93,-0.91}
,{-55.8,41.06,25.87,9.03,-30.93,14.06,-37.68,45.65,26.69,50.71,152.03,-45.44,-88.23,51.25,-41.74,-25.95,-9.78}
,{33.57,-46.42,46.06,-19.93,-23.24,17.42,50.27,-49.65,-80.36,1.78,-45.44,194.76,24.02,-13.67,12.21,-72.74,-0.63}
,{97.91,5.41,-26.57,0.04,-14.71,28.64,11.36,-70.63,2.11,-35.35,-88.23,24.02,213.84,14.05,46.82,46.43,-2.21}
,{-31.86,25.57,-44.99,188.4,-110.4,85.67,-64.61,-11.61,-14.73,-34.23,51.25,-13.67,14.05,300.44,-50.53,-59.76,9.79}
,{90.69,-31.02,35.55,37.1,18.43,56.66,4.45,-14.43,-29.01,-12.83,-41.74,12.21,46.82,-50.53,228.36,20.74,0.02}
,{26.37,81.78,-5.15,41.78,30.27,10.39,-4.77,-26.8,-23.01,-4.93,-25.95,-72.74,46.43,-59.76,20.74,139.15,-3.38}};
void WriteInput()
{
    float wbuf[272];
    int k=0;
    for(int i =0;i<ROW_SIZE;i++)
    for(int j=0;j<COL_SIZE;j++)
    {
       wbuf[k] = matrix[i][j]; 
       k+=1;
    }


    write_float32_n("input_data", wbuf, 272);

    
}

void ReceiveResults()
{

    float rbuf[288];
    read_float32_n("output_data", rbuf, 288);
    int I;
    printf("The upper triangular matrix is \n");
    for (I = 0; I < 272; I++)
    {
        fprintf(stderr, "%f  ", rbuf[I]);
        if ((I+1)%17 == 0) 
         printf("\n");
    }
    printf("The solution is \n");
    for (I = 272; I < 288; I++)
    {
        fprintf(stderr, "%f  ", rbuf[I]);
        if ((I == 287) )//|| (I == 9) || (I == 14) || (I == 19))
            printf("\n");
    }

    all_done = 1;
}
DEFINE_THREAD(WriteInput)
DEFINE_THREAD(ReceiveResults)
void Exit(int sig)
{
    fprintf(stderr, "## Break! ##\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    int _err_ = 0;
    signal(SIGINT, Exit);
    signal(SIGTERM, Exit);

#ifdef SW
    start_daemons(stdout, 0);
#endif

    PTHREAD_DECL(WriteInput);
    PTHREAD_DECL(ReceiveResults);


    PTHREAD_CREATE(WriteInput);
    PTHREAD_CREATE(ReceiveResults);
    ge();

    while (!all_done)
    {
    }

    return (0);
}

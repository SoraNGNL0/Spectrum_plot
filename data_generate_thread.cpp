#include "data_generate_thread.h"
#include <Windows.h>
#include <cstring>
#include <QDebug>
#include <QReadWriteLock>
extern QReadWriteLock in_lock;

extern bool run_flag;
int fft_len = 256;
extern bool clk_flag;

data_generate_thread::data_generate_thread()
{
    fps = 32; //每秒帧数
}

void data_generate_thread::run()
{
    //qDebug()<<"1";
    double *in = new double[fft_len];
    //in_lock.lockForWrite();
    int k = 0;
    while(run_flag)
    {
        for(int j=0;j<fft_len/2;j++)
        {
            in[j] = k%50;
            if (j <= 10)
                in[j] = 100;
        }
        in[fft_len/2]=1;
        for(int j=fft_len/2+1;j<fft_len;j++)
        {
            in[j] = k%50;
            if (j <= 10)
                in[j] = 100;
        }
        //qDebug()<<"in[i][1]="<<in[i][1];
        emit data_complete(in,fft_len);
        Sleep(1000/fps);
        //qDebug()<<"i="<<i;
        ++k;
    }
    //in_lock.unlock();
}

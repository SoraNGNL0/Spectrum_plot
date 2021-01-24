#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

bool run_flag;
extern int fft_len;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    run_flag = false;
    ui->setupUi(this);

    data_generate=new data_generate_thread;

    waterfall=new WaterfallWidget(this,160,fft_len);
    QHBoxLayout *time_fre_layout=new QHBoxLayout();
    time_fre_layout->setContentsMargins(0,0,0,0);
    time_fre_layout->addWidget(waterfall);
    ui->graphicsView->setLayout(time_fre_layout);
    connect(data_generate,SIGNAL(data_complete(double*,int)),waterfall,SLOT(updateWithData(double *,int)));


    waterfall2=new WaterfallWidget(this,100,fft_len);
    waterfall2->horizontal = false;
    QHBoxLayout *time_space_layout=new QHBoxLayout();
    time_space_layout->setContentsMargins(0,0,0,0);
    time_space_layout->addWidget(waterfall2);
    ui->graphicsView_2->setLayout(time_space_layout);
    connect(data_generate,SIGNAL(data_complete(double*,int)),waterfall2,SLOT(updateWithData(double *,int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::datastart()
{
    waterfall->res_location = 0;
    waterfall->st_point=0;
    if(run_flag)
    {
        run_flag = false;
        data_generate->wait();
    }
    else
    {
        if(!data_generate->isRunning())
            data_generate->start();
        run_flag = true;
    }

}


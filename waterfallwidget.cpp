#include "waterfallwidget.h"

#include <QPainter>
#include <QDebug>

#include <cstring>

#include <algorithm>
#include <complex>
#include <cmath>
#include <climits>
#include <QtAlgorithms>
#include <windows.h>

static const QImage::Format ImageFormat = QImage::Format_RGB888; //RGB888---colch=3 ,RGB32---colch=4


WaterfallWidget::WaterfallWidget(QWidget *parent,size_t lc_in, size_t pp_in) :
    QWidget(parent), _image_mutex(new QMutex()),
    _image(new QImage(size(), ImageFormat))
{
    rubberBand=NULL;
    st_point=0;
    setImgSize(lc_in,pp_in);
}

WaterfallWidget::~WaterfallWidget()
{
    delete _image;
    if(first_flag)
        delete[] bits_bf;
}

void WaterfallWidget::setImgSize(size_t lc_in, size_t pp_in){
    line_cnt = lc_in,
    points_perline = pp_in;
    ori_lineCnt = line_cnt;
    ori_perline = points_perline;
    bits_bf = new uchar[line_cnt*points_perline*colch]; //4表示4个通道——RGBA
    QSize size(ori_perline,ori_lineCnt);
    ori_image=new QImage(size,ImageFormat);//original_image initialized
}

void WaterfallWidget::paintEvent(QPaintEvent *event)
{
    QMutexLocker lock(_image_mutex);
    QPainter painter(this);

    if(horizontal)
    {
        painter.rotate(90.0);
        painter.scale(1, -1);
        painter.drawImage(QRect(0, 0, height(), width()), *_image);
    }
    else
        painter.drawImage(QRect(0, 0, width(), height()), *_image);


    //painter.drawImage(0, 0, *_image);
}

void WaterfallWidget::resizeEvent(QResizeEvent *event)//拉伸之后图片不会报错
{

    QWidget::resizeEvent(event);

    QMutexLocker lock(_image_mutex);
    //delete _image;
    //qDebug()<<"resizeEvent!";
    _image = new QImage(event->size(), ImageFormat);
    _image->fill(Qt::gray);
}

void WaterfallWidget::updateWithData(double *data,int len)
{
    baseVal = maxVal-minVal;
    if(!first_flag)
    {
        bits_bf = new uchar[line_cnt*len*colch];
        qDebug()<<"分配bits_bf";
    }
    QMutexLocker lock(_image_mutex);
    QSize size;
    int w;
    int h;
    w = line_cnt;
    h = points_perline;
    size.setHeight(w);
    size.setWidth(h);
    //shift the image one row up
    uchar* bits = _image->bits();
    size_t bytes_per_line = _image->bytesPerLine();
    size_t old_size = bytes_per_line * (_image->height() - 1);//除了需要画的那根线以外的字节数
    if (size != _image->size())
    {
        _image = new QImage(size, ImageFormat);
        qDebug()<<"lzhlzhlzhlzhlzhlzhlzhlzhlzh"<<size;
        _image->fill(Qt::blue);
        if(first_flag)
        {
            uchar* bits1 = _image->bits();//Image的宽
            ::memmove(bits1,bits_bf,line_cnt*points_perline*colch);
            //repaint();
        }

    }
    bits_bf_lock.lockForWrite();
    if(horizontal)
    {
        ::memmove(bits, bits + bytes_per_line, old_size);
        {
           for (size_t i = 0; i < points_perline;i++)//qMin((qint64)_image->width(), len)
            {
               _image->setPixel(i,w-1,getRgbAt(data[i],7));
            }
        }
    }
    else
    {
        ::memmove(bits + bytes_per_line, bits,  old_size);
        ::memmove(ori_image->bits()+ori_image->bytesPerLine(), ori_image->bits(), ori_image->bytesPerLine()*(ori_lineCnt-1));
       for (size_t i = 0; i < points_perline;i++)//qMin((qint64)_image->width(), len)
       {
            _image->setPixel(i,0,getRgbAt(data[i+st_point],7));
       }
       for (size_t i = 0; i < ori_perline;i++)
           ori_image->setPixel(i,0,getRgbAt(data[i],7));
    }
    update(); //优于repaint()
    uchar* bits1 = _image->bits();
    ::memmove(bits_bf,bits1,line_cnt*points_perline*colch);
    bits_bf_lock.unlock();
    first_flag = true;

}

QRgb WaterfallWidget::getRgbAt(double amp, int flag)
{
    QColor col;
    if(abs(isinf(amp))||isnan(amp))
        amp=0;
    dVal = amp - minVal;
    index = floor(dVal/baseVal*64);
    index = qMin(index,63);
    index = qMax(index,0);
    switch (flag)
    {
    case 1:
    {
        col.setRgb(redextenedkindman64[index],greenextenedkindman64[index],blueextenedkindman64[index]);
        break;
    }
    case 2:
    {

        col.setRgb(redkindman64[index],greenkindman64[index],bluekindman64[index]);
        break;
    }
    case 3:
    {
        col.setRgb(redextendblackbody64[index],greenextendblackbody64[index],blueextendblackbody64[index]);
        break;
    }
    case 4:
    {

        col.setRgb(redbentcoolwarmtable[index],greenbentcoolwarmtable[index],bluebentcoolwarmtable[index]);
        break;
    }
    case 5:
    {

        col.setRgb(redsmoothcoolwarmtable[index],greensmoothcoolwarmtable[index],bluesmoothcoolwarmtable[index]);
        break;
    }
    case 6:
    {

        col.setRgb(redJet32[index],greenJet32[index],blueJet32[index]);
        break;
    }
    case 7:
    {

        col.setRgb(redJet64[index],greenJet64[index],blueJet64[index]);
        break;
    }
    default:
    {

        col.setRgb(redJet64[index],greenJet64[index],blueJet64[index]);
    }
        break;
    }
    return col.rgb();
}


void WaterfallWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(rubberBand)
        rubberBand->setGeometry(QRect(origin,event->pos()).normalized());
    QWidget::mouseMoveEvent(event);
}

void WaterfallWidget::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"pos="<<event->pos.x();
    QWidget::mousePressEvent(event);
    origin=event->pos();
    if(!rubberBand)
        rubberBand=new QRubberBand(QRubberBand::Rectangle,this);
    rubberBand->setGeometry(QRect(origin,QSize()));
    rubberBand->show();
    temp1.setX(origin.x()*1.0/width()*points_perline);//reverse the event pot to real pos
    temp1.setY((origin.y()*1.0/height())*line_cnt);//reverse the event pot to real pos
    qDebug()<<"temp1="<<temp1;
    if(event->button()==Qt::RightButton)
    {
       bits_bf_lock.lockForWrite();
       st_point = 0;
       points_perline=ori_perline;
       line_cnt=ori_lineCnt;
       bits_bf = new uchar[line_cnt*points_perline*colch];
       ::memmove(bits_bf,ori_image->bits(),line_cnt*points_perline*colch);
       res_location = 0;//返回的时候左上角的累加数置为0
       redraw = true;
       bits_bf_lock.unlock();
       qDebug()<<"---------------------------------------------------unrobust";
    }
    QWidget::mousePressEvent(event);
}

void WaterfallWidget::wheelEvent(QWheelEvent *event)
{

}

void WaterfallWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(rubberBand)
        rubberBand->hide();
    if(redraw){
        redraw = false;
        return;
    }
    temp2.setX((event->pos().x()*1.0/width())*points_perline);//reserve event_pos to real pos
    temp2.setY((event->pos().y()*1.0/height())*line_cnt);//reserve event_pos to real pos
    if((abs(temp2.x()-temp1.x())<=3) || (abs(temp2.y()-temp1.y()<=3)))
    {
        return;
    }
    temp2.setX(qMin(temp2.x(),(int)points_perline));
    temp2.setX(qMax(temp2.x(),0));
    temp2.setY(qMin(temp2.y(),(int)line_cnt));
    temp2.setY(qMax(temp2.y(),0));
    qDebug()<<"temp2="<<temp2;
    if(!horizontal)
    {
        res_location=res_location+qMin(temp1.x(),temp2.x());
        if((abs(temp2.x()-temp1.x())>3) && (abs(temp2.y()-temp1.y()>3)))
        {
            bits_bf_lock.lockForWrite();
            st_point = qMin(temp1.x(),temp2.x());
            line_cnt = abs(temp2.y()-temp1.y());
            points_perline = abs(temp2.x()-temp1.x());
            bits_bf = new uchar[line_cnt*points_perline*colch]; //4表示4个通道——RGBA
            uchar *bits = _image->bits();
            for(int i=0;i<line_cnt;i++)
            {
                qDebug()<<"_image->bytesPerLine()="<<_image->bytesPerLine();
                qDebug()<<"points_perline="<<points_perline;
                qDebug()<<"colch="<<colch;
                 ::memmove(bits_bf+i*points_perline*colch,bits+_image->bytesPerLine()*(qMin(temp1.y(),temp2.y())-1+i)+st_point*colch,colch*points_perline);
            }
            bits_bf_lock.unlock();
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void WaterfallWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(!horizontal)
    {
        if(points_perline==ori_perline)
        {
            clk_location=event->pos().x()*1.0/width()*ori_perline;
            //qDebug()<<"clk_location="<<clk_location;
        }
        else
        {
            //qDebug()<<"res_location="<<res_location;
            clk_location=res_location+(int)(((event->pos().x())*1.0/width())*points_perline);
            //qDebug()<<"clk_location="<<clk_location;
        }
    }
}


















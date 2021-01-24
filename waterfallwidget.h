#ifndef WATERFALLWIDGET_H
#define WATERFALLWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <QRubberBand>
#include <QReadWriteLock>
#include <math.h>

#include "colormap.h"

class WaterfallWidget : public QWidget
{
    Q_OBJECT
protected:
    QMutex* _image_mutex;
    QReadWriteLock bits_bf_lock;
    QImage* _image;
    QImage* ori_image;

public:
    explicit WaterfallWidget(QWidget *parent,size_t lc_in, size_t pp_in);
    ~WaterfallWidget();
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    QRgb getRgbAt(double amp,int flag);
    uchar *bits_bf;
    uchar *bits_mouse_bf;//use for mouse stretch
    bool first_flag = false;
    bool horizontal = true; //the direction of the waterfall
    size_t line_cnt;//the line count in waterfall plot
    size_t points_perline;
    int ori_lineCnt,ori_perline;
    int st_point;
    int index;
    double maxVal = 80;
    double minVal = 0;
    double dVal;
    double baseVal;
    QPoint temp1,temp2;
    bool redraw = false;
    int res_location=0;//reserve the upleft location
    int clk_location=0;//standard location

    void setImgSize(size_t lc_in, size_t pp_in);


private:
    size_t colch = 3; //the bytes of each point
    QRubberBand *rubberBand;
    QPoint origin;

public slots:
    void updateWithData(double *data,int len);
};

#endif // WATERFALLWIDGET_H

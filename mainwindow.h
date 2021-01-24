#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data_generate_thread.h"
#include "waterfallwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    data_generate_thread *data_generate;
    WaterfallWidget *waterfall, *waterfall2;

private slots:
    void datastart();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

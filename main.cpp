﻿#include "mainwindow.h"
#include <QApplication>
#include <QReadWriteLock>

QReadWriteLock in_lock;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

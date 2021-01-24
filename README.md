# Spectrum_plot
![](https://img.shields.io/badge/C++-95.6%-brightgreen.svg) ![](https://img.shields.io/badge/qt-5-yellow.svg)![](https://img.shields.io/badge/release-v2021.1.24-red.svg)

This project is about using the Qt platform to draw time-frequency domain waveforms. It is worth noting that the dynamic waveforms are drawn here, that is, the multi-threading function in Qt is used.

## Software

In this project, the software and compilers we used are as follows：

- windows10
- Qt creator 4.9.0
- Qt 5.12.2
- Microsoft Visual C++ Complier 15.0(x86)

## Description 

The functions of each code file are described as follows(.h and .cpp)：

- **data_generate_thread**：thread function，continuous signal data is generated in this thread.
- **colormap**：Values corresponding to different colors of the two-dimensional graph
- **waterfallwidget**：mouse operation, including scroll wheel operation (screen zoom in or zoom out), mouse operation (screen zoom in, restore).
- **timefre**：receiving time domain data and dynamic display operations, including the short time-frequency transform of time data, three operations of removing the beginning time-frequency data, connecting the end time-frequency data, and displaying the whole time-frequency data.
- **mainwindow**：the function is directly associated with mainwindow.ui, call data_generate_thread, chartview, acousticwidget three functions to draw dynamic time-frequency-domain waveforms.
- **main**：call mainwindow function.

## Example

The running results of the project are as follows：

![img](https://github.com/blank1996/Signal_Plot/blob/main/gif_show.gif)



## change log

V1(2021.1.25)

- Upload code of project
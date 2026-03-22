/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2022 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: https://www.qcustomplot.com/                         **
**             Date: 06.11.22                                             **
**          Version: 2.1.1                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This started from the example code RealTimePlot for QCustomPlot.                                                              **
**                                                                                                         **
*************************************************************************************************************/

#ifndef VoltageWindow_H
#define VoltageWindow_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"


namespace Ui {
class VoltageWindow;
}

class VoltageWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit VoltageWindow(QApplication *a, QWidget *parent = 0);
  ~VoltageWindow();
  
  void setupRealtimeDataDemo(QCustomPlot *customPlot);
  void setupParametricCurveDemo(QCustomPlot *customPlot);

  void ProcessLine(QString line);
  void GetData(QString fileName);
  QStringList first,second;
  uint32_t index, lastindex;

  QVector<double> Time, Voltage;

private slots:
  void realtimeDataSlot();
  void screenShot();

private:
  Ui::VoltageWindow *ui;
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  int currentDemoIndex;
  QApplication* qapp; //?
};

#endif // VoltageWindow_H

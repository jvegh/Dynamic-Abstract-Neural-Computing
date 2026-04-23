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
#include <QVector>
#include "ScQtSimulator.h"
#include "NeuronPhysical.h"
#include "qcustomplot.h"


namespace Ui {
class VoltageWindow;
}

class VoltageWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit VoltageWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent = 0);
  ~VoltageWindow();
  
  void setupRealtimeDataDemo(QCustomPlot *customPlot);
  void setupParametricCurveDemo(QCustomPlot *customPlot);

  void ProcessLine(QString line);
  void GetData(QString fileName);
  void replot(void);
  void ResetDisplay(void){index = 0;}
  QStringList first,second;
  uint32_t index, lastindex;

  QVector<double> Time, Voltage, Gradient;

private slots:
  void realtimeDataSlot();
  void screenShot();
  void screenshotFilesTriggered();

private:
  Ui::VoltageWindow *ui;
  ScQtSimulator * m_Simulator;
  NeuronPhysical *m_neuron;
  void setupMenus();
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  int currentDemoIndex;
//  QApplication* qapp; //?
  QVector<double> x, y0, y1;

  QCPCurve *VoltagePlot;
  QVector<QCPCurveData> dataVoltagePlot;
  QCPItemEllipse *RunningPoint;


};

#endif // VoltageWindow_H

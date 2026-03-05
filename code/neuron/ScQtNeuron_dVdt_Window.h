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
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#ifndef SCQTNEURON_DVDTWINDOW_H
#define SCQTNEURON_DVDTWINDOW_H

//#include <QMainWindow>
#include <QWindow>
#include <QTimer>
#include "../../3rdParty/qcustomplot/include/qcustomplot.h"
#include "SimulatorControlWindow.h"

class ScQtNeuron_MainWindow;
namespace Ui {
class ScQtNeuron_dVdt_Window;
}

class ScQtNeuron_dVdt_Window : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ScQtNeuron_dVdt_Window(ScQtNeuron_MainWindow *parent);
  ~ScQtNeuron_dVdt_Window();
  
  void setupDemo(/*int demoIndex*/);
  void setupRealtimeDataDemo();
//  void setupParametricCurveDemo(QCustomPlot *customPlot);

  void setupPlayground(QCustomPlot *customPlot);

  void ProcessLine(QString line);
  void GetData(QString fileName);
  QStringList first,second;
  uint32_t index, lastindex;

  QVector<double> Time, Voltage, dVdt;

private slots:
  void realtimeDataSlot();
  void screenShot();
//  void allScreenShots();
  void simulatorEvent();
  
private:
  Ui::ScQtNeuron_dVdt_Window *ui;
    ScQtNeuron_MainWindow *m_parent;
  SimulatorControlWindow *m_Simulator_ControlWindow;
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  int currentDemoIndex;
};

#endif // SCQTNEURON_DVDTWINDOW_H

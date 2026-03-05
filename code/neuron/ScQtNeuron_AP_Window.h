
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

#ifndef SCQTNEURON_APWINDOW_H
#define SCQTNEURON_APWINDOW_H

//#include <QMainWindow>
#include <QWindow>
#include <QTimer>
#include "../../3rdParty/qcustomplot/include/qcustomplot.h"

class ScQtNeuron_MainWindow;
class SimulatorControlWindow;
namespace Ui {
class ScQtNeuron_AP_Window;
}

class ScQtNeuron_AP_Window : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ScQtNeuron_AP_Window(ScQtNeuron_MainWindow *parent);
    ~ScQtNeuron_AP_Window();
  
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
//  void setupParametricCurveDemo(QCustomPlot *customPlot);

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
  Ui::ScQtNeuron_AP_Window *ui;
//  QString demoName;
  QTimer dataTimer;
  QCustomPlot *customPlot;
//  QCPItemTracer *itemDemoPhaseTracer;
//  int currentDemoIndex;
  ScQtNeuron_MainWindow *m_parent;
  unsigned m_Mode;
  SimulatorControlWindow *m_Simulator_ControlWindow;
};

#endif // SCQTNEURON_APWINDOW_H

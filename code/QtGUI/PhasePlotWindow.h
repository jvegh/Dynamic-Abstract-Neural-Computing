
#ifndef PhasePlotWindow_H
#define PhasePlotWindow_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include "NeuronPhysical.h"
#include "ScQtSimulator.h"
#include "qcustomplot.h"


namespace Ui {
class PhasePlotWindow;
}

class PhasePlotWindow : public QMainWindow
{
    friend class ScQtNeuron_MainWindow;
  Q_OBJECT
  
public:
  explicit PhasePlotWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent = 0);
  ~PhasePlotWindow();
  
  void setupRealtimeDataDemo(//QCustomPlot *customPlot
      );
//  void setupParametricCurveDemo(QCustomPlot *customPlot);

  void ProcessLine(QString line);
  void GetData(QString fileName);
  void replot(void);
  void Reset();
  void ResetDisplay(void){index = 0;}
  void DisplayMode_Set(bool M);
  void RunningPointPosition_Set(double xpos, double ypos);
 // QStringList first,second, third, fourth;
  uint32_t index, lastindex;

//  QVector<double> Time, Voltage, Gradient;

private slots:
  void realtimeDataSlot();
  void screenShot();
  void screenshotFilesTriggered();

private:
  Ui::PhasePlotWindow *ui;
    ScQtSimulator * m_Simulator;
  NeuronPhysical *m_neuron;
  QString demoName;
//  QTimer dataTimer;
//  QCPItemTracer *itemPhaseTracer;
  QCPCurve *PhasePlot;
  void setupMenus();
  QVector<QCPCurveData> dataPhasePlot;
  bool m_DisplayMode;
//      int currentDemoIndex;
  QCPItemEllipse *RunningPoint;
};

#endif // PhasePlotWindow_H

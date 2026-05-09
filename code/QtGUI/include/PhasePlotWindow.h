
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
  bool DisplayMode_Get(void){return m_DisplayMode;}
  void RunningPointPosition_Set(double xpos, double ypos);
  void DrawArrow(double xpos, double ypos, QString S, double xoffset=1000, double yoffset=100);
 // QStringList first,second, third, fourth;
  uint32_t index, lastindex;

//  QVector<double> Time, Voltage, Gradient;

private slots:
  void realtimeDataSlot();
  void screenShot();

private:
  Ui::PhasePlotWindow *ui;
    ScQtSimulator * m_Simulator;
  NeuronPhysical *m_neuron;
  QCPCurve *PhasePlot;
  void setupMenus();
  QVector<QCPCurveData> dataPhasePlot;
  bool m_DisplayMode = true;
  QCPItemEllipse *RunningPoint;
};

#endif // PhasePlotWindow_H

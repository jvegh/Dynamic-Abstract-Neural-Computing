
/** @file CurrentWindow.h
 *  @brief The main window for the SystemC-based neuron simulator, using Qt's stuff
  */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#ifndef CurrentWindow_H
#define CurrentWindow_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include "ScQtSimulator.h"
#include "NeuronPhysical.h"
#include "qcustomplot.h"


namespace Ui {
class CurrentWindow;
}

class CurrentWindow : public QMainWindow
{
  Q_OBJECT
    friend class ScQtNeuron_MainWindow;

public:
  explicit CurrentWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent = 0);
  ~CurrentWindow();
  
  void setupDataPlot();

   void replot(void);
  void Reset();
  void ResetDisplay(void){index = 0;}
  void AISRunningPointPosition_Set(double xpos, double ypos);
  void NaRunningPointPosition_Set(double xpos, double ypos);
  void ResultingRunningPointPosition_Set(double xpos, double ypos);
  QStringList first,second;
  uint32_t index, lastindex;

  QVector<double> Time, Current, Gradient;

private slots:
  void displayDataSlot();
  void screenShot();

private:
  Ui::CurrentWindow *ui;
  ScQtSimulator * m_Simulator;
  NeuronPhysical *m_neuron;
  void setupMenus();
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  int currentDemoIndex;
//  QApplication* qapp; //?
  QVector<double> x, y0, y1;

  QCPCurve *AISCurrentPlot, *NaCurrentPlot, *ResultingCurrentPlot;
  QVector<QCPCurveData> dataAISCurrentPlot, dataNaCurrentPlot, dataResultingCurrentPlot;
  QCPItemEllipse *AISRunningPoint, *NaRunningPoint, *ResultingRunningPoint;

};

#endif // CurrentWindow_H

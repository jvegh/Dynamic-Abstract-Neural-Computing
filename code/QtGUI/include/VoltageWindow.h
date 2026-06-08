/** @file VoltageWindow.h
 *  @brief The main window for the SystemC-based neuron simulator, using Qt's stuff
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

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
    friend class ScQtNeuron_MainWindow;

public:
  explicit VoltageWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent = 0);
  ~VoltageWindow();
  
  void setupPlot(void);
  void DrawArrow(double xpos, double ypos, QString S, double xoffset=0, double yoffset=50);

  void ProcessLine(QString line);
  void GetData(QString fileName);
  void replot(void);
  void Reset();
  void ResetDisplay(void){index = 0;}
  void RunningPointPosition_Set(double xpos, double ypos);
  void DrawItemText(double xpos, double ypos, QString S, QColor Col);
  uint32_t index;

  QVector<double> Time, Voltage, Gradient;

private slots:
  void displayDataSlot();
  void screenShot();

private:
  Ui::VoltageWindow *ui;
  ScQtSimulator * m_Simulator;
  NeuronPhysical *m_neuron;
  void setupMenus();
  void PlotBrackets(int32_t key, double coord1, double coord2, double y);

  bool m_HaveAlreadyH, m_HaveAlreadyP;
  QCPCurve *VoltagePlot;
  QVector<QCPCurveData> dataVoltagePlot;
  QCPItemEllipse *RunningPoint;

  double m_T_DeliveringBegin;
  double m_T_RelaxingBegin;
  double m_V_Peak;
  bool m_FirstRelax;
};

#endif // VoltageWindow_H

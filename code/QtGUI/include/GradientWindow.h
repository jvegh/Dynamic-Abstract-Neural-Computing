/** @file GradientWindow.h
 *  @brief The gradient display window dfor the SystemC-based neuron simulator, using Qt's stuff
 *  Ideas taken from  https://www.qcustomplot.com/
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#ifndef GradientWindow_H
#define GradientWindow_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include "ScQtSimulator.h"
#include "NeuronPhysical.h"
#include "qcustomplot.h"


namespace Ui {
class GradientWindow;
}

class GradientWindow : public QMainWindow
{
  Q_OBJECT
  friend class ScQtNeuron_MainWindow;
  
public:
    explicit GradientWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent = 0);
    ~GradientWindow();
  
    void setupPlot(  );
    void DrawArrow(double xpos, double ypos, QString S, double xoffset=0, double yoffset=50);

    void replot(void);
    void Reset();
    uint32_t index;

    QVector<double> Time, Voltage, Gradient;

private slots:
    void displayDataSlot();
    void screenShot();
private:
    Ui::GradientWindow *ui;
    ScQtSimulator * m_Simulator;
    NeuronPhysical *m_neuron;
    QCPCurve *GradientPlot;
    QVector<QCPCurveData> dataGradientPlot;
    QCPItemEllipse *RunningPoint;

    QCPCurve *AISGradientPlot;
    QVector<QCPCurveData> dataAISGradientPlot;
    QCPItemEllipse *AISRunningPoint;

    QCPCurve *RushinGradientPlot;
    QVector<QCPCurveData> dataRushinGradientPlot;
    QCPItemEllipse *RushinRunningPoint;
    void RunningPointPositionGradient_Set(double xpos, double ypos);
    void AISRunningPointPositionGradient_Set(double xpos, double ypos);
    void RushinRunningPointPositionGradient_Set(double xpos, double ypos);
};

#endif // GradientWindow_H

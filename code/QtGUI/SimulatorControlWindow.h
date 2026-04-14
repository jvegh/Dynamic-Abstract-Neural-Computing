/** @file SimulatorControlWindow.h
 *  @brief A GUI for controling a semi-general purpose simulator for SystemC tasks, using Qt's stuff.
 *
 * @see ScQtSimulator
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#ifndef SimulatorControlWindow_H
#define SimulatorControlWindow_H

#include <QMainWindow>
#include <QThread>
#include <qdialog.h>
#include "ScQtSimulator.h"
#include "NeuronPhysical.h"
#include "Utils.h"

namespace Ui
{
class SimulatorControlWindow;
}

class ScQtNeuron_MainWindow;

class SimulatorControlWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulatorControlWindow( NeuronPhysical *Neuron, ScQtNeuron_MainWindow *parent = 0);
    ~SimulatorControlWindow();
    Ui::SimulatorControlWindow *ui;
    NeuronPhysical *m_Neuron;
    ScQtNeuron_MainWindow* parent_Get() { return m_parent;}
private:
    /**
     * @brief Thread object which will let us manipulate the running thread
     */
    QThread *thread;
    /**
     * @brief Object which contains methods that should be runned in another thread
     */
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    ScQtNeuron_MainWindow *m_parent;
private slots:
    void on_resetButton_clicked();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_eventHappened();
/*    void on_stepButton_clicked();
    void on_eventButton_clicked();
    void on_spikeButton_clicked();*/
};

#endif // SimulatorControlWindow_H

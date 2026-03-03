/*
   Copyright 2013 Fabien Pierre-Nicolas.
      - Primarily authored by Fabien Pierre-Nicolas

   This file is part of multiple-methods-qt-thread-example, a simple example to demonstrate how to use a thread running multiple methods.
   This example is explained on http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This progra is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SimulatorControlWindow_H
#define SimulatorControlWindow_H

#include <QMainWindow>
#include <QThread>
#include <qdialog.h>
#include "ScQtSimulator.h"
#include "TestNeuronPhysical.h"

namespace Ui
{
class SimulatorControlWindow;
}

class ScQtNeuron_MainWindow;

class SimulatorControlWindow : public QDialog
{
    Q_OBJECT
    friend class ScQtNeuron_AP_Window;

public:
    explicit SimulatorControlWindow(ScQtNeuron_MainWindow *parent = 0);
    ~SimulatorControlWindow();
    Ui::SimulatorControlWindow *ui;
    NeuronPhysicalTEST *MyNeuron;

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
    void on_method1Button_clicked();
    void on_method2Button_clicked();
    void on_method3Button_clicked();
    void on_resetButton_clicked();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_eventHappened();
/*    void on_stepButton_clicked();
    void on_eventButton_clicked();
    void on_spikeButton_clicked();*/
};

#endif // SimulatorControlWindow_H

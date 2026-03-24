/*
      - Started from the example by Fabien Pierre-Nicolas on http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
 */

#include <QStatusBar>
#include <QDebug>
#include <QCloseEvent>
#include "SimulatorControlWindow.h"
#include "ScQtNeuron_MainWindow.h"
#include "ui_SimulatorControlWindow.h"
#include "Utils.h"

SimulatorControlWindow::SimulatorControlWindow( NeuronPhysical *Neuron, ScQtNeuron_MainWindow *parent) :
    QDialog(parent),
    m_Neuron(Neuron),
    ui(new Ui::SimulatorControlWindow),m_parent(parent)
{
    ui->setupUi(this);
    QString message = tr("A context menu is available by right-clicking");
    parent_Get()->statusBar()->showMessage(message, 1200);

    setWindowTitle(tr("Neuron-ScQt simulator control dialog"));
    setMinimumSize(360, 280);
    resize(500, 300);
    move(600,800);
    // The thread and the simulator are created in the constructor so it is always safe to delete them.
    thread = new QThread();
    parent_Get()->m_Simulator = new ScQtSimulator();
    parent_Get()->m_Simulator->moveToThread(thread);
    connect(thread, SIGNAL(started()), parent_Get()->m_Simulator, SLOT(mainLoop()));
    connect(parent_Get()->m_Simulator, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    qDebug()<<"Starting thread in Thread "<<this->QObject::thread()->currentThreadId();
    thread->start();
    ui->SimulatedTime->setText("Help");
    connect(parent_Get()->m_Simulator, SIGNAL(valueChanged(QString)), ui->label, SLOT(setText(QString)));
    connect(parent_Get()->m_Simulator,SIGNAL(eventHappened()), this, SLOT(on_eventHappened()));
}

SimulatorControlWindow::~SimulatorControlWindow()
{
    parent_Get()->m_Simulator->abort();
    thread->wait();
    qDebug()<<"Deleting thread and simulator in Thread "<<this->QObject::thread()->currentThreadId();
    delete thread;
    delete parent_Get()->m_Simulator;
    delete ui;
}

void SimulatorControlWindow::on_resetButton_clicked()
{
    qDebug()<<"Reset simulator in Thread "<<this->QObject::thread()->currentThreadId();
}

void SimulatorControlWindow::on_eventHappened()
{
    qDebug()<<"Event happened @ "<< 1000*sc_time_stamp().to_seconds() << ":" << m_Neuron->MembraneAbsolutePotential_Get();
    ui->SimulatedTime->setText(QString(sc_time_String_Get(sc_core::sc_time_stamp()).c_str()));
    ui->timeUser->setText(QString(time_String_Get(parent_Get()->m_Simulator->userTime_Get(),CLOCK_TIME_UNIT_S,1,7).c_str()));
    ui->timeSystem->setText(QString(time_String_Get(parent_Get()->m_Simulator->systemTime_Get()/1000.,CLOCK_TIME_UNIT_S,2,7).c_str()));
//        QString message = tr("An event happened");
//        parent_Get()->statusBar()->showMessage(message, 1200);
}
void SimulatorControlWindow::on_startButton_clicked()
{
    qDebug()<<"Start simulator in Thread "<<this->QObject::thread()->currentThreadId();
    //          usleep( 100000 * 4);//;simWdw->intervalSpin->value() );

//    sc_process_handle T =  sc_get_current_process_handle();
    QString message = tr("Start button clicked");
    parent_Get()->statusBar()->showMessage(message, 1200);

}
void SimulatorControlWindow::on_stopButton_clicked()
{
    qDebug()<<"Stop simulator in Thread "<<this->QObject::thread()->currentThreadId();
    QString message = tr("Stop button clicked");
    parent_Get()->statusBar()->showMessage(message, 1200);
}
/*void SimulatorControlWindow::on_stepButton_clicked()
{
    qDebug()<<"Step simulator in Thread "<<this->QObject::thread()->currentThreadId();
}
*/
/*void SimulatorControlWindow::on_eventButton_clicked()
{
    qDebug()<<"Event simulator in Thread "<<this->QObject::thread()->currentThreadId();
}
void SimulatorControlWindow::on_spikeButton_clicked()
{
    qDebug()<<"Spike simulator in Thread "<<this->QObject::thread()->currentThreadId();
}*/
void SimulatorControlWindow::on_method1Button_clicked()
{
    parent_Get()->m_Simulator->requestMethod(ScQtSimulator::Method_SingleSteps);
    QString message = tr("Method1 requested");
    parent_Get()->statusBar()->showMessage(message, 1200);
/*    std::cerr << ui->continuousMode->isChecked()<< ui->eventMode->isChecked()<< ui->timeMode->isChecked()
              << ui->stepMode->isChecked() << ui->spikeMode->isChecked()<< "\n";*/
}

void SimulatorControlWindow::on_method2Button_clicked()
{
    parent_Get()->m_Simulator->requestMethod(ScQtSimulator::Method_SimulatedTime);
    QString message = tr("Method2 requested");
    parent_Get()->statusBar()->showMessage(message, 1200);
}

void SimulatorControlWindow::on_method3Button_clicked()
{
    parent_Get()->m_Simulator->requestMethod(ScQtSimulator::Method3);
    QString message = tr("Method3 requested");
    parent_Get()->statusBar()->showMessage(message, 1200);
}

//#if 0
// Pass responsibility of closing to the main window
void SimulatorControlWindow::closeEvent(QCloseEvent *event)
{
/*    QMessageBox::warning(this, tr("Application"),
                         tr("This window can be closed only by the application"),
                         QMessageBox::Yes );*/
    event->ignore();
}
//#endif

#include "moc_SimulatorControlWindow.cpp"

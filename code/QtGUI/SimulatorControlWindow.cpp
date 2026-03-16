/*
      - Started from the example by Fabien Pierre-Nicolas on http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
 */

#include "SimulatorControlWindow.h"
#include "ScQtNeuron_MainWindow.h"
//#include "ui_SimulatorControlWindow.h"
#include "ui_SimulatorControlWindow.h"
#include <QStatusBar>
#include <QDebug>
#include <QCloseEvent>

SimulatorControlWindow::SimulatorControlWindow(ScQtNeuron_MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::SimulatorControlWindow),m_parent(parent)
{
    ui->setupUi(this);
//    QString message = tr("A context menu is available by right-clicking");
//    parent->statusBar()->showMessage(message, 1200);

    setWindowTitle(tr("Neuron-ScQt simulator control dialog"));
    setMinimumSize(160, 160);
    resize(480, 320);
    move(600,800);
    // The thread and the simulator are created in the constructor so it is always safe to delete them.
    thread = new QThread();
    MyNeuron = new NeuronPhysicalTEST("NeuronPhysical");
    parent->m_Simulator = new ScQtSimulator();
    parent->m_Simulator->moveToThread(thread);
    connect(thread, SIGNAL(started()), parent->m_Simulator, SLOT(mainLoop()));
    connect(parent->m_Simulator, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    qDebug()<<"Starting thread in Thread "<<this->QObject::thread()->currentThreadId();
    thread->start();
    ui->SimulatedTime->setText("Help");
//    ui->SimulatedTimeLCD->set
    connect(parent->m_Simulator, SIGNAL(valueChanged(QString)), ui->label, SLOT(setText(QString)));
    connect(parent->m_Simulator,SIGNAL(eventHappened()), this, SLOT(on_eventHappened()));
}

SimulatorControlWindow::~SimulatorControlWindow()
{
    m_parent->m_Simulator->abort();
    thread->wait();
    qDebug()<<"Deleting thread and simulator in Thread "<<this->QObject::thread()->currentThreadId();
    delete thread;
    delete m_parent->m_Simulator;
;

    delete ui;
}

void SimulatorControlWindow::on_resetButton_clicked()
{
    qDebug()<<"Reset simulator in Thread "<<this->QObject::thread()->currentThreadId();
}

void SimulatorControlWindow::on_eventHappened()
{
        qDebug()<<"Event happened @ "<< 1000*sc_time_stamp().to_seconds() << ":" << MyNeuron->MembraneAbsolutePotential_Get();
    ui->SimulatedTime->setText("123.45");
}
void SimulatorControlWindow::on_startButton_clicked()
{
    qDebug()<<"Start simulator in Thread "<<this->QObject::thread()->currentThreadId();
    //          usleep( 100000 * 4);//;simWdw->intervalSpin->value() );

    sc_process_handle T =  sc_get_current_process_handle();
 //   qDebug()<< T;

}
void SimulatorControlWindow::on_stopButton_clicked()
{
    qDebug()<<"Stop simulator in Thread "<<this->QObject::thread()->currentThreadId();
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
    m_parent->m_Simulator->requestMethod(ScQtSimulator::Method1);
/*    std::cerr << ui->continuousMode->isChecked()<< ui->eventMode->isChecked()<< ui->timeMode->isChecked()
              << ui->stepMode->isChecked() << ui->spikeMode->isChecked()<< "\n";*/
}

void SimulatorControlWindow::on_method2Button_clicked()
{
    m_parent->m_Simulator->requestMethod(ScQtSimulator::Method1);
}

void SimulatorControlWindow::on_method3Button_clicked()
{
    m_parent->m_Simulator->requestMethod(ScQtSimulator::Method1);
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

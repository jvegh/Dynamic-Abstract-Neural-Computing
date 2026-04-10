/*
      - Started from the example by Fabien Pierre-Nicolas on http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
 */

#include <QStatusBar>
#include <QDebug>
#include <QCloseEvent>
#include "SimulatorControlWindow.h"
#include "ui_SimulatorControlWindow.h"
#include "ScQtNeuron_MainWindow.h"
#include "Utils.h"

SimulatorControlWindow::SimulatorControlWindow( NeuronPhysical *Neuron, ScQtNeuron_MainWindow *parent) :
    QDialog(parent),
    m_Neuron(Neuron),
    ui(new Ui::SimulatorControlWindow),m_parent(parent)
{
    ui->setupUi(this);
    // Set up Slider 1
    ui->Slider1->setMinimum(3);
    ui->Slider1->setMaximum(17);
    ui->Slider1->setValue(5);
    ui->Slider1Value->setText(QString::number(ui->Slider1->value()));
    QObject::connect(ui->Slider1, &QSlider::valueChanged, this, [=] () {
        (ui->Slider1Value->setText(QString::number(ui->Slider1->value())));
    });
    // Set up Slider 2
    ui->Slider2->setMinimum(15);
    ui->Slider2->setMaximum(50);
    ui->Slider2->setValue(23);
    ui->Slider2Value->setText(QString::number(ui->Slider2->value()));
    QObject::connect(ui->Slider2, &QSlider::valueChanged, this, [=] () {
        (ui->Slider2Value->setText(QString::number(ui->Slider2->value(),'f',2)));
    });

    setWindowTitle(tr("Neuron-ScQt simulator control dialog"));
    setMinimumSize(360, 300);
//    resize(500, 300);
    move(600,800);
    ui->StepNumberBox->setRange(1,100);
    ui->StepTimeBox->setRange(10,1000);
    //
    // The thread and the simulator are created in the constructor so it is always safe to delete them.
    //
    thread = new QThread();
    parent_Get()->m_Simulator = new ScQtSimulator();
    parent_Get()->m_Simulator->moveToThread(thread);
    connect(thread, SIGNAL(started()), parent_Get()->m_Simulator, SLOT(mainLoop()));
    connect(parent_Get()->m_Simulator, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);

    // Receive SC event from the simulator
//    connect(parent_Get()->m_Simulator,  SIGNAL(NeuralBreakpoint), this, SLOT(on_scEventHappened()));
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
    //    QString message = tr("Method3 requested");
    parent_Get()->m_Simulator->TimesReset();  // Reset the three times
    m_Neuron->Initialize_Do();  // Re-initialize the SystemC part
    parent_Get()->VoltageWindow_Get()->ResetDisplay();
    on_eventHappened();
    qDebug()<<"Reset simulator in Thread "<<this->QObject::thread()->currentThreadId();
}

void SimulatorControlWindow::on_eventHappened()
{
//    qDebug()<<"Event happened @ "<< 1000*sc_time_stamp().to_seconds() << ":" << m_Neuron->MembraneAbsolutePotential_Get();
    ui->SimulatedTime->setText(QString(sc_time_String_Get(parent_Get()->m_Simulator->scTime_Get()).c_str()));
    ui->timeUser->setText(QString(time_String_Get(parent_Get()->m_Simulator->userTime_Get(),CLOCK_TIME_UNIT_S,1,7).c_str()));
    ui->timeSystem->setText(QString(time_String_Get(parent_Get()->m_Simulator->systemTime_Get()/1000.,CLOCK_TIME_UNIT_S,2,7).c_str()));
}


void SimulatorControlWindow::on_startButton_clicked()
{
    if(ui->timeMode->isChecked())
    {
        parent_Get()->m_Simulator->NoOfSteps_Set(ui->StepNumberBox->value());
    }
    if(ui->stepMode->isChecked())
    {
        parent_Get()->m_Simulator->TimeOfSteps_Set(sc_core::sc_time(ui->StepTimeBox->value(),sc_core::SC_US));
    }
    parent_Get()->m_Simulator->requestMethod(ScQtSimulator::Method_SingleSteps);
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

void SimulatorControlWindow::on_method2Button_clicked()
{
    parent_Get()->m_Simulator->requestMethod(ScQtSimulator::Method_SimulatedTime);
    QString message = tr("Method2 requested");
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

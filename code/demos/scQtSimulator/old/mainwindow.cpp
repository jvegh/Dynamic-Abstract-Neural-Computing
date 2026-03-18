
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
//#include "scQtModule.h"
#include "scQtThread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // The scQtThread and the scQtSimulator are created in the constructor so it is always safe to delete them.
    thread = new scQtThread("myThread");
    m_scQtSimulator = new scQtSimulator("mySimulator");
    m_scQtSimulator->moveToThread(thread);
    connect(m_scQtSimulator, SIGNAL(valueChanged(QString)), ui->label, SLOT(setText(QString)));
    connect(m_scQtSimulator, SIGNAL(workRequested()), thread, SLOT(start()));
    connect(m_scQtSimulator, SIGNAL(resetRequested()), m_scQtSimulator, SLOT(doReset()));
    connect(thread, SIGNAL(started()), m_scQtSimulator, SLOT(doWork()));
//    connect(thread, SIGNAL(reseted()), m_scQtSimulator, SLOT(doReset()));
    connect(m_scQtSimulator, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);


/*
 *  //    sc_start( SC_ZERO_TIME );// Run the initialization phase to create pending activity
    // connect(m_scQtSimulator, SIGNAL(valueChanged(QString)), ui->label, SLOT(setText(QString)));
    connect(m_scQtThread, SIGNAL(runChanged(QString,uint)),this,SLOT(on_threadShot(QString,uint)));
//    connect(m_scQtSimulator->A, SIGNAL(runChanged(QString,uint)),this,SLOT(on_threadShot(QString,uint)));

 * //    connect(thread, SIGNAL(eventChanged(sc_core::sc_module *M, void* i)),this,SLOT(on_threadEvent(sc_core::sc_module *M, void* i)));
//    connect(thread->A, SIGNAL(scQtEvent(scQtModule, uint)),this,SLOT(this->on_scQtEvent(scQtModule, uint)));
//    connect(A, SIGNAL(scQtEvent(scQtModule*, uint)),this,SLOT(on_scQtEvent(scQtModule*, uint)));
 //   connect(thread->A, &scQtModule::scQtEvent,this,&MainWindow::on_scQtEvent);
//    connect(thread->A, &scQtModule::scQtEvent,this,&MainWindow::on_scQtEvent);
*/
     connect(m_scQtSimulator, SIGNAL(workRequested()), thread, SLOT(start()));
//    connect(thread, SIGNAL(started()), m_scQtSimulator, SLOT(dowork()));
     connect(thread, &scQtThread::started, m_scQtSimulator, &scQtSimulator::doWork);
/*
//    connect(thread, SIGNAL(finished()), m_scQtSimulator, SLOT(finish()));
    connect(m_scQtSimulator, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
*/
}

MainWindow::~MainWindow()
{
    /*
    if(thread->isRunning())
    {
        thread->exit();
        thread->QThread::wait();
    }
    qDebug()<<"Deleting scQtThread and scQtSimulator in thread "<<this->QObject::thread()->currentThreadId();
    delete thread;
    delete m_scQtSimulator;
*/
    delete ui;
}


void MainWindow::on_startButton_clicked()
{
    // To avoid having two threads running simultaneously, the previous thread is interrupted.
    if(thread->isRunning())
        thread->requestInterruption();
    thread->QThread::wait(); // If the thread is not running, this will immediately return.

    m_scQtSimulator->requestWork();
}

void MainWindow::on_resetButton_clicked()
{
    // To avoid having two threads running simultaneously, the previous thread is interrupted.
    if(thread->isRunning())
        thread->requestInterruption();
    thread->QThread::wait(); // If the thread is not running, this will immediately return.

    m_scQtSimulator->requestReset();
}

void MainWindow::on_threadShot(QString S, uint i)
{
    qDebug()<<"received scQtThread shot in thread "<<this->QObject::thread()->currentThreadId()<< " " << S << " " << i;
}

#if 0
void MainWindow::on_scQtEvent(//scQtModule *M,
                              uint i)
{
    qDebug()<<"Received event from " //<< M
             << "@" << i << " in main from "  << " @ " << this->QObject::thread()->currentThreadId();
}
#endif

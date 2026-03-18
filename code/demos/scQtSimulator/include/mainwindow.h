
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
//#include "scQtThread.h"
#include "scQSimulator.h"
namespace Ui
{
class MainWindow;
}


class WorkerThread : public QThread
{
    Q_OBJECT
    void run() override {
        QString result;
        /* ... here is the expensive or blocking operation ... */
        emit resultReady(result);
    }
signals:
    void resultReady(const QString &s);
};
/*
void MyObject::startWorkInAThread()
{
    WorkerThread *workerThread = new WorkerThread(this);
    connect(workerThread, &WorkerThread::resultReady, this, &MyObject::handleResults);
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    Ui::MainWindow *ui;
    /**
     * @brief scQtThread object which will let us manipulate the running thread
        and the SystemC part
     */

    //scQtThread *thread;
    /**
     * @brief Object which contains methods that should be runned in another thread
     */
    scQSimulator *m_QSimulator;

private slots:
    void on_startButton_clicked();
    void on_resetButton_clicked();
    void on_threadShot(QString S, uint i);
/*    void on_scQtEvent(//scQtModule *M,
                      uint i);
 //   void on_threadEvent(sc_core::sc_object *M, uint i);
*/
};

#endif // MAINWINDOW_H

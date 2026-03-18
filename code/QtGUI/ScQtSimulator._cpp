/*
      - Started from the example by Fabien Pierre-Nicolas on http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
 */

#include <QTimer>
#include <QEventLoop>

#include <QThread>
#include <QDebug>
#include <systemc>
#include "ScQtSimulator.h"

ScQtSimulator::ScQtSimulator(QObject *parent) :
    QObject(parent)
{
    _abort = false;
    _interrupt = false;
    sc_start( sc_core::SC_ZERO_TIME);
}

void ScQtSimulator::requestMethod(ScQtSimulator::Method method)
{
//    qDebug()<<"Request ScQtSimulator Method"<<method<<" in Thread "<<thread()->currentThreadId();
    QMutexLocker locker(&mutex);
    _interrupt = true;
    _method = method;
    condition.wakeOne();
}

void ScQtSimulator::abort()
{
    qDebug()<<"Request ScQtSimulator aborting in Thread "<<thread()->currentThreadId();
    QMutexLocker locker(&mutex);
    _abort = true;
    condition.wakeOne();
}

void ScQtSimulator::doMethod1()
{

    mutex.lock();
//**    sc_start( sc_core::SC_ZERO_TIME);
    sc_core::sc_time ThisTime = sc_core::sc_time_to_pending_activity();
//    sc_start( sc_core::sc_time_to_pending_activity() );
//        std::cerr << ThisTime.to_string() << "\n";
     sc_core::sc_start( ThisTime);

//    qDebug()<< ThisTime.to_string();
    mutex.unlock();

    qDebug()<<"Starting Method1 in Thread "<<thread()->currentThreadId();
    emit eventHappened();

/*
    for (int i = 0; i < 10; i ++) {

        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug()<<"Interrupting Method1 in Thread "<<thread()->currentThreadId();
            break;
        }

        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

        emit valueChanged(QString::number(i));
    }
*/
}

void ScQtSimulator::doMethod2()
{
    qDebug()<<"Starting Method2 in Thread "<<thread()->currentThreadId();

    for (int i = 0; i < 20; i ++) {

        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug()<<"Interrupting Method2 in Thread "<<thread()->currentThreadId();
            break;
        }

        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

        emit valueChanged(QString::number(i));
    }
}

void ScQtSimulator::doMethod3()
{
    qDebug()<<"Starting Method3 in Thread "<<thread()->currentThreadId();

    for (int i = 0; i < 30; i ++) {

        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        sc_start(sc_core::SC_ZERO_TIME);
        mutex.unlock();

        if (abort || interrupt) {
            qDebug()<<"Interrupting Method3 in Thread "<<thread()->currentThreadId();
            break;
        }

        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
         emit valueChanged(QString::number(i));
    }
}

void ScQtSimulator::mainLoop()
{
    qDebug()<<"Starting ScQtSimulator mainLoop in Thread "<<thread()->currentThreadId();

    forever {

        mutex.lock();
        if (!_interrupt && !_abort) {
            condition.wait(&mutex);
        }
        _interrupt = false;

        if (_abort) {
            qDebug()<<"Aborting ScQtSimulator mainLoop in Thread "<<thread()->currentThreadId();
            mutex.unlock();
            emit finished();
            return;
        }

        Method method = _method;
        mutex.unlock();

        switch(method) {
        case Method1:
            doMethod1();
            break;
        case Method2:
            doMethod2();
            break;
        case Method3:
            doMethod3();
            break;
        }
    }
}
// Just to avoid undefined_reference_to_vtable
#include "moc_ScQtSimulator.cpp"

/** @file ScQtSimulator.h
 *  @brief A semi-general purpose simulator for SystemC task, using Qt's stuff
 *  Ideas taken from  http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
 *  https://www.researchgate.net/publication/228972213_gSysC_A_graphical_front_end_for_SystemC
 *  and https://github.com/mortbopet/Ripes
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 *  Last edited 2026.04.19
*/

//#include <systemc>
// Maybe a timer is not needed
//#include <QTimer>
#include <QEventLoop>
#include <QThread>
#include <QDebug>
#include <unistd.h>

#include "ScQtSimulator.h"

ScQtSimulator::ScQtSimulator(QObject *parent) :
    QObject(parent)
{
    _abort = false;
    _interrupt = false;
    sc_start( sc_core::SC_ZERO_TIME);
    TimesReset();
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

#if 0
void ScQtSimulator::doSimulationSteps()
{
    sc_core::sc_time BeginTime = sc_core::sc_time_stamp();
        sc_core::sc_time ThisTime = sc_core::sc_time_to_pending_activity(); // Make a single simulation step
            BENCHMARK_TIME_BEGIN(&m_system_t,&m_system_x);    // Begin benchmarking here
        sc_core::sc_start( ThisTime);                      // Measure processor time of simulating step
            BENCHMARK_TIME_END(&m_system_t,&m_system_x,&m_system_s);   // End benchmarking here
        emit eventHappened();
         uint64_t DiffTime = (sc_core::sc_time_stamp()-BeginTime).to_seconds()*1000.*1000.*m_SlowFactor;
        usleep(DiffTime);  // Now display refresh can start in the other thread
/*    if (_abort || _interrupt) {
        qDebug()<<"Interrupted doSimulationSteps in Thread "<<thread()->currentThreadId();
    }*/
}
#endif
void ScQtSimulator::doSimulationSteps()
{    uint64_t DiffTime;
    do
    {
        sc_core::sc_time BeginTime = sc_core::sc_time_stamp();
        sc_core::sc_time ThisTime = sc_core::sc_time_to_pending_activity(); // Make a single simulation step
            BENCHMARK_TIME_BEGIN(&m_system_t,&m_system_x);    // Begin benchmarking here
        sc_core::sc_start( ThisTime);                      // Measure processor time of simulating step
            BENCHMARK_TIME_END(&m_system_t,&m_system_x,&m_system_s);   // End benchmarking here
        DiffTime = (sc_core::sc_time_stamp()-BeginTime).to_seconds()*1000.*1000.*m_SlowFactor;
    }   while(!DiffTime); // Do not send event at zero time difference
        emit eventHappened();

        usleep(DiffTime);  // Now display refresh can start in the other thread
/*    if (_abort || _interrupt) {
        qDebug()<<"Interrupted doSimulationSteps in Thread "<<thread()->currentThreadId();
    }*/
}



#if 0
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
#endif

void ScQtSimulator::mainLoop()
{
 //   qDebug()<<"Starting ScQtSimulator mainLoop in Thread "<<thread()->currentThreadId();

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
        case Method_SingleSteps:
            doSimulationSteps();
            break;
#if 0
        case Method_SimulatedTime:
            doSimulatedTime();
            break;

        case Method3:
            doMethod3();
            break;
#endif
        }
    }
}
// Just to avoid undefined_reference_to_vtable
#include "moc_ScQtSimulator.cpp"

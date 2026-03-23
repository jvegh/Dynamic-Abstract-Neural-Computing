/*
      - Started from the example by Fabien Pierre-Nicolas on http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
 */

#include <systemc>
#include <QTimer>
#include <QEventLoop>
#include <QThread>
#include <QDebug>

#include "ScQtSimulator.h"

/*
In your class (or member function) define
    @code{.cpp}
chrono::steady_clock::time_point t =chrono::steady_clock::now();
std::chrono::duration< int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;
@endcode
    and use the macros as as
    @code{.cpp}
BENCHMARK_TIME_RESET(&t,&x,&s); // Reset at the very begining, say in the constructor
@endcode
    Later put the benchmarked code between macros, used as brackets
    @code{.cpp}
BENCHMARK_TIME_BEGIN(&t,&x);    // Begin the benchmarking here
your code
    BENCHMARK_TIME_END(&t,&x,&s);   // End benchmarking here
@endcode
    After using these macros, the benchmarked time values are returned:
                                               (since BEGIN) in x (nanoseconds)
                                               and the sum of all benchmarked time (since RESET) in s (nanoseconds).
                                                       Access the result as
                                                       @code{.cpp}
                                                       std::cerr  << "Simulation took " << s.count()/1000/1000. << " msec CLOCK time" << endl;
@endcode

        In an object, you can RESET in the constructor,
    in the member functions between BEGIN and END measure the
            one-time utilization, and in the destructor to read out the total benchmarked time.
        Or to benchmark (in multiple variables) execution CLOCK time  about critical sections of your code.
*/

ScQtSimulator::ScQtSimulator(QObject *parent) :
    QObject(parent)
{
    _abort = false;
    _interrupt = false;
    sc_start( sc_core::SC_ZERO_TIME);
    m_clock_time_begin = QTime::currentTime();
    // Now initialize the system time clock; reset system clock timer
    m_system_t =chrono::steady_clock::now();
    m_system_x = m_system_s = (std::chrono::duration< int64_t, nano>)0;
    BENCHMARK_TIME_RESET(&m_system_t,&m_system_x,&m_system_s); // Reset at the very beginning, say in the constructor
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
    BENCHMARK_TIME_BEGIN(&m_system_t,&m_system_x);    // Begin the benchmarking here
     sc_core::sc_start( ThisTime);                      // Measure processor time of simulating step
    BENCHMARK_TIME_END(&m_system_t,&m_system_x,&m_system_s);   // End benchmarking here
//    qDebug()<< ThisTime.to_string();
    mutex.unlock();

    qDebug()<<"Starting Method1 in Thread "<<thread()->currentThreadId();
    emit eventHappened();
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


#include "scQtThread.h"
#include <QTimer>
//#include <QEventLoop>

#include <QThread>
#include <QDebug>


scQtThread::scQtThread(sc_core::sc_module_name nm, QObject *parent) :
   QThread(parent), sc_core::sc_module(nm)
{
    typedef scQtThread SC_CURRENT_USER_MODULE; // Needed if some routines are listed in this constructor
/*     SC_THREAD(Initialize_method);
    sensitive << Initialize;
    //    dont_initialize();
    qDebug()<<"Starting Worker Thread "<<thread()->currentThreadId();
    SC_THREAD(Third_method);
    sensitive << Initialize;
    //    dont_initialize();
*/
}
/*
void scQtThread::Initialize_method()
{
     qDebug()<<"Initing Worker Thread "<<thread()->currentThreadId();
    qDebug()<< "Emitting " << Initialize.get_parent_object()->name() << " in " << thread()->currentThreadId();;
    Other.notify(10,SC_MS);
    sc_core::wait(10,SC_MS);
    Third.notify(10,SC_MS);
}

void scQtThread::Third_method()
{
    qDebug()<<"Initing Third Thread "<<thread()->currentThreadId();
    qDebug()<< "Emitting " << Third.get_parent_object()->name() << " in " << thread()->currentThreadId();;
}

scQtThread::~scQtThread()
{
    if (not sc_end_of_simulation_invoked())
        sc_stop(); //< invoke end_of_simulation
    qDebug()<<"Quiting Worker Thread "<<thread()->currentThreadId();

}
*/
void scQtThread::run()
{
/*
    while( sc_pending_activity() ) {
        qDebug()<<"Waiting SC Worker Thread "<<thread()->currentThreadId();
        sc_start( sc_time_to_pending_activity() );
 //       qDebug()<<"Receiving SC Worker Thread "<<thread()->currentThreadId();
        qDebug()<< "Emitting " << Other.get_parent_object()->name() << " in " << thread()->currentThreadId();;
        emit runChanged(Other.get_parent_object()->name(), 1);
 //       emit eventChanged(dynamic_cast <sc_core::sc_module*>(Other.get_parent_object()->get_parent_object()), thread()->currentThreadId());
    }
*/
    unsigned int index = 10;
    while(index--)
    {
        qDebug()<<"Runninging Worker Thread "<<thread()->currentThreadId();
        msleep(900);
        emit runChanged("OK", index);
    }
}


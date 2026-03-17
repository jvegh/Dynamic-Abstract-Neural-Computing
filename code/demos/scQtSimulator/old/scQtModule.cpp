
#include "scQtModule.h"
#include "scQtSimulator.h"
#include <QTimer>
//#include <QEventLoop>

#include <QDebug>

scQtModule::scQtModule(sc_core::sc_module_name nm, scQtSimulator *Sim, QObject *parent) :
   QObject(parent), sc_core::sc_module(nm)
{
    typedef scQtModule SC_CURRENT_USER_MODULE; // Needed if some routines are listed in this constructor
//    connect(scQtSimulator, &scQtSimulator::resetRequested, scQtModule, &scQtModule::);
    if(Sim)
        connect( Sim, &scQtSimulator::requestModuleReset, this, &scQtModule::resetModule);
    SC_THREAD(Initialize_method);
    sensitive << Initialize;
    //    dont_initialize();
    qDebug()<<"Starting Module "<< nm << " " << this;
    SC_THREAD(Third_method);
    sensitive << Initialize;
    //    dont_initialize();
}
void scQtModule::Initialize_method()
{
    qDebug()<< "Emitting " << name() << " Inited in " << this;
    emit runChanged(name(), 1);

    emit scQtEvent(//this,
                   1);
    Other.notify(10,SC_MS);
    sc_core::wait(10,SC_MS);
    Third.notify(10,SC_MS);
}

void scQtModule::Third_method()
{
    qDebug()<< "Emitting " << name() << " Third in " << this;
    emit runChanged("A",3);
    emit scQtEvent(//this,
                   3);
}

scQtModule::~scQtModule()
{
    qDebug()<<"Quiting module "<< name();

}

void scQtModule::resetModule()
{
    qDebug()<<"Reseting module "<< name();
}

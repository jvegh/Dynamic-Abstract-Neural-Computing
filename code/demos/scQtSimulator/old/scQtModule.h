/** @file scQtThread.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *
 *  @brief The thread to run SystemC-related ativity and inform a Qt-based GUI
  */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#ifndef SCQTMODULE_H
#define SCQTMODULE_H

#include <QObject>
#include <systemc>

class scQtSimulator;
using namespace sc_core;
//
class scQtModule : public QObject, sc_core::sc_module
{
    Q_OBJECT
private:
    sc_event Initialize, Other, Third;

public:
    explicit scQtModule(sc_core::sc_module_name nm, scQtSimulator *sim = 0, QObject *parent = 0);
    void Initialize_method();
    void Third_method();
    virtual ~scQtModule();

signals:
    void runChanged(QString Q, uint i);
    void scQtEvent(//scQtModule *M,
                   uint i);
public slots:
    void resetModule();
};

#endif // SCQTMODULE_H

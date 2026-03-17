/*
 * The idea taken from
 * https://github.com/fabienpn/simple-qt-thread-example
 */

#ifndef SQCTSIMULATOR_H
#define SQCTSIMULATOR_H

#include <QObject>
#include <QMutex>
#include <systemc>
#include "scQtModule.h"
using namespace sc_core;

//
class scQtSimulator : public QObject, sc_core::sc_module
{
    Q_OBJECT

public:
    explicit scQtSimulator(sc_core::sc_module_name nm,
                           QObject *parent = 0);
//    scQtModule *A;
    /**
     * @brief Requests the process to start
     *
     * It is thread safe as it uses #mutex to protect access to #_working variable.
     */
    void requestWork();
    /**
     * @brief Requests the process to start
     *
     * It is thread safe as it uses #mutex to protect access to #_working variable.
     */
    void requestReset();
    /**
     * @brief Requests the process to abort
     *
     * It is thread safe as it uses #mutex to protect access to #_abort variable.
     */
    void abort();

private:
    /**
     * @brief Process is aborted when @em true
     */
    bool _abort;
    /**
     * @brief @em true when Worker is doing work
     */
    bool _working;
    /**
     * @brief Protects access to #_abort
     */
    QMutex mutex;
signals:
    /**
     * @brief This signal is emitted when the Simulator request to Work
     * @sa requestWork()
     */
    void workRequested();
    /**
     * @brief This signal is emitted when the Simulator request to Work.
     * Makes a thread-safe call to
     * @sa requestWork()
     */
    void resetRequested();
    /**
     * @brief The GUI emits this signal when to reset the Simulator is requested.
     * Makes a thread-safe call to
     * @sa requestSimulatorReset()
     */
    void requestSimulatorReset();
    /**
     * @brief The simulator emits this signal when to reset the Modules is requested
     * @sa requestModuleReset()
     */
    void requestModuleReset();
    /**workRequested
     * @brief This signal is emitted when counted value is changed (every sec)
     */
    void valueChanged(const QString &value);
    /**
     * @brief This signal is emitted when process is finished (either by counting 60 sec or being aborted)
     */
    void finished();
    //    void eventChanged(sc_core::sc_module *M, void* i);
    //    void runChanged(QString Q, uint i);

public slots:
    /**
     * @brief Does the useful work
     *
     * Counts 15 sec in this example.
     * Counting is interrupted if #_aborted is set to true.
     */
    void doWork();
    /**
     * @brief resetSimulation
     *
     * The SystemC must not be restarted, by design
     * The used modules must implement a slot resetReceived()
     * and to the appropriate module reset operation
     */
    void doReset();
 //   void start();
 //   void finish();
signals:
//    void eventChanged(sc_core::sc_module *M, void* i);
//    void runChanged(QString Q, uint i);
};

#endif // SQCTSIMULATOR_H

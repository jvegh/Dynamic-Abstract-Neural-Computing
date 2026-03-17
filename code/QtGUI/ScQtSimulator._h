/*
      - Started from the example by Fabien Pierre-Nicolas on http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
  */

#ifndef SCQTSIMULATOR_H
#define SCQTSIMULATOR_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>


class ScQtSimulator : public QObject
{
    Q_OBJECT

public:
    explicit ScQtSimulator(QObject *parent = 0);
    /**
     * @brief This enum describes the various available methods
     */
    enum Method {
        Method1 = 1,
        Method2 = 2,
        Method3 = 3

    };
    /**
     * @brief Requests for the method @em method to be executed
     *
     * This method will defines #_method and set #_abort to interrupt current method.
     * It is thread safe as it uses #mutex to protect access to #_method and #_abort variable.
     */
    void requestMethod(Method method);
    /**
     * @brief Requests the process to abort
     *
     * It is thread safe as it uses #mutex to protect access to #_abort variable.
     */
    void abort();

private:
    /**
     * @brief Currently requested method
     */
    Method _method;
    /**
     * @brief Process is aborted when @em true
     */
    bool _abort;
    /**
     * @brief Current method is interrupted when @em true
     */
    bool _interrupt;
    /**
     * @brief Protects access to #_abort
     */
    QMutex mutex;
    /**
     * @brief Condition used to wait for a new request to be called in the #mainLoop()
     */
    QWaitCondition condition;
    /**
     * @brief 1st method which could be called
     *
     * Counts 10 sec in this example.
     * Counting is interrupted if #_abort or #_interrupt is set to true.
     */
    void doMethod1();
    /**
     * @brief 2nd method which could be called
     *
     * Counts 20 sec in this example.
     * Counting is interrupted if #_abort or #_interrupt is set to true.
     */
    void doMethod2();
    /**
     * @brief 3rd method which could be called
     *
     * Counts 30 sec in this example.
     * Counting is interrupted if #_abort or #_interrupt is set to true.
     */
    void doMethod3();

signals:
    /**
     * @brief This signal is emitted when counted value is changed (every sec)
     */
    void valueChanged(const QString &value);
    /**
     * @brief This signal is emitted when  simulator returns from SystemC
     * @param T the simulated time of the event
     */
    void eventHappened(void);

    /**
     * @brief This signal is emitted when process is finished (either by counting 60 sec or being aborted)
     */
    void finished();

public slots:
    /**
     * @brief Waits for a method to be requested then executes it, then waits again
     */
    void mainLoop();
};

#endif // SCQTSIMULATOR_H

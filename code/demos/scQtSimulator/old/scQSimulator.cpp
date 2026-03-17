#if 0

/** @file AP_Conceptual.cpp
 *  @brief The main file for the demo to generate a conceptual action potential
 *  SystemC-based program
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
// All systemc modules should include systemc.h header file
#include <systemc>
#include "scQtSimulator.h"

//#define MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
//#include "MacroTimeBenchmarking.h"    // Must be after the define to have its effect
#define MAX_CLOCK_CYCLES 10000
using namespace std; using namespace sc_core;
#define PROJECT_NAME "ScQtAND"
#define PROJECT_VERSION "0.0.7"

#define PU_InitialDelayTime1 sc_core::sc_time(63,SC_US)
#define PU_InitialDelayTime sc_core::sc_time(100,SC_US)
#define PU_SynchronizeTime1 sc_core::sc_time(80,SC_US) // During "Relaxing", before initial
#define PU_SynchronizeTime2 sc_core::sc_time(120,SC_US) // During "Delivering"
#define PU_InputTime1       sc_core::sc_time(190,SC_US) // Normal input
#define PU_InputTime2       sc_core::sc_time(197,SC_US) // Normal input
#define PU_InputTime3       sc_core::sc_time(205,SC_US) // Normal input
#define PU_InputTime4       sc_core::sc_time(213,SC_US) // Normal input

#include "NeuronConceptual.h"
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug event for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

string simulation_name =  string(PROJECT_NAME) + "_Conceptual" + " V" + string(PROJECT_VERSION);

//#include <pthread.h>

#include <unistd.h>
#include "scNeuronSimulator.h"
#if 0
using namespace sc_core;

class ThreadSafeEventIf : public sc_interface {
    virtual void notify(sc_time delay = SC_ZERO_TIME) = 0;
    virtual const sc_event &default_event(void) const = 0;
protected:
    virtual void update(void) = 0;
};

class ThreadSafeEvent : public sc_prim_channel, public ThreadSafeEventIf {
public:
    ThreadSafeEvent(const char *name = ""): event(name) {}

    void notify(sc_time delay = SC_ZERO_TIME) {
        this->delay = delay;
        async_request_update();
    }

    const sc_event &default_event(void) const {
        return event;
    }
protected:
    virtual void update(void) {
        event.notify(delay);
    }
    sc_event event;
    sc_time delay;
};

class scSimulator : sc_module
{
public:
    scSimulator(sc_module_name nm):
        sc_module(nm)
    {
        SC_HAS_PROCESS(scSimulator);
        SC_THREAD(main);
        SC_METHOD(eventOneTriggered);
        sensitive << eventOne;
        dont_initialize();
        SC_METHOD(eventTwoTriggered);
        sensitive << eventTwo;
        dont_initialize();
        SC_METHOD(eventInitialized);
        sensitive << eventTwo;
        dont_initialize();
    }
private:
    void main() {
        std::cout << "Beginning" << std::endl;
        usleep(2 * 1000 * 1000); // Just for the example, event is added to pending events during this sleep
         std::cout << "Done" << std::endl;
    }

    void eventOneTriggered() {
        std::cout << "Got event #1" << std::endl;
    }
    void eventTwoTriggered() {
        std::cout << "Got event #2" << std::endl;
    }
public:
    void eventInitialized() {
        std::cout << "Inited" << std::endl;
    }

public:
    ThreadSafeEvent eventOne;
    ThreadSafeEvent eventTwo;
    ThreadSafeEvent eventInited;
};
//scSimulator* MySym;

void *externalHostThread(void *arg) {
    usleep(1 * 1000 * 1000); // Just for the example
    scSimulator* Sim = (scSimulator*)(arg);
    usleep(100 * 1000); // Just for the example
    Sim->eventOne.notify();
    usleep(100 * 1000); // Just for the example
    std::cout << "Event #1 notified from an external host thread" << std::endl;
    Sim->eventTwo.notify();
    usleep(100 * 1000); // Just for the example
    std::cout << "Event #2 notified from an external host thread" << std::endl;
    return NULL;
}

#endif

//scNeuronSimulator Sim("scSim");

class NeuronConceptualDemo : public NeuronConceptual
{
public:
    /*!
     * \brief Creates a demo-purpose conceptual neuron unit
     * @param nm the SystemC name of the module
     */
    NeuronConceptualDemo(sc_core::sc_module_name nm ):
        NeuronConceptual(nm){
        Sim = new scNeuronSimulator("mySim");
    }

    virtual ~NeuronConceptualDemo(void)    {}// Must be overridden

    //! Activity for intializing the unit
    virtual void Initialize_Do();
    scNeuronSimulator* Sim ;
    EVENT_GenComp_type EVENT_GenComp;
};

void NeuronConceptualDemo::
    Initialize_Do()
{
    NeuronConceptual::Initialize_Do();
    sc_core::sc_time BaseTime = sc_core::sc_time_stamp();
        // We rely on the default initialization; no initialization at the beginning
    EVENT_GenComp.InputReceived.notify(PU_InputTime1);
    DEBUG_SC_EVENT(name(),"Schd 'InputReceived' @" <<sc_time_String_Get(PU_InputTime1+BaseTime));
    Sim->EVENT_GenComp.Initialize.notify();
    Sim->EVENT_GenComp.ComputingBegin.notify(10,SC_US);
    Sim->EVENT_GenComp.InputReceived.notify(sc_time(195,SC_US));
}

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTimer>

int sc_main(int argc, char* argv[]) {
    std::cout << simulation_name << " started" << std::endl;
    QCoreApplication a(argc, argv);
//    qsrand(0);
    auto threadTest = std::function<void ()> ([&]() {
        if(QCoreApplication::instance()->thread() == QThread::currentThread()) {
            qDebug() << "UI Thread in use";
        } else {
            qDebug() << "Worker thread in use";
        }

        for (int var = 0; var < 10; ++var) {
//            int r = qrand() % 100;
            int r=var;
            QThread::msleep(r);
            qDebug() << "[Worker Thread " << QThread::currentThreadId() << "] " << r;
        }
    });
    QThread *thread = QThread::create(threadTest);
    thread->start();

    int var = 0;
    std::function<void ()> timerTest;
    timerTest = [&](){
        int r = 47;
//       int r = qrand() % 100;
        qDebug() << "[UI Thread " << QThread::currentThreadId() << "] " << r;
        ++var;
        if (var < INT_MAX)
            QTimer::singleShot(r, timerTest);
    };
    int r = 10;
//    int r = qrand() % 100;
    QTimer::singleShot(r, timerTest);

    return a.exec();
#if 0
 /*   scQtSimulator *mySimulator = new scQtSimulator();
    NeuronConceptualDemo NPD("NeuronPhysical");
 */
 *//   NPD.ObservingBit_Set(gcob_ObserveValueChanged,1);
//    sc_start(MAX_CLOCK_CYCLES ,SC_US);
    sc_start(SC_ZERO_TIME);
    while(sc_pending_activity())
    {
        std::cerr << "@" << sc_time_stamp().to_seconds()*1000. << " in main\n";
        sc_start(sc_time_to_pending_activity());

    }
#endif //0
 #if 0
    pthread_t thread;
    pthread_create(&thread, NULL, externalHostThread, &Sim);
    usleep(2 * 1000 * 1000); // Just for the example

    sc_start();
    NeuronConceptualDemo NPD("NeuronPhysical");
       sc_start(MAX_CLOCK_CYCLES ,SC_US);
 /*   wait(NPD.EVENT_GenComp.InputReceived); // Send in Initialize_Do()
       NPD.EVENT_GenComp.InputReceived.notify(PU_InputTime2-PU_InputTime1);
            DEBUG_SC_EVENT(NPD.name(),"Schd 'InputReceived' @" <<sc_time_String_Get(PU_InputTime2));
    wait(NPD.EVENT_GenComp.InputReceived);
            EXPECT_EQ(0+2*MembraneInputIncrease,NPU.MembraneRelativePotential_Get());
    NPU.EVENT_GenComp.InputReceived.notify(PU_InputTime3-PU_InputTime2);
            DEBUG_SC_EVENT(NPU.name(),"Schd 'InputReceived' @" <<sc_time_String_Get(PU_InputTime3));
    wait(NPU.EVENT_GenComp.InputReceived);
            // After 3 inputs plus one discharge
            EXPECT_EQ(0+3*MembraneInputIncrease-MembraneRelaxDischarge,NPU.MembraneRelativePotential_Get());
    NPU.EVENT_GenComp.InputReceived.notify(PU_InputTime4-PU_InputTime3);
            DEBUG_SC_EVENT(NPU.name(),"Schd 'InputReceived' @" <<sc_time_String_Get(PU_InputTime4));
    wait(NPU.EVENT_GenComp.InputReceived);
            EXPECT_EQ(0+4*MembraneInputIncrease-2*MembraneRelaxDischarge,NPU.MembraneRelativePotential_Get());

    EXPECT_EQ(PU_InputTime4,sc_core::sc_time_stamp());
     wait(NPU.EVENT_GenComp.DeliveringBegin);
    EXPECT_EQ(4,NPU.NoOfInputsReceived_Get());
    EXPECT_EQ(PU_InputTime1+3*HEARTBEAT_TIME_DEFAULT,sc_core::sc_time_stamp());
    EXPECT_TRUE(NPU.MembraneRelativePotential_Get()>ThresholdPotential);

    EXPECT_EQ( gcsm_Delivering, NPU.StageFlag_Get());  // The unit is relaxing; initially
     wait(NPU.EVENT_GenComp.DeliveringEnd);

    NPU.EVENT_GenComp.InputReceived.notify(5*HEARTBEAT_TIME_DEFAULT);
     DEBUG_SC_EVENT(NPU.name(),"Schd 'InputReceived' @" <<sc_time_String_Get(PU_InputTime1-BaseTime));
     // 6 heartbeats later there appears a new input in the   */
    /*
    wait(NPU.EVENT_GenComp.RelaxingBegin);
    EXPECT_EQ(sc_time_stamp(),NPU.scRelaxingBeginTime_Get());
    EXPECT_EQ(NPU.MembraneRelativePotential_Get(),ThresholdPotential-1);
     // Check if discharge in relaxing works
    // Check if memory works (receiving input in non-RestingMembranePotential works
    wait(NPU.EVENT_GenComp.InputReceived);
//    wait(NPU.EVENT_GenComp.ComputingBegin); Just imitated
    // Idle period will only change at 'ComputingBegin'
    EXPECT_EQ(sc_time(PU_InputTime1),NPU.scIdlePeriod_Get());
    NPU.EVENT_GenComp.InputReceived.notify(6*HEARTBEAT_TIME_DEFAULT);
    DEBUG_SC_EVENT(NPU.name(),"Schd 'InputReceived' @" <<sc_time_String_Get(PU_InputTime1-BaseTime));
    // A new input at non-resting potential received
    EXPECT_EQ(NPU.MembraneRelativePotential_Get(),ThresholdPotential+6 -5-6);
    wait(50*HEARTBEAT_TIME_DEFAULT);
    // A self-consistency check
    EXPECT_EQ(sc_time(30,SC_US),NPU.scResultPeriod_Get());
    EXPECT_EQ(sc_time(1.281,SC_MS),NPU.scRelaxingBeginTime_Get());
    // Check if relaxing completed
    EXPECT_EQ(NPU.MembraneRelativePotential_Get(),0);
 */
#endif
    return(0);
}

#endif // 0 of example

#include <systemc>
#include <pthread.h>
#include <unistd.h>

using namespace sc_core;

class ThreadSafeEventIf : public sc_interface {
    virtual void notify(sc_time delay = SC_ZERO_TIME) = 0;
    virtual const sc_event &default_event(void) const = 0;
protected:
    virtual void update(void) = 0;
};

class ThreadSafeEvent : public sc_prim_channel, public ThreadSafeEventIf {
public:
    ThreadSafeEvent(const char *name = ""): event(name) {}

    void notify(sc_time delay = SC_ZERO_TIME) {
        this->delay = delay;
        async_request_update();
    }

    const sc_event &default_event(void) const {
        return event;
    }
protected:
    virtual void update(void) {
        event.notify(delay);
    }
    sc_event event;
    sc_time delay;
};

SC_MODULE(Foo)
{
public:
    SC_CTOR(Foo)
    {
        SC_THREAD(main);

        SC_METHOD(eventTriggered);
        sensitive << event;
        dont_initialize();
    }
private:
    void main() {
        usleep(5 * 1000 * 1000); // Just for the example, event is added to pending events during this sleep
        wait(SC_ZERO_TIME); // Schedule (event is evaluated here)
        usleep(1 * 1000 * 1000); // Just for the example
        std::cout << "Done" << std::endl;
    }

    void eventTriggered() {
        std::cout << "Got event" << std::endl;
    }

public:
    ThreadSafeEvent event;
};

//void *work(void *parm) {
//}

void *externalHostThread(void *arg) {
    usleep(2 * 1000 * 1000); // Just for the example
    Foo* foo = (Foo*)(arg);
    foo->event.notify();
    std::cout << "Event notified from an external host thread" << std::endl;
    return NULL;
}

int sc_main(int argc, char *argv[])
{
    Foo foo("foo");

    pthread_t thread;
    pthread_create(&thread, NULL, externalHostThread, &foo);

    sc_start();

    return 0;
}

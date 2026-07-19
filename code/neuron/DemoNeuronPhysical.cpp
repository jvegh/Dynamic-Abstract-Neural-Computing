#include "DemoNeuronPhysical.h"


#define MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
#define SC_MAKE_TIME_BENCHMARKING  // uncomment to measure the SystemC time with benchmarking macros

#include "NeuronPhysical.h"

// Define parameters for calculating membrane voltage's time derivative
#define Rushin_Amplitude 40000.
// ** #define Rushin_Amplitude 140000.
#define Rushin_A 0.1
//#define Rushin_B 0.2
#define Rushin_B 2

// Define parameters for calculating axon's voltage's time derivative
/*
 * #define Axon_Amplitude 20000.
#define Axon_A 0.15
#define Axon_B 3
*/
#define Synaptic_Amplitude 6000.
#define Synaptic_A 0.15
#define Synaptic_B 3
// RC parameters of the membrane
#define Membrane_Tau .22
// Saved value = 2
#define Membrane_R  .20
#define Membrane_C Membrane_Tau/ Membrane_R

// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

DemoNeuronPhysical::
    DemoNeuronPhysical(sc_core::sc_module_name nm ):  // Module name
   NeuronPhysical (nm)
{ /// Just an empty constructor
    std::cerr << "constructor called\n";
 /*   SC_THREAD(GenerateEvents);
    sensitive << EVENT_GenComp.Initialize;
    dont_initialize();*/
};

DemoNeuronPhysical::
    ~DemoNeuronPhysical(void){}


extern vector<double> Default_RushinParameters;    // Parameters for the rushin current
extern vector<double> Default_NaParameters;// = {Rushin_Amplitude, Rushin_A, 20};
// Parameters for the rushin current
extern vector<double> Default_SynapticParameters;
// Parameters for the axonal input
extern vector<double> Default_MembraneParameters;
// Parameters for the membrane

DemoSimpleSingleAP::
    DemoSimpleSingleAP(sc_core::sc_module_name nm ):  // Module name
    DemoNeuronPhysical (nm)
{ /// Just an empty constructor
    m_RushinParameters = Default_RushinParameters ;    // Parameters for the rushin
    m_NaParameters = Default_NaParameters;
    m_SynapticParameters = Default_SynapticParameters;            // Parameters for the axonal input
    m_MembraneParameters = Default_MembraneParameters;  // Parameters for the membrane
    std::cerr << "constructor called\n";
    SetParameters();
    SC_THREAD(GenerateEvents);
    sensitive << EVENT_GenComp.Initialize;
        dont_initialize(); // Must be initialized before using
};

DemoSimpleSingleAP::
    ~DemoSimpleSingleAP(void){}

/*
 * Generate a single Action Potential-like membrane voltage, with simulated threshold exceeding
 * Imitates chargeup and discharge
 */

void DemoSimpleSingleAP::GenerateEvents()
{
    std::cerr << "Test GenerateEvents called\n";
    EVENT_GenComp.Initialize.notify(PU_InitialDelayTime);
    wait(EVENT_GenComp.Initialize);
    //     std::cerr << "Test Initialized\n";
    BaseTime = sc_core::sc_time_stamp();
    EVENT_GenComp.InputReceived.notify(PU_InputTime1-PU_InitialDelayTime);

    wait(EVENT_GenComp.InputReceived);
    // Received the first input, send the next one
    EVENT_GenComp.InputReceived.notify(PU_InputTime2-PU_InputTime1);

    wait(EVENT_GenComp.InputReceived);
    EVENT_GenComp.InputReceived.notify(PU_InputTime3-PU_InputTime2);
    wait(EVENT_GenComp.InputReceived);
    EVENT_GenComp.InputReceived.notify(PU_InputTime4-PU_InputTime3);
}

void DemoSimpleSingleAP::SetParameters()
{

}

void  DemoSimpleSingleAP::ClearEvents()
{
    EVENT_GenComp.Initialize.cancel();  //
    EVENT_GenComp.InputReceived.cancel(); // Be sure to cancel pending events
}

DemoSimpleFailedAP::
    DemoSimpleFailedAP(sc_core::sc_module_name nm ):  // Module name
    DemoNeuronPhysical (nm)
{ /// Just an empty constructor
    std::cerr << "constructor called\n";
    SetParameters();
    SC_THREAD(GenerateEvents);
    sensitive << EVENT_GenComp.Initialize;
    dont_initialize(); // Must be initialized before using
};

DemoSimpleFailedAP::
    ~DemoSimpleFailedAP(void){}

/*
 * Generate a single Action Potential-like membrane voltage, with simulated threshold exceeding
 * Imitates chargeup and discharge
 */

void DemoSimpleFailedAP::GenerateEvents()
{
    std::cerr << "Test GenerateEvents called\n";
    EVENT_GenComp.Initialize.notify(PU_InitialDelayTime);
    wait(EVENT_GenComp.Initialize);
    //     std::cerr << "Test Initialized\n";
    BaseTime = sc_core::sc_time_stamp();
    EVENT_GenComp.InputReceived.notify(PU_InputTime1-PU_InitialDelayTime);

    wait(EVENT_GenComp.InputReceived);
}

void DemoSimpleFailedAP::SetParameters()
{

}

void  DemoSimpleFailedAP::ClearEvents()
{
    EVENT_GenComp.Initialize.cancel();  //
    EVENT_GenComp.InputReceived.cancel(); // Be sure to cancel pending events
}

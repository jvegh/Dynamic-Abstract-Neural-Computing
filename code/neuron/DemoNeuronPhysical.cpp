#include "DemoNeuronPhysical.h"


// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug event for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

DemoNeuronPhysical::
    DemoNeuronPhysical(sc_core::sc_module_name nm ):  // Module name
   NeuronPhysical (nm)
{ /// Just an empty constructor
    std::cerr << "constructor called\n";
//        Initialize_Do();
    SC_THREAD(GenerateSimpleSingleAP);
    sensitive << EVENT_GenComp.Initialize;
//    dont_initialize();
};

DemoNeuronPhysical::
    ~DemoNeuronPhysical(void){}

/*
 * Generate a single Action Potential-like membrane voltage, with simulated threshold exceeding
 * Imitates chargeup and discharge
 */

 void DemoNeuronPhysical::GenerateSimpleSingleAP()
{
     std::cerr << "Test Generate called\n";
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


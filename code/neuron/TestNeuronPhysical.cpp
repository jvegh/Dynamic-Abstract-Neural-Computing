#include <gtest/gtest.h>
#include "TestNeuronPhysical.h"


// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug event for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

NeuronPhysicalTEST::
    NeuronPhysicalTEST(sc_core::sc_module_name nm ):  // Module name
   NeuronPhysical (nm)
{ /// Just an empty constructor
    std::cerr << "constructor called\n";
//        Initialize_Do();
};

NeuronPhysicalTEST::
     ~NeuronPhysicalTEST(void){}

//NeuronPhysicalTEST NPPU("NeuronPhysical");

// A new test class  of these is created for each test
class TEST_NeuronPhysicalPU
{
public:
    TEST_NeuronPhysicalPU(void);
    ~TEST_NeuronPhysicalPU(void);
 };


/*
 * Generate a single Action Potential-like membrane voltage, with simulated threshold exceeding
 * Imitates chargeup and discharge
 */
 void NeuronPhysicalTEST::Initialize_Do(void)
 {
     std::cerr << "Test Init called\n";
//    EVENT_GenComp.Initialize.notify(PU_InitialDelayTime);
//    wait(EVENT_GenComp.Initialize);
    BaseTime = sc_core::sc_time_stamp();
    EVENT_GenComp.InputReceived.notify(PU_InputTime1-PU_InitialDelayTime);

//    wait(EVENT_GenComp.InputReceived);
 /*       // Received the first input, send the next one
     EVENT_GenComp.InputReceived.notify(PU_InputTime2-PU_InputTime1);

    wait(EVENT_GenComp.InputReceived);
    EVENT_GenComp.InputReceived.notify(PU_InputTime3-PU_InputTime2);
    wait(EVENT_GenComp.InputReceived);
    EVENT_GenComp.InputReceived.notify(PU_InputTime4-PU_InputTime3);
*/
/*
            // After 3 inputs plus one discharge
    //        EXPECT_EQ(0+3*MembraneInputIncrease-MembraneRelaxDischarge,NPPU.RelativeMembranePotential_Get());
    //        EXPECT_EQ(0+4*MembraneInputIncrease-2*MembraneRelaxDischarge,NPPU.RelativeMembranePotential_Get());
    wait(NPPU.EVENT_GenComp.ComputingBegin);

 //   EXPECT_EQ(PU_InputTime4,sc_core::sc_time_stamp());
     wait(NPPU.EVENT_GenComp.DeliveringBegin);
    EXPECT_EQ(4,NPPU.NoOfInputsReceived_Get());
 //   EXPECT_EQ(NPPU.PU_InputTime1+3*NPPU.m_Heartbeat_time_default,sc_core::sc_time_stamp());
   // EXPECT_TRUE(NPPU.MembraneRelativePotential_Get()>ThresholdPotential);

    EXPECT_EQ(GenCompStageMachine_t::gcsm_Delivering, NPPU.StageFlag_Get());  // The unit is relaxing; initially
     wait(NPPU.EVENT_GenComp.DeliveringEnd);

 //   NPPU.EVENT_GenComp.InputReceived.notify(3*NPPU.m_Heartbeat_time_default);
     // 3 heartbeats later there appears a new input in the
    wait(NPPU.EVENT_GenComp.RelaxingBegin);
*/
}

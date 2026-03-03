/** @file scGenComp_PU_Bio.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include "scGenComp_PU_Bio.h"
// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
//#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

static NeuronConstants NeuronData_Default;
static UniversalConstants UniversalData;

/*extern sc_core::sc_time
    Heartbeat_time_default    //< The reset value of heartbeat time
    ,Heartbeat_time_resolution;    //< The time tolerance
*/
#define VOLTAGE_CHANGE_ALLOWED 2.   // Max 2 mV change enabled during integration

// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing
    scGenComp_PU_Bio::
scGenComp_PU_Bio(sc_core::sc_module_name nm
                     ):  // Heartbeat time
    scGenComp_PU_Abstract(nm
                            ) // Defaults to variable execution time, no central clock
    ,m_NeuronData(&NeuronData_Default)

{
    // Those above set $\tau=10\ ms$
    m_Membrane_V = 0;   // Working with relative potentials
    m_Membrane_dV = 0;
    m_MembraneGradientPositive = true;
    s_Heartbeat_time_default = HEARTBEAT_TIME_DEFAULT_BIO;    // Overload the static value
    s_Heartbeat_time_resolution = HEARTBEAT_TIME_RESOLUTION_BIO;
    m_Heartbeat_time = s_Heartbeat_time_default;
}

void scGenComp_PU_Bio::
    DeliveringBegin_Do()
{
    scGenComp_PU_Abstract::DeliveringBegin_Do();
    m_SynapsesEnabled = false;
}
void scGenComp_PU_Bio::
    DeliveringEnd_Do()
{
    scGenComp_PU_Abstract::DeliveringEnd_Do();
    m_MembraneGradientPositive = true;
 //   m_PeakReached = true; // If the AP overpassed the depolarized state
    m_SynapsesEnabled = true; //
}
// Adjusts the heartbeat time (integration step size)
void scGenComp_PU_Bio::
    Heartbeat_Adjust(void)
{
    /* This works with voltage step limitation
    float diff_V = abs(m_Membrane_V-m_Membrane_V_last);
    if(diff_V<0.5)
    { // The membrane's voltage changed less than 0.5 mV; can increase
        m_Heartbeat_time *=2;
        m_dt = m_Heartbeat_time.to_seconds()*1000.;
        DEBUG_SC_EVENT(name(),"Heartbeat set to " << m_dt );
    }
    else if(diff_V>2.0)
    { // The membrane's voltage changed more than 2 mV; must decrease
        m_Heartbeat_time /=2;
        m_dt = m_Heartbeat_time.to_seconds()*1000.;
        DEBUG_SC_EVENT(name(),"Heartbeat set to " << m_dt );
    }

    m_Membrane_V_last = m_Membrane_V;   // Preserve the 'new last' potential
*/
    // This works with dV/dt limitation
    float diff_V = abs(m_Membrane_dV);
     if(diff_V<VOLTAGE_CHANGE_ALLOWED*2)
    { // The membrane's voltage changed less than 0.5 mV; can increase
        if(m_Heartbeat_time <s_Heartbeat_time_default*5)
        {
            HeartbeatTime_Set(m_Heartbeat_time *2);
                DEBUG_SC_EVENT(name(),"Heartbeat set to " << m_dt );
        }
    }
    if(diff_V>VOLTAGE_CHANGE_ALLOWED)
    { // The membrane's voltage changed more than allowed; must decrease if above resolution
        if((m_Heartbeat_time > s_Heartbeat_time_resolution))
        {
            HeartbeatTime_Set(m_Heartbeat_time /2);
                DEBUG_SC_EVENT(name(),"Heartbeat set to " << m_dt );
        }
    }
}

void scGenComp_PU_Bio::
    Heartbeat_Relaxing_Do()
{
}

void scGenComp_PU_Bio::
    Heartbeat_Computing_Do()
{
}

void scGenComp_PU_Bio::
    Heartbeat_Delivering_Do()
{
}

/*
 * Initialize the GenComp unit.
 */
void scGenComp_PU_Bio::
    Initialize_Do(void)
{
    scGenComp_PU_Abstract::Initialize_Do();
                DEBUG_SC_EVENT(name(),"Initialized for BIO mode");
    // Reset synaptic input handling
    m_MembraneGradientPositive = true;
    m_SynapsesEnabled = true; //
    m_Heartbeat_time = s_Heartbeat_time_default;
//    m_dt = m_Heartbeat_time.to_seconds(); // Calculating in usec
}

/*
 * This routine makes actual input processing, although most of the job is done in Process() and Heartbeat()
 * It can be called in state 'Processing' (if not first input),
 * in state 'Ready' if first input,
 * or in state GenCompStateMachineType_t::gcsm_XXXRelaxing in the relative refractory period
 *
 * If it was the first spike, issue 'ComputingBegin' and re-issue
*/

void scGenComp_PU_Bio::
   InputReceived_Do(void)
{
    scGenComp_PU_Abstract::InputReceived_Do();
 }

void scGenComp_PU_Bio::
     OutputItem()
{
    cerr << "(" << sc_time_String_Get(sc_time_stamp()) << "," << MembraneRelativePotential_Get() << ")\n";
}









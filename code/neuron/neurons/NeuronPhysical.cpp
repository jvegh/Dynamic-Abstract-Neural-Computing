/** @file NeuronPhysical.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The physics-based abstract neuron
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#define MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
#define SC_MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros

#include "NeuronPhysical.h"

// Define parameters for calculating membrane voltage's time derivative
#define Rushin_Amplitude 280000.
#define Rushin_A 0.1
//#define Rushin_B 0.2
#define Rushin_B 2

// Define parameters for calculating axon's voltage's time derivative
#define Axon_Amplitude 20000.
#define Axon_A 0.15
#define Axon_B 3
// RC parameters of the membrane
#define Membrane_tau .22
// Saved value = 2
#define Membrane_R  .20
#define Membrane_C Membrane_tau/ m_R_membrane

// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

extern sc_core::sc_time
    Heartbeat_time_default    //< The reset value of heartbeat time
    ,Heartbeat_time_resolution;    //< The time tolerance

int OutputCounter= 0; // Just to help output, temporary
// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing
    NeuronPhysical::
NeuronPhysical(sc_core::sc_module_name nm, NeuronConstants* Neuron):
    scGenComp_PU_Bio(nm),
    m_Neuron(Neuron)
{
        m_RushinCurrent = (NeuronInputCurrent*)NULL;
    m_Neuron->MembraneFromCPF_TauMSec_Set(700,2);
        // Saved value = 2
//        m_R_membrane = m_Neuron->MembraneResistanceGOhm_Get();
//        m_Neuron->MembraneResistanceGOhm_Set(0.1);
/*        m_Membrane_R = m_Neuron->MembraneResistanceGOhm_Get();
        m_Membrane_C = m_Neuron->MembraneCapacityPF_Get();
        m_Neuron->MembraneCapacityPF_Set(100);
        m_Membrane_Tau = m_Neuron->MembraneTauMSec_Get();
 */       Tracing_Initialize();
}

// Create rush-in  current source
// Called when neuron passes to "Delivering"
    void NeuronPhysical::
    Create_Rushin()
{
        vector<double> Par;
        Par.push_back(Rushin_Amplitude);
        Par.push_back(Rushin_A);
        Par.push_back(Rushin_B);
        // From this point on, we do have a rush-in current
        m_RushinCurrent = new NeuronInputCurrent(this,NeuronInputCurrent_t::nict_RushIn,
                                                 scLocalTimeMS_Get(),
                                                 Par
                                                 );
                DEBUG_SC_EVENT(name(),"Rush-in Current "
                                                           << " in stage '" << GenCompStageMachineType2String(mStageFlag)
                                                           << "' opened");
}

void NeuronPhysical::
    Tracing_Initialize()
{
    //scGenComp_PU_Abstract::Tracing_Initialize();
    sc_trace(m_tracefile,m_t,"LocalTime");
    sc_trace(m_tracefile,m_Membrane_V_Rushin,"Rushin_V");
    sc_trace(m_tracefile,m_Membrane_V,"V_out");
    sc_trace(m_tracefile,m_Membrane_dVdt_Resulting,"Resulting_dV/dt");
    sc_trace(m_tracefile,m_Membrane_dVdt_Rushin,"Rushin_dV/dt");
    sc_trace(m_tracefile,m_Membrane_dVdt_AIS,"AIS_dV/dt");
    sc_trace(m_tracefile,float(mStageFlag),"Stage");
    sc_trace(m_tracefile,m_SynapsesEnabled,"Synapses");
}


void NeuronPhysical::
    Initialize_Do()
{
    scGenComp_PU_Bio::Initialize_Do();   // Do also inherited initialization
            DEBUG_SC_EVENT(name(),"Initialized for NeuronPhysical");
    m_Membrane_V = 0;
    m_Membrane_dV = 0;
    m_Relaxing_Stopped = true;
//    m_HasUnhandledInput = false;
    m_SynapsesEnabled = true;
}

void NeuronPhysical::InputCurrentDelete(NeuronInputCurrent* IC)
{
    std::vector<NeuronInputCurrent*>::iterator it;
    it = find(m_SynapticCurrents.begin(),m_SynapticCurrents.end(),IC);
    if (it != m_SynapticCurrents.end())
    {
        DEBUG_SC_EVENT(name(),"Delete input current#" << m_SynapticCurrents.size());
        delete *it;
        m_SynapticCurrents.erase(m_SynapticCurrents.begin());
    }
}

// Imitate potential increase on the membrane, due to an input
void NeuronPhysical::
    InputReceived_Do()
{
    scGenComp_PU_Bio::InputReceived_Do();    // Do also inherited processing
             DEBUG_SC_EVENT_LOCAL(scLocalTime_Get(),name(),"RCVD 'InputReceived' in '" << GenCompStageMachineType2String(mStageFlag) << "'");
    // Add a new synaptic current to the existing ones
    // Create a parameter array, will be destroyed by the current
    std::vector<double> *Par = new vector<double>;
    Par->push_back(Axon_Amplitude);
    Par->push_back(Axon_A);
    Par->push_back(Axon_B);
    // From this point on, we do have a new synaptic input current
    m_SynapticCurrents.push_back(
            new NeuronInputCurrent(this, NeuronInputCurrent_t::nict_RushIn,
                                         scLocalTimeMS_Get(),
                                         *Par
                               ));
}


/**
     * @brief Calculate the membrane's new potential by solving a DE at
     * the simulated time
     *
     * The input currents are provided in units of [pA], the gradients in [pA/s]
     * The voltage is provided in units of [V], the gradients at [V/s]
     * @see NeuronConstants
     */

    void NeuronPhysical::
    Calculate_Do()
{
    m_Input_dVdt = 0; //
    m_dt = m_Heartbeat_time.to_seconds()*1000; // We calculate in msec
 //   double Input_dIdt = 0; // Just for developing/debugging
//    double Membrane_dIdt_Rushin = 0;
    switch(StageFlag_Get())
    {
        case GenCompStageMachine_t::gcsm_Relaxing:
        {
            // Check if we do have rush-in current at all
            if(m_Relaxing_Stopped || !m_RushinCurrent)
                m_Membrane_dVdt_Rushin = 0;
            else
            {
                // Question: if the synaptic charge conserves
               for(uint32_t i = 0; i< m_SynapticCurrents.size(); i++)
                {
                    m_Input_dVdt += m_SynapticCurrents[i]->VoltageGradient_Get(m_t);
//                    Input_dIdt += m_SynapticCurrents[i]->CurrentGradient_Get(m_t);
                }
                m_Membrane_dVdt_Rushin = m_RushinCurrent->VoltageGradient_Get(m_t);
                m_MembraneGradientPositive =
                    (m_Membrane_dVdt_Rushin>0);
            }
            break;
        }
        case GenCompStageMachine_t::gcsm_Computing:
        {
            // Previous membrane!
            m_Membrane_dVdt_Rushin = 0;
            m_Membrane_V_Rushin = 0;
            // Question: if the synaptic charge conserves
            for(uint32_t i = 0; i< m_SynapticCurrents.size(); i++)
            {
                m_Input_dVdt += m_SynapticCurrents[i]->VoltageGradient_Get(m_t); // [V/s]
//                Input_dIdt += m_SynapticCurrents[i]->CurrentGradient_Get(m_t);
            }
            break;
        }
        case GenCompStageMachine_t::gcsm_Delivering:
        {
           // m_SynapsesEnabled = m_Membrane_V < ThresholdPotential;
            if(!m_RushinCurrent) break;
            m_Membrane_V_Rushin = m_RushinCurrent->VoltageValue_Get(m_t);
            m_Membrane_dVdt_Rushin = m_RushinCurrent->VoltageGradient_Get(m_t);
 //           Membrane_dIdt_Rushin = m_RushinCurrent->CurrentGradient_Get(m_t);
            m_MembraneGradientPositive
          //  m_PeakReached
                = m_Membrane_dVdt_Rushin < 0;
            break;
        }
        default: assert(0); break;
    }
    m_AIS_I = m_Membrane_V/MembraneResistanceGOhm_Get(); // The AIS current, in pA
 //   m_Membrane_dVdt_AIS = m_Membrane_V/MembraneResistanceGOhm_Get()   // The AIS dV/dt
 //                         /MembraneCapacityPF_Get(); // The AIS gradient, in [V/s]
    // Equivalent
    m_Membrane_dVdt_AIS = m_Membrane_V/MembraneTauMSec_Get()*1000;  // The AIS gradient, in [V/s]

//    m_MembraneRushin_dVdt =  m_dV_dt_membrane;
//    I_AIS = m_dV_dt_membrane-I_AIS;
    // The resulting gradient, in [mV/s]
    m_Membrane_dVdt_Resulting = m_Membrane_dVdt_Rushin
                                + m_Input_dVdt - m_Membrane_dVdt_AIS; // in [V/s]
//    m_MembraneResulting_dVdt = I_AIS *m_dt   // The charge difference
//                               /m_C_membrane;
    m_Membrane_dV = m_Membrane_dVdt_Resulting * m_dt;  // The voltage  change, in [mV], m_dt in [sec]

    m_Membrane_V +=  m_Membrane_dV; // in [mV]
        /*//.2+
                    m_MembraneResulting_dVdt *m_dt   // The charge difference
//                /m_C_membrane
        ;
*/
    // Now we know all changed quatities; adjust the step size
    Heartbeat_Adjust();
}

// Handle neuronal membrane's potential in 'Computing' mode
void NeuronPhysical::
    Heartbeat_Computing_Do()
{
/*    if(m_HasUnhandledInput)
    {
//??        OutputItem();
//        m_Membrane_V += 8; // A temporary hack, just to imitate potential increase
        m_HasUnhandledInput = false;
    }*/
    Calculate_Do();
}

// Handle neuronal membrane's potential in 'Delivering' mode
void NeuronPhysical::
    Heartbeat_Delivering_Do()
{
    Calculate_Do();
}

// Handle heartbeats in 'Relaxing' mode
void NeuronPhysical::
    Heartbeat_Relaxing_Do()
{
    Calculate_Do();
}

// return true if to stop heartbeating in 'Computing' mode
bool NeuronPhysical::
    Heartbeat_Computing_Stop()
{   if(m_Membrane_V < 0)
    {   // Charging failed
        ChargeupFailed();
        return true;
    }
    else //No obvious problem, continue computing
    {
        if(m_Membrane_V < ThresholdPotential) return false;
        // We are over, finish computing and start delivering
        Create_Rushin();
        return true;
     }
}

// return true if to stop heartbeating in 'Delivering' mode
bool NeuronPhysical::
    Heartbeat_Delivering_Stop()
{  
    if(m_Membrane_V >= ThresholdPotential)
        return false; // Continue delivering
    m_Relaxing_Stopped = false; // Relaxing just began
    return true;
}

// return true if to stop heartbeating in 'Relaxing' mode
bool NeuronPhysical::
    Heartbeat_Relaxing_Stop()
{
    // Maybe we were relaxing and received our first input
//    if(m_HasUnhandledInput)return true;
    // We are after delivering, check if before the hyperrelaxation turning point
    if(!m_MembraneGradientPositive) return false;
//    if(!m_PeakReached) return false;
    // We are over the hyperpolarization turning point, check if resting
    if ((abs(m_Membrane_V )>= AllowedRestingPotentialDifference))
        return false; // Continue relaxing
    // Make sure the resting potential really zero
    m_Membrane_V = 0; m_Relaxing_Stopped = true;
    // Cancel the synaptic inputs currents
    while(m_SynapticCurrents.size())
        InputCurrentDelete(m_SynapticCurrents[0]);
    // Cancel rush-in current
    delete m_RushinCurrent;
    m_RushinCurrent = (NeuronInputCurrent*)NULL;
    return true;
}


 void  NeuronPhysical::
    OutputItem(void)
{
 //    if(0==OutputCounter++%5)
    cerr << m_t  << ","
//     sc_time_String_Get(scLocalTime_Get()) << ","
         << m_Membrane_V << ","
         << m_Membrane_V_Rushin << ","
         << m_Membrane_dVdt_Rushin << ","
         << m_Membrane_dVdt_AIS << ","
         <<m_Membrane_dVdt_Resulting << ","
         << m_SynapsesEnabled << ","
         << m_AIS_I/m_Neuron->MembraneResistanceGOhm_Get()/150000 << ","
         <<  "\n"; //<
 }


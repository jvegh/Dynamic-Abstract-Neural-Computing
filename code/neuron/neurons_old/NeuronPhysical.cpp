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
#define Rushin_Amplitude 350.
#define Rushin_A 4
//#define Rushin_B 0.2
#define Rushin_B 0.18
// Define parameters for calculating axon's voltage's time derivative
#define Axon_Amplitude 40.
#define Axon_A 6.3
#define Axon_B 0.2
// RC parameters of the membrane
#define Membrane_tau .22
// Saved value = 2
#define Membrane_R  .20
#define Membrane_C tau/ m_R_membrane


// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
//#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"

int OutputCounter= 0; // Just to help output, temporary
// The units of general computing work in the same way, using general events
// \brief Implement handling the states of computing
    NeuronPhysical::
NeuronPhysical(sc_core::sc_module_name nm ):
    scGenComp_PU_Bio(nm)
{
        m_RushinCurrent = (NeuronInputCurrent*)NULL;
        m_tau = Membrane_tau;
        // Saved value = 2
        m_R_membrane =Membrane_R; // Resistivity of the membrane
        m_C_membrane = m_tau/ m_R_membrane; // capacity, arbitrary
        Tracing_Initialize();
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
                                                 scLocalTime_Get().to_seconds()*1000.,
                                                 Par
                                                 );
}

void NeuronPhysical::
    Tracing_Initialize()
{
    //scGenComp_PU_Abstract::Tracing_Initialize();
    sc_trace(m_tracefile,m_t,"LocalTime");
    sc_trace(m_tracefile,m_MembraneRushin_V,"Rushin_V");
    sc_trace(m_tracefile,m_Membrane_V,"V_out");
    sc_trace(m_tracefile,m_MembraneResulting_dVdt,"Resulting_dV/dt");
    sc_trace(m_tracefile,m_MembraneRushin_dVdt,"Rushin_dV/dt");
    sc_trace(m_tracefile,m_MembraneAIS_dVdt,"AIS_dV/dt");
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
    m_HasUnhandledInput = false;
    m_SynapsesEnabled = true;
}

void NeuronPhysical::InputCurrentDelete(NeuronInputCurrent* IC)
{
    std::vector<NeuronInputCurrent*>::iterator it;
    it = find(m_SynapticCurrents.begin(),m_SynapticCurrents.end(),IC);
    if (it != m_SynapticCurrents.end())
    {
//        std::cout << "Element found in myvector: " << *it << '\n';
        DEBUG_SC_EVENT(name(),"Delete input current#" << m_SynapticCurrents.size());
        delete *it;
        m_SynapticCurrents.erase(m_SynapticCurrents.begin());
    }
//    else
//        std::cout << "Element not found in myvector\n";
}

// Imitate potential increase on the membrane, due to an input
void NeuronPhysical::
    InputReceived_Do()
{
    scGenComp_PU_Bio::InputReceived_Do();    // Do also inherited processing
             DEBUG_SC_EVENT_LOCAL(scLocalTime_Get(),name(),"RCVD 'InputReceived' in '" << GenCompStageMachineType2String(mStageFlag) << "'");
    m_HasUnhandledInput = true;
    // Add a new synaptic current to the existing ones
    // Create a parameter array, will be destroyed by the current
    std::vector<double> *Par = new vector<double>;
    Par->push_back(Axon_Amplitude);
    Par->push_back(Axon_A);
    Par->push_back(Axon_B);
    // From this point on, we do have a new synaptic input current
    m_SynapticCurrents.push_back(
            new NeuronInputCurrent(this, NeuronInputCurrent_t::nict_RushIn,
                                         scLocalTime_Get().to_seconds()*1000.,
                                         *Par
                               ));
}

float NeuronPhysical::
    AxonTimeDerivative_Get(float Delay)
{
    double LocalTime = m_t-Delay;  //
    double dVdt = Axon_Amplitude*(
                      Axon_A*exp(-Axon_A*LocalTime-Axon_B*LocalTime)
                      -Axon_B*exp(-Axon_B*LocalTime)*exp(1-exp(-Axon_A*LocalTime))
                      );
    return dVdt;
}

/**
     * @brief Calculate the membrane's new potential by solving a DE at
     * the new time after advancing time by the Heartbeat value
     */

    void NeuronPhysical::
    Calculate_Do()
{
//    m_MembraneRushin_V = MembraneRushinVoltage_Get();
    m_Input_dVdt = 0; //
    switch(StageFlag_Get())
    {
        case GenCompStageMachine_t::gcsm_Relaxing:
        {
            // Check if we do have rush-in current at all
            if(m_Relaxing_Stopped || !m_RushinCurrent)
                m_MembraneRushin_dVdt = 0;
            else
            {
                // Question: if the synaptic charge conserves
                for(uint32_t i = 0; i< m_SynapticCurrents.size(); i++)
                {
                    m_Input_dVdt += m_SynapticCurrents[i]->Gradient_Get(m_t-scLocalTime_Get().to_seconds()*1000.);
                }
                m_MembraneRushin_dVdt = m_RushinCurrent->Gradient_Get(m_t-mDeliveringBeginTime.to_seconds()*1000.);
                m_MembraneGradientPositive =
                    (m_MembraneRushin_dVdt>0);
            }
            break;
        }
        case GenCompStageMachine_t::gcsm_Computing:
        {
            // Previous membrane!
            m_MembraneRushin_dVdt = 0;
            m_MembraneRushin_V = 0;
            // Question: if the synaptic charge conserves
            for(uint32_t i = 0; i< m_SynapticCurrents.size(); i++)
            {
                m_Input_dVdt += m_SynapticCurrents[i]->Gradient_Get(m_t-scLocalTime_Get().to_seconds()*1000.);
            }
            break;
        }
        case GenCompStageMachine_t::gcsm_Delivering:
        {
            m_SynapsesEnabled = m_Membrane_V < ThresholdPotential;
            if(!m_RushinCurrent) break;
            m_MembraneRushin_V = m_RushinCurrent->Value_Get(m_t);
            m_MembraneRushin_dVdt = m_RushinCurrent->Gradient_Get(m_t);
            m_MembraneGradientPositive
          //  m_PeakReached
                = m_MembraneRushin_dVdt < 0;
            break;
        }
        default: assert(0); break;
    }
    m_AIS_I = m_Membrane_V/m_R_membrane * m_dt; // The AIS current
    m_MembraneAIS_dVdt = m_Membrane_V/m_R_membrane   // The AIS dV/dt
                         /m_C_membrane;

//    m_MembraneRushin_dVdt =  m_dV_dt_membrane;
//    I_AIS = m_dV_dt_membrane-I_AIS;
    m_MembraneResulting_dVdt = m_MembraneRushin_dVdt - m_MembraneAIS_dVdt;
//    m_MembraneResulting_dVdt = I_AIS *m_dt   // The charge difference
//                               /m_C_membrane;
    m_Membrane_dV = m_MembraneResulting_dVdt * m_dt;  // The charge difference

    m_Membrane_V +=  m_Membrane_dV;
        /*//.2+
                    m_MembraneResulting_dVdt *m_dt   // The charge difference
//                /m_C_membrane
        ;
*/
//    Heartbeat_Adjust();
}

// Handle neuronal membrane's potential in 'Computing' mode
void NeuronPhysical::
    Heartbeat_Computing_Do()
{
    if(m_HasUnhandledInput)
    {
//??        OutputItem();
        m_Membrane_V += 8; // A temporary hack, just to imitate potential increase
        m_HasUnhandledInput = false;
    }
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
    if(m_HasUnhandledInput)return true;
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
         << m_MembraneRushin_V << ","
         << m_MembraneRushin_dVdt << ","
         << m_MembraneAIS_dVdt << ","
         <<m_MembraneResulting_dVdt << ","
         << m_SynapsesEnabled << ","
         << (m_Membrane_V - m_AIS_I) << ","
         <<  "\n"; //<
 }


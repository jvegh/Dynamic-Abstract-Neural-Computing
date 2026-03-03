/** @file NeuronInputCurrent.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The physics-based abstract neuron's input current
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

//#define MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
//#define SC_MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros

#include "NeuronInputCurrent.h"
using namespace std;

// Define parameters for calculating membrane voltage's time derivative
#define Membrane_Amplitude 350.
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

#include "NeuronInputCurrent.h"
#include "NeuronPhysical.h"


NeuronInputCurrent::
    NeuronInputCurrent(NeuronPhysical* ParentNeuron
                       , NeuronInputCurrent_t CurrentType
                       , double BeginTime
                       , std::vector<double> &Parameters
                       ):
    m_ParentNeuron( ParentNeuron)  // Remember owner neuron
    ,m_Parameters(Parameters)      // Input current's parameters
    ,m_CurrentType(CurrentType)     // and the type of current
    ,m_BeginTime(BeginTime)          // Beginning of the time scale
{
             // This code is temporary, for handling "double exponential" type
    switch(m_CurrentType)
    {
    case NeuronInputCurrent_t::nict_RushIn:
    case NeuronInputCurrent_t::nict_PSP:
        assert(3 == Parameters.size());
        break;
    default: assert(0); // Unknown current type
        break;
    }

            DEBUG_SC_EVENT(m_ParentNeuron->name(),"Current# " << m_ParentNeuron->m_SynapticCurrents.size()
                                                       << " in stage '" << GenCompStageMachineType2String(m_ParentNeuron->mStageFlag)
                                               << "' opened");
}
NeuronInputCurrent::~NeuronInputCurrent(void)
{            DEBUG_SC_EVENT(m_ParentNeuron->name(),"Current# " << m_ParentNeuron->m_SynapticCurrents.size()
                                                    << " in stage '" << GenCompStageMachineType2String(m_ParentNeuron->mStageFlag)
                                               << "' closed");
    m_Parameters.clear();
}


// The time must be in milliseconds
double NeuronInputCurrent::
    CurrentValue_Get(double TT)
{
    double T = TT - m_BeginTime;
    if(T<0)return 0.;
    switch(m_CurrentType)
    {
    case NeuronInputCurrent_t::nict_RushIn:
    case NeuronInputCurrent_t::nict_PSP:
        return m_Parameters[0]*(1-exp(-T/m_Parameters[1]))*exp(-T/m_Parameters[2]);
        break;
        // No need to check current's type
    default: assert(0);
    }
}

double NeuronInputCurrent::
    VoltageGradient_Get(double TT)
{
    return CurrentGradient_Get(TT)/m_ParentNeuron->MembraneCapacityPF_Get();
}

double NeuronInputCurrent::
    VoltageValue_Get(double TT)
{
    return CurrentValue_Get(TT)/m_ParentNeuron->MembraneCapacityPF_Get();
}

// The time must be in milliseconds
double NeuronInputCurrent::
    CurrentGradient_Get(double TT)
{
    double T = TT - m_BeginTime;
    if(T<0)return 0.;
    switch(m_CurrentType)
    {
    case NeuronInputCurrent_t::nict_RushIn:
    case NeuronInputCurrent_t::nict_PSP:
        return m_Parameters[0]*(
                      exp(-T/m_Parameters[1]-T/m_Parameters[2])/m_Parameters[1]
                      -exp(-T/m_Parameters[2])*exp(1-exp(-T/m_Parameters[1]))/m_Parameters[2]
                      );
        break;
        // No need to check current's type
    default: assert(0);
    }

}

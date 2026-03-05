/** @file NeuronPhysical.h
 *  @brief Function prototypes for the physics-based neuron module
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef NEURONPHYSICS_H
#define NEURONPHYSICS_H

#include "scGenComp_PU_Bio.h"
#include "NeuronInputCurrent.h"

/** @addtogroup GENCOMP_MODULE_BIOLOGY Biological processing
 *  @{
 */
/*!
 * \class NeuronPhysical
 * \brief  Implements a general physical-type neuron
 *
 * The neuron membrane's current and synaptic currents are descibed
 * by a current contribution in which a saturating charge-up function
 * is multiplied by a discharge function
 * In the corresponding states, the neuron is in one of its GenCompStageMachineType_t states
 *  - GenCompStageMachineType_t::gcsm_Computing computing (collects charge)
 *  - GenCompStageMachineType_t::gcsm_Delivering: delivers action potential
 *  - GenCompStageMachineType_t::gcsm_Relaxing: restores resting potential
 *
 *  The Heartbeat_method() handles an internal 'state update' signal. In the corresponding stages, the neuron is
 *  - GenCompStageMachineType_t::gcsm_Relaxing: Until it reaches its resting potential, calls Calculate_Do(). Upon receiving an input,
 *    changes to stage gcsm_Processing. Inputs enabled: the relative refractory period.
 *  - GenCompStageMachineType_t::gcsm_Computing: Until it reaches its threshold potential, calls Calculate_method().
 *    At that point, mebrane passes to stage gcsm_Delivering. Inputs enabled.
 *  - GenCompStageMachineType_t::gcsm_Delivering: Until it reaches the threshold membrane potential again
 *    calls Calculate_method(). (The neuron delivers its result to its output section.)
 *    After terminating, it passes to stage gcsm_Relaxing, without restoring membrane's potential.
 *    At the beginning 'TransmittingBegin' is also issued.
 *    (Sends 'Begin TransmittingBegin') @see scGenComp_PU_Abstract.
 *
 *    Signal transmission happens in parallel with the further processing.
 *    Inputs disabled; the absolute refractory period.
 *
 * --These stages below are momentary states: need some action and passes to one of the above stages
 *  - NeuronPhysical::InputReceived_method(): the unit received new input.
 *  Only administrative action; the received input is handled in Heartbeat_XXX_Do(),
 *  as described in Calculate_Do(). Disabled in mode gcsm_Delivering.
 *
 *  - NeuronPhysical::Synchronize_method(): deliver immediate spike, anyhow ;  (a momentary state)
 *   Passes to gcsm_Relaxing (after issuing 'DeliveringEnd')
 */

class NeuronPhysical : public scGenComp_PU_Bio
{
    friend NeuronInputCurrent;
public:
    /*!
     * \brief Creates a physics-based neuron unit
     * @param nm the SystemC name of the module
     */
    NeuronPhysical(sc_core::sc_module_name nm );
    virtual ~NeuronPhysical(void)
    {
        m_RushinCurrent = (NeuronInputCurrent*)NULL;

        // using std::find with vector and iterator:
/*        std::vector<NeuronInputCurrent*>::iterator it;
        it = find(m_SynapticCurrents.begin(),m_SynapticCurrents.end(),

        it = find (myvector.begin(), myvector.end(), 30);
        if (it != myvector.end())
            std::cout << "Element found in myvector: " << *it << '\n';
        else
            std::cout << "Element not found in myvector\n";

        for (std::vector<NeuronInputCurrent*> it = m_SynapticCurrents.begin() ; it != m_SynapticCurrents.end(); it++)
        {

            m_SynapticCurrents.find()
            std::vector<NeuronInputCurrent*>::iterator SynapticCurrent =
                find(m_SynapticCurrents.begin(), m_SynapticCurrents.end(), *it);
            assert(*SynapticCurrent!=nullptr);
            delete *SynapticCurrent;
            m_SynapticCurrents.erase(m_SynapticCurrents.begin());
//            *SynapticCurrent=0;
 //           m_SynapticCurrents.shrink_to_fit();
        }
        m_SynapticCurrents.clear();  // Delete the maybe active synaptic currents
*/
    }// Must be overridden

    virtual void Tracing_Initialize(); // Initialize tracing: voltage vs time
    /**
     * @brief Calculate the membrane's new potential by solving a PDE at
     * the new time after advancing time by the Heartbeat value
     */
    virtual void Calculate_Do();
    /**
     * @brief Pass the parameters for the funtions
     */
    virtual void Create_Rushin();
    /*! Heartbeat processing in 'Computing':
     *  all inputs effective
     */
    virtual void Heartbeat_Computing_Do();
    /*! Heartbeat processing in 'Delivering':
     *  synaptic inputs disabled
     */
    virtual void Heartbeat_Delivering_Do();
    /*! Heartbeat processing in 'Relaxing':
     *  all inputs effective
     */
    virtual void Heartbeat_Relaxing_Do();

    //! Activity for intializing the unit
    virtual void Initialize_Do();

    /**
     *   Stage 'Computing' normally passes to stage 'Delivering',
     *   but it can fall-back to 'Relaxing' mode.
     *   If membrane's potential is above threshold, a rush-in current is created and
     *   the 'Delivering' stage follows.
     *   If membrane's potential is near to the resting value (the excitation failed),
     *   the 'Relaxing' stage follows.
     *   In these cases the function returns 'true' that finishes the stage.
     *   Otherwise, the neuron remains in 'Computing' stage (the function returns 'false')
     *
     *   @return true if to stop heartbeating in 'Computing' mode
     */
    virtual bool Heartbeat_Computing_Stop();
    /** Stage 'Delivering' normally passes to stage 'Relaxing',
     *  @return true if to stop heartbeating in 'Delivering' mode
     */
        virtual bool Heartbeat_Delivering_Stop();
    // @return true if to stop heartbeats in 'Relaxing' mode
         virtual bool Heartbeat_Relaxing_Stop();


    void InputCurrentDelete(NeuronInputCurrent* IC);
    /**
     * @brief A new synaptic input (spike or clamping) received; only in 'Relaxing' and 'Computing' states,
     * furthermore in "XXXRelaxing" state during the "relative refractory" period
     *
     * A spike arrived, store spike parameters. Receving an input is a momentary action, just administer its processing.
     * Most of the job is done in methods Heartbeat_Relaxing() and Heartbeat_Computing().
     * If it was the first spike, issue 'Begin_Computing'
     *
     * Called by scGenComp_PU_Abstract::InputReceived_method()
     * Reimplemented given that in biology the first input also starts processing
     *
     * The received input inserts a new NeuronInputCurrent (for rush-in,
     * for synaptic inputs, for clamping and patching). The integration interrogates those
     * input sources.
     *
     * Changing operating stage resets integration time step.
     *
     * We prepare for several simultanous synaptic inputs: the currents are stored
     * in a vector and contribute independently.
     */



    virtual void InputReceived_Do();

    /**
     * @brief AxonTimeDerivative_Get
     * param[in] Delay of axonal input with respect to local time, in ms
     * @return the time derivative of axon voltage due to the arrived current
     */
    float AxonTimeDerivative_Get(float Delay);
    void  OutputItem(void);
    vector <NeuronInputCurrent*> m_SynapticCurrents;    // Stores pointers to the currents

protected:

    NeuronInputCurrent* m_RushinCurrent;
    // Just for tracing
//    double m_dV_dt_membrane;    // The actual time derivative
    double m_MembraneRushin_V;    // Only for tracing
    double m_Input_dVdt;    // The sum of the input gradients
    double m_MembraneRushin_dVdt;
    double m_MembraneAIS_dVdt;
    double m_MembraneResulting_dVdt;
    double m_AIS_I; // Current through the AIS


    bool m_HasUnhandledInput; // Signal if to open a new input
    double m_tau;
    // Saved vaulue = 2
    double m_R_membrane; // Resistivity of the membrane
    double m_C_membrane; // capacity, arbitrary
};// of class NeuronPhysical
/** @}*/
#endif // NEURONPHYSICS_H

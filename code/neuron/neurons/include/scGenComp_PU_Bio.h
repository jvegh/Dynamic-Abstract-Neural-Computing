/** @file scGenComp_PU_Bio.h
 *  @ingroup GENCOMP_MODULE_BIOLOGY

 *  @brief Function prototypes for the bio computing module
 *  It is just event handling, no modules
 *
 *  @todo
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */


#ifndef SCBIOGENCOMP_H
#define SCBIOGENCOMP_H

#include "scGenComp_PU_Abstract.h"

#ifndef NEURONDEMO_H

/*!
 * \class scGenComp_PU_Bio
 * \brief  Implements a general biological-type computing PU.
 * Defaults to variable (that is, biologically defined)  execution time, no central clock.
 *
 * In the corresponding states, the neuron is in one of its GenCompStageMachine_t states
 *  - GenCompStageMachine_t::gcsm_Computing: computing (collects charge)
 *  - GenCompStageMachine_t::gcsm_Delivering: delivers action potential
 *  - GenCompStageMachine_t::gcsm_Relaxing: restores resting potential
 *
 *  The scGenComp_PU_Abstract::Heartbeat_method() handles an internal 'update' signal.
 *  Internally it branches to scGenComp_PU_Abstract::Heartbeat_Computing_Do(),
 *  scGenComp_PU_Abstract::Heartbeat_Delivering_Do() or scGenComp_PU_Abstract::Heartbeat_Relaxing_Do(),
 *  as the current GenCompStageMachine_t requires. Usually, it calls Calculate_Do().
 *
 *  During processing, it calls a Heartbeat_XXX_Stop() method
 *  to see if heartbeating in the current state should be stopped. It advances to the next stage
 *  or stops heartbeating.
 *
 *  (It is _NOT_ a central clock signal; it refreshes only the SIMULATED time.)
 *  In the corresponding states, the neuron is
 *  - GenCompStageMachine_t::gcsm_Relaxing: Until it reaches its resting potential, calls Calculate_Do(). Upon receiving an input,
 *    changes to state GenCompStageMachine_t::gcsm_Computing. Inputs enabled: the relative refractory period.
 *  - GenCompStageMachine_t::gcsm_Computing: Until it reaches its threshold potential, calls Calculate_Do().
 *    At that point, changes to state  to state GenCompStageMachine_t::gcsm_Delivering. Inputs enabled.
 *  - GenCompStageMachine_t::gcsm_Delivering: Until it reaches its lowest membrane potential
 *    (its increase turns from negative to positive), calls Heartbeat_Relaxing_Do().
 *    At that point, changes to state  to state GenCompStageMachine_t::gcsm_Relaxing, without restoring membrane's potential.
 *    At the beginning issues signal 'TransmittingBegin'. Signal transmission happens
 *    in parallel with the further processing. Inputs disabled; the absolute refractory period.
 *
 *  The InputReceived_method(): the unit received new input (a momentary state).
 *  Only administrative action; the received input is handled in Heartbeat_method(),
 *  as described in Calculate_Do(). Disabled in mode GenCompStageMachine_t::gcsm_Delivering.
 *
 * --The states below are momentary states: need action and passes to one of the above states
 * - GenCompStageMachine_t::gcsm_Delivering: The unit is delivering its result to its output section
 *   - After some time, it Sends 'Begin Transmitting' @see scGenComp_PU_Abstract
 *     (Activates transmission unit to send computed result to its chained unit(s),
 *      then goes to GenCompStageMachine_t::gcsm_Relaxing
 * - GenCompStageMachine_t::gcsm_Synchronizing: deliver result, anyhow ;  (a momentary state)
 *   - in biological mode, deliver immediate spike
 *   Passes to GenCompStageMachine_t::gcsm_Relaxing (after issuing EVENT_GenComp_t::ComputingEnd)
 *
 *

\anchor fig_AP_ConceptualDemo
@image html AP_ConceptualDemo.png "The conceptual time course of an action potential" width=500px

@latexonly
\begin{figure}
\includegraphics[width=.4\textwidth]{images/AP_ConceptualDemo.pdf}
\caption{The conceptual time course of an action potential}
\label{fig_AP_ConceptualDemo}
\end{figure}
@endlatexonly

 */
#endif // NEURONDEMO_H


/** @addtogroup GENCOMP_MODULE_BIOLOGY
 *  @{
 */

class scGenComp_PU_Bio : public scGenComp_PU_Abstract
{
  public:
    /*!
     * \brief Creates a biological general computing unit
     * @param[in] nm the SystemC name of the module
     *
     * Creates an abstract biological computing unit
     */
    scGenComp_PU_Bio(sc_core::sc_module_name nm   // Module name
                      );
      virtual ~scGenComp_PU_Bio(void){} // Must be overridden

    /** @brief The extra processing for beginning computing
     */
    virtual void ComputingBegin_Do()
    {
    }
    void DeliveringBegin_Do();
    void DeliveringEnd_Do();

    /**
     * @brief A new spike (or clamping) received; only in 'Relaxing' and 'Computing' states,
     * furthermore in "XXXRelaxing" state during the "relative refractory" period
     *
     * A spike arrived, store spike parameters. Receving an input is a momentary action, just administer its processing.
     * Most of the job is done in methods Heartbeat_Relaxing() and Heartbeat_Computing().
     * If it was the first spike, issue 'Begin_Computing'
     *
     * Called by scGenComp_PU_Abstract::InputReceived_method
     * Reimplemented given that in biology the first input also starts computing
     */
    virtual void InputReceived_Do();

    /**
     * @brief Heartbeat_method
     *
     * A periodic  signal as a timebase for solving differential equations.
     *
     * - The unit receives a signal EVENT_GenComp.HeartBeat and handles it
     *   differently in different modes
     * - In 'Computing' mode, re-calculates membrane's charge-up potential
     * - In 'Relaxing' mode, re-calculates membrane's decay potential
     * - In 'Delivering' mode, re-calculates membrane's decay potential
     */
    /**
     * @brief Adjust integration step size, to keep accuracy and computing time tolerable
     */
    virtual void Heartbeat_Adjust(void);
    //! Heartbeat activity for relaxing
    virtual void Heartbeat_Relaxing_Do();
    //! Heartbeat activity for computing
    virtual void Heartbeat_Computing_Do();
    //! Heartbeat activity for delivering
    virtual void Heartbeat_Delivering_Do();

    float MembraneAbsolutePotential_Get(){return m_Membrane_V+RestingMembranePotential;}
    float MembraneRelativePotential_Get(){return m_Membrane_V;}

/*    // Values taken from Koch's Biophysics, page 11
    void Membrane_R_Set(double R = 0.1)    //< in GOhm
    {
        m_NeuronData->MembraneResistanceGOhm_Set(R);
      }    // Store the value in kOhm
    void Membrane_C_Set(double C = 100.)    //< in pF
      {   m_NeuronData->MembraneCapacitancePF_Set(C);}    // Store the value in fF
    double Membrane_TauMS_Get(void)   //< Return $\tau$ in ms
      {   return m_NeuronData->MembraneTauMSec_Get();}
    double MembraneResistanceGOhm_Get()
    {return m_NeuronData->MembraneResistanceGOhm_Get();}
*/
protected:
    //! Puts the PU to its default state
    virtual void Initialize_Do();
    void OutputItem();
    float m_Membrane_V; //< The actual value of membrane potential, in mV
    float m_Membrane_V_last; //< The actual value of membrane potential, in mV
    float m_Membrane_dV;   //< The actual change in membrane potential, in mV
    bool m_MembraneGradientPositive; //< Negative from AP peak to HP peak
 //   bool m_PeakReached; //< If the AP overpassed the depolarized state
    bool m_SynapsesEnabled; //<
private:
    NeuronConstants* m_NeuronData;
  };// of class scGenComp_PU_Bio
/** @}*/

#endif // SCBIOGENCOMP_H

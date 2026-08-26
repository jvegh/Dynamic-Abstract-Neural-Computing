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
#if 0
#include "../3rdParty/ode/include/ode/ode_euler.h"
#endif

#define MakeDebugPrint 0

/** @addtogroup GENCOMP_MODULE_BIOLOGY Biological processing
 *  @{
 */
/*!
 * \class NeuronPhysical
 * \brief  Implements a general physical-type neuron.
 * The neuron collects membrane's, synaptic and artificial currents,
 * for details see NeuronInputCurrent.
 *
 * \todo Implement the prepared midpoint Euler integration method
 */
//static  NeuronConstants DefaultNeuron;
class ScQtNeuron_MainWindow;

class NeuronPhysical : public scGenComp_PU_Bio,
#if 0
                       ode::OdeEuler::OdeEuler,
#endif // 0
                       NeuronConstants
{
    friend class NeuronInputCurrent; friend class ScQtNeuron_MainWindow;
public:
    /*!
     * \brief Creates a physics-based neuron unit
     *
     *  * In the corresponding states, the neuron is in one of its GenCompStageMachine_t states
 *  - GenCompStageMachine_t::gcsm_Computing computing (collects charge)
 *  - GenCompStageMachine_t::gcsm_Delivering: delivers action potential
 *  - GenCompStageMachine_t::gcsm_Relaxing: restores resting potential
 *
 *  The Heartbeat_method() handles an internal 'state update' signal. In the corresponding stages, the neuron is
 *  - GenCompStageMachine_t::gcsm_Relaxing: Until it reaches its resting potential, calls Calculate_Do(). Upon receiving an input,
 *    changes to stage GenCompStageMachine_t::gcsm_Computing. Inputs enabled: the relative refractory period.
 *  - GenCompStageMachine_t::gcsm_Computing: Until it reaches its threshold potential, calls Calculate_method().
 *    At that point, mebrane passes to stage GenCompStageMachine_t::gcsm_Delivering. Inputs enabled.
 *  - GenCompStageMachine_t::gcsm_Delivering: Until it reaches the threshold membrane potential again
 *    calls Calculate_method(). (The neuron delivers its result to its output section.)
 *    After terminating, it passes to stage GenCompStageMachine_t::gcsm_Relaxing, without restoring membrane's potential.
 *    At the beginning 'TransmittingBegin' is also issued.
 *    (Sends 'Begin TransmittingBegin') @see scGenComp_PU_Abstract.
 *
 *    Signal transmission happens in parallel with the further processing.
 *    Inputs disabled; the absolute refractory period.
 *
 * --These stages below are momentary states: need some action and passes to one of the above stages
 *  - NeuronPhysical::InputReceived_method(): the unit received new input.
 *  Only administrative action; the received input is handled in Heartbeat_XXX_Do(),
 *  as described in Calculate_Do(). Disabled in mode GenCompStageMachine_t::gcsm_Delivering.
 *
 *  - NeuronPhysical::Synchronize_method(): deliver immediate spike, anyhow ;  (a momentary state)
 *   Passes to GenCompStageMachine_t::gcsm_Relaxing (after issuing 'DeliveringEnd')
 *
 *   For solving the Ordinary Differential Equation, the package\\
 *   and its Euler method is used.\\
 *   https://en.wikipedia.org/wiki/Euler_method
     * @param nm the SystemC name of the module
     */
    NeuronPhysical(sc_core::sc_module_name nm
                   );
    virtual ~NeuronPhysical(void)
    {
    }// Must be overridden
#if 0
    //!evaluates the system of ODEs in autonomous form and must be defined by a derived class
    /* !
        The incoming `solin` vector contains the current values of all solution variables and has length `neq`. The output vector should be filled with the time derivatives for each variable in `solin`. All elements of `fout` should be set, even if they're zero, because the `fout` array isn't cleared before it's reused.
            \param[in] solin current solution array
            \param[out] fout evaluation of system of ordinary differential equations
        */

    virtual void ode_fun (double *solin, double *fout)
    {

    }

    virtual void adapt()
    {

    }

    //------
    //extras

    //!does any extra stuff before starting a solve
    virtual void before_solve ();
    //!does any extra stuff after each step
    /*!
        \param[in] t current value of ODE system's independent variable
        */
    virtual void after_step (double t);
    //!does any extra stuff only when a step is captured

    virtual void after_solve ()
    {

    }
    /*!
        This is a virtual function overridden by the class which implements the stepping algorightm. It should never be used outside of the wrapper step() function, and this wrapper should always be used instead.
            \param[in] dt time step size
        */
    virtual void step_ (double dt);

    //----------------
    //solver functions

    //!increments the step counter and the time, checks the solution integrity if needed, stores the time step in the object, and executes after_step() if extra is true
    //! neval_must be incremented in step() when defined
    //!
    //! In the first-order process, the formula
    //!
    //! \f$y_{n+1} = y_{n} + h\times f\bigl(t_{n},y_{n}\bigr)\f$
    //!
    //! in the second-order one (the midpoint method), the formula
    //!
    //! \f$y_{n+1} =  y_{n} + h\times f\bigl(t_n+\frac{1}{2},  y_{n} + \frac{1}{2} f(t_{n},y_{n}) \bigr)\f$
    //!
    //! is used.
    //!  \f$ y^\prime = f\bigl(t,y(t)\bigr)\f$
    //!
    //! Recently, only the 1st order formula is used.
    //! The time is pre-set (i.e., no direct way to set the time to another value)
    //!
    void step (double dt, bool extra=true);

    //!advances a single time step (without changing counters or the time) and must be defined in the derived class implementing the solver/method
    /* !
        This is a virtual function overridden by the class which implements the stepping algorightm. It should never be used outside of the wrapper step() function, and this wrapper should always be used instead.
            \param[in] dt time step size
        */
#endif // 0

    virtual void Tracing_Initialize(); // Initialize tracing: voltage vs time
    /**
     * @brief Calculate the actual potential gradient
     * The strictly needed calculations only
     */
    void CalculateGradient();
    /**
     * @brief Calculate the membrane's new potential by solving a PDE at
     * the new time after advancing time by the Heartbeat value
     */
    virtual void Calculate_Do();
    /**
     * @brief Create a new rush-in current for the neuron
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

    //! Activity for initializing computing stage
    virtual void ComputingBegin_Do();

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

    /** Stage 'Delivering' normally passes to stage 'Relaxing',
     *  @return true if to stop heartbeats in 'Relaxing' mode
     */
    virtual bool Heartbeat_Relaxing_Stop();

    /**
     * @brief InputCurrentDelete
     *
     * When a new input current received (see InputReceived_Do()),
     * a new NeuronInputCurrent is added to the internally maintained list.
     * This routine deletes a NeuronInputCurrent from the list.
     *
     * @param IC The input current to delete
     */

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
     * @brief Check the legacy of receiving input
     * @return true if it is received NOT in 'Delivering' state
     */
    virtual bool InputIsLegal(void)
    {
        bool Legal =  GenCompStageMachine_t::gcsm_Delivering != StageFlag_Get();
        if(!Legal) EVENT_GenComp.InputIllegal.notify(SC_ZERO_TIME); // Inform the possible observers
        return Legal;
    }
    /**
     * @brief Finish operating-related activity
     */
    void OperationFinished();

#if MakeDebugPrint
    virtual void  OutputItem(void);
#endif
    vector <NeuronInputCurrent*> m_SynapticCurrents;    // Stores pointers to the currents
    NeuronInputCurrent *m_RushinCurrent;
    NeuronInputCurrent *m_NaCurrent;
    double dVdtResulting_Get(void) {return m_Membrane_dVdt_Resulting;}
    double dVdtResultingLast_Get(void) {return m_Membrane_Last_dVdt;}
    double dVdtAIS_Get(void) {return m_Membrane_dVdt_AIS;}
    double dVdtInput_Get(void) {return m_Input_dVdt + m_Membrane_dVdt_Rushin;}
    double I_AIS_Get(void){return m_AIS_I;}
    double I_Na_Get(void){return m_Na_I;}
    double I_Resulting_Get(void){return m_Resulting_I;}
    vector<double> *MembraneParameters_Get() {return &m_MembraneParameters;}
    vector<double> *RushinParameters_Get() {return &m_RushinParameters;}
    void RushinParameters_Set(uint32_t index, double value) {m_RushinParameters[index] = value;}
    void MembraneParameters_Set(uint32_t index, double value) {m_MembraneParameters[index] = value;}
protected:

    double m_Membrane_V_Rushin;             // Only for tracing
    double m_Input_dVdt;                    // The sum of the input gradients
    double m_Membrane_dVdt_AIS;
    double m_Membrane_dVdt_Resulting;
    double m_Membrane_dVdt_Rushin;          // The rushin-only current
    double m_Membrane_Last_dVdt;
    double m_AIS_I;                         // Current through the AIS
    double m_Na_I;
    double m_Resulting_I;

    vector<double> m_RushinParameters;      // Parameters for the rushin current
    vector<double> m_NaParameters;
    vector<double> m_SynapticParameters;    // Parameters for the axonal input current
    vector<double> m_MembraneParameters;    // Parameters for the membrane
};// of class NeuronPhysical
/** @}*/
#endif // NEURONPHYSICS_H

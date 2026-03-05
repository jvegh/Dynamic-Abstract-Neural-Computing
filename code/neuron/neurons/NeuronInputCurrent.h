/** @file NeuronInputCurrent.h
 *  @brief Function prototypes for the physics-based neuron module's input current
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef NEURONINPUTCURRENT_H
#define NEURONINPUTCURRENT_H
//#include <iostream>
#include <vector>
#include <cassert>
class NeuronPhysical;

class scGenComp_PU_Bio;   // Needed for remembering which neuron's current

using namespace std;

/** @addtogroup GENCOMP_MODULE_BIOLOGY
 *  @{
 */

/**
 * Defines the type of an input current
 */
enum class NeuronInputCurrent_t
{   nict_RushIn,    //< Rush-in current
    nict_PSP,       //< Post-synaptic current \f$Ampl*(1-exp(-\alpha*t)*(-\beta*t)\f$
    nict_Square,    //< A square wave
};

/*!
 * \class NeuronInputCurrent
 * \brief  Implements a general input current for physical-type neuron
 *
 * In rush-in and synaptic input types, currents are descibed
 * by a current contribution in which a saturating charge-up function
 * is multiplied by a discharge function
 *
 * The input currents produce a gradient on the condenser.
 * The input current is \f$I=dQ/dt\f$, that produces a gradient \f$ dV/dt = d(Q/C)/dt = I/C \f$
 *
 * The \f$I_i\f$ current is provided in pA  (i.e. in \f$10^-{12}\frac{Cb}{s}\f$,
 * so it causes on a capacitor \f$C\f$ (given in [pF]) and on a resistor \f$R\f$ \f[I_i*R\f$ [mV]  voltage.
 *
 * A current gradient \f$\frac{I_i}{dt} \quad [\frac{pA}{s}]\f$ causes
 * on a capacitor \f$C\f$ (given in [pF]) a voltage gradient \f$\frac{1}{C]\frac{dIi}{dt}\f$
 * increase
 * \f[I_i/C\f$ [V/s],
 *
 *
 *
 * @see NeuronConstants
 *
 *
 * The input currents are created by the ParentNeuron.
 * - The Rush-in current is created when the stage "Delivering" begins
 * - The synaptic currents are created when synaptic inputs received from the upstream neurons
 * - The artificial current begins when the experimenter switches it on
 */

class NeuronInputCurrent
{
public:
    /*!
     * \brief Creates an input current for a physics-based neuron unit
     * @param[in] ParentNeuron the owner neuron
     * @param[in] t type of the input current
     * @param[in] BeginTime the beginning of the current's time scale [msec]
     * @param[in] Parameters the parameters of the current
     */
    NeuronInputCurrent(NeuronPhysical* ParentNeuron,
        NeuronInputCurrent_t t,
        double BeginTime,
        std::vector<double> &Parameters
        );
    virtual ~NeuronInputCurrent(void);
    /**
     * @brief CurrentValue_Get
     * @param TT time of the current on the current's time scale to be calculated at [msec]
     * @return value of current at the value TT, in [pA]
     */
    double CurrentValue_Get(double TT);
    /**
     * @brief CurrentGradient_Get
     * @param TT time of the current on the current's time scale to be calculated at [msec]
     * @return value of current gradient at the value TT, in [pA/s]
     */
    double CurrentGradient_Get(double TT);
    /**
     * @brief VoltageValue_Get
     * @param TT time of the current on the current's time scale to be calculated at [msec]
     * @return value of voltage at the value TT,  the current causes of the parent neuron's membrane, in [V]
     */
    double VoltageValue_Get(double TT);
    /**
     * @brief VoltageGradient_Get
     * @param TT
     * @return the value of the voltage gradient the current causes of the parent neuron's membrane, in [V/s]
     */
    double VoltageGradient_Get(double TT);
  protected:
    NeuronPhysical* m_ParentNeuron;
    vector <double> m_Parameters;
    NeuronInputCurrent_t m_CurrentType;
    double
        m_BeginTime;    ///< The beginning of the current, local time, [ms]
 };// of class NeuronInputCurrent
/** @}*/
#endif // NEURONINPUTCURRENT_H

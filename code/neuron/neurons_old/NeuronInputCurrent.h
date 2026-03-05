/** @file NeuronInputCurrent.h
 *  @brief Function prototypes for the physics-based neuron module's input current
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef NEURONINPUTCURRENT_H
#define NEURONINPUTCURRENT_H
#include <iostream>
#include <vector>
#include <cassert>
class NeuronPhysical;

class scGenComp_PU_Bio;   // Needed for remembering which neuron's current

using namespace std;

/** @addtogroup GENCOMP_MODULE_BIOLOGY
 *  @{
 */

enum class NeuronInputCurrent_t
{   nict_RushIn,    //< Rush-in current
    nict_PSP,       //< Post-synaptic current
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
 * The input currents are created by the ParentNeuron.
 * - The Rush-in current is created when the stage "Delivering" begins
 * - The synaptic currents are created when synaptic inputs received from the upstream neurons
 * - The artificial current begins when the experimenter switches it in
 */

class NeuronInputCurrent
{
public:
    /*!
     * \brief Creates a physics-based neuron unit
     * @param[in] ParentNeuron the owner neuron
     * @param[in] t type of the input current
     * @param[in] BeginTime the beginning of the current's time scale
     * @param[in] Parameters the parameters of the current
     */
    NeuronInputCurrent(NeuronPhysical* ParentNeuron,
        NeuronInputCurrent_t t,
        double BeginTime,
        std::vector<double> &Parameters
        );
    virtual ~NeuronInputCurrent(void);
    double Value_Get(double TT);
    double Gradient_Get(double TT);
protected:
    NeuronPhysical* m_ParentNeuron;
    vector <double> m_Parameters;
    NeuronInputCurrent_t m_CurrentType;
    double
        m_BeginTime;    ///< The beginning of the current, local time, [ms]
 };// of class NeuronInputCurrent
/** @}*/
#endif // NEURONINPUTCURRENT_H

/** @file DemoNeuronPhysical.h
 *  @ingroup GENCOMP_MODULE_DEMO

 *  @brief A unit to implement demos for a physics-based neuron-like processing unit
 *  It has the basic functionality of biological neurons, and produces different test sequences
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef DEMONEURONPHYSICAL_H
#define DEMONEURONPHYSICAL_H
/* * @addtogroup GENCOMP_MODULE_DEMO
 *  @{
 */

#include "NeuronPhysical.h"

/**
 * \class DemoNeuronPhysical
 * \brief  A simple class to implement demos for the operation of a general neuron-like computing unit
 */

class DemoNeuronPhysical : public NeuronPhysical
{
public:
    /*!
     * \brief Creates a test neuron-like general computing unit
     * @param[in] nm the SystemC name of the module
     *
     * Creates a class for testing the neuron-like general biological computing unit.
     * A template for developing and testing your own first units
     *
     * Solves PDE, but only imitates inputs
     */
    DemoNeuronPhysical(sc_core::sc_module_name nm   // Module name
                         );
    ~DemoNeuronPhysical(void);
    void GenerateSimpleSingleAP();
    sc_core::sc_time PU_InitialDelayTime = sc_core::sc_time(100,SC_US);
    sc_core::sc_time PU_InputTime1       = sc_core::sc_time(190,SC_US); // Normal input
    sc_core::sc_time PU_InputTime2       = sc_core::sc_time(211,SC_US); // Normal input
    sc_core::sc_time PU_InputTime3       = sc_core::sc_time(235,SC_US); // Normal input
    sc_core::sc_time PU_InputTime4       = sc_core::sc_time(261,SC_US); // Normal input
    sc_core::sc_time BaseTime;
 };
/* * @}*/

#endif // DEMONEURONPHYSICAL_H

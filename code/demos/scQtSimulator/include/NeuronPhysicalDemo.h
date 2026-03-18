/** @file NeuronPhysicalDemo.h
 *  @ingroup GENCOMP_MODULE_DEMO

 *  @brief A unit to implement a physics-based neuron-like processing unit
 *  It has the basic functionality of biological neurons, and produces the conceptual diagram
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/


#ifndef NEURONPHYSICALDEMO_H
#define NEURONPHYSICALDEMO_H
/** @addtogroup GENCOMP_MODULE_DEMO
 *  @{
 */

#include "NeuronPhysical.h"

/**
 * \class NeuronPhysicalDEMO
 * \brief  A simple demo class to implement the operation of a general neuron-like computing unit
 *
 *
 */

class NeuronPhysicalDEMO : public NeuronPhysical
{
public:
    /*!
     * \brief Creates a demo neuron-like general computing unit
     * @param[in] nm the SystemC name of the module
     *
     * Creates a class for testing the neuron-like general biological computing unit.
     * A template for developing and testing your own first units
     *
     * Solves PDE, but only imitates inputs
     */
    NeuronPhysicalDEMO(sc_core::sc_module_name nm   // Module name
                         );
    ~NeuronPhysicalDEMO(void);
 };
/** @}*/

#endif // SCBIOGENCOMPNEURONDEMO_H

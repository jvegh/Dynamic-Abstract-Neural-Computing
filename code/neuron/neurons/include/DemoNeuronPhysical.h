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
 * \brief  A simple generic base class to implement demos for the operation of a general neuron-like computing unit.
 * It does not generate events and does not have pre-set parameters
 */

class DemoNeuronPhysical : public NeuronPhysical
{
public:
    /*!
     * \brief Creates a test neuron-like general computing unit
     * @param[in] nm the SystemC name of the module
     *
     * Creates a base class for testing the neuron-like general biological computing unit.
     * A template for developing and testing your own first units
     *
     * Solves PDE, but only imitates inputs
     */
    DemoNeuronPhysical(sc_core::sc_module_name nm   // Module name
                         );
    ~DemoNeuronPhysical(void);
    virtual void ClearEvents() = 0;
    virtual void SetParameters() = 0;
    virtual void GenerateEvents() = 0;
 };


/**
 * \class DemoNeuronPhysical
 * \brief  A simple base class to implement demos for the operation of a general neuron-like computing unit.
 * It generates events for the unit being under the control of the simulator
 */

 class DemoSimpleSingleAP : public DemoNeuronPhysical
 {
 public:
     DemoSimpleSingleAP(sc_core::sc_module_name nm   // Module name
                        );
     ~DemoSimpleSingleAP(void);
     virtual void SetParameters();
     virtual void GenerateEvents();
     virtual void ClearEvents();
     sc_core::sc_time PU_InitialDelayTime = sc_core::sc_time(100,SC_US);
     sc_core::sc_time PU_InputTime1       = sc_core::sc_time(190,SC_US); // Normal input
     sc_core::sc_time PU_InputTime2       = sc_core::sc_time(211,SC_US); // Normal input
     sc_core::sc_time PU_InputTime3       = sc_core::sc_time(235,SC_US); // Normal input
     sc_core::sc_time PU_InputTime4       = sc_core::sc_time(261,SC_US); // Normal input
     sc_core::sc_time BaseTime;
 };

 /**
 * \class DemoNeuronPhysical
 * \brief  A simple base class to implement demos for the operation of a general neuron-like computing unit.
 * It generates events for the unit being under the control of the simulator
 */

 class DemoSimpleFailedAP : public DemoNeuronPhysical
 {
 public:
     DemoSimpleFailedAP(sc_core::sc_module_name nm   // Module name
                        );
     ~DemoSimpleFailedAP(void);
     virtual void SetParameters();
     virtual void GenerateEvents();
     virtual void ClearEvents();
    sc_core::sc_time PU_InitialDelayTime = sc_core::sc_time(100,SC_US);
    sc_core::sc_time PU_InputTime1       = sc_core::sc_time(190,SC_US); // Normal input
    sc_core::sc_time PU_InputTime2       = sc_core::sc_time(211,SC_US); // Normal input
    sc_core::sc_time BaseTime;
 };

/* * @}*/

#endif // DEMONEURONPHYSICAL_H

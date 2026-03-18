/** @file GenCompObserveType.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *
 *  @brief Enums and string for the
   */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef GENCOMPOBSERVETYPE_H
#define GENCOMPOBSERVETYPE_H
/** @addtogroup GENCOMP_MODULE_PROCESS
     *  @{
         */

#include <bitset>
#include "scGenComp_Config.h"

/*!
 * \class GenCompObserve_t
 * \brief the names of the bits in the bitset describing
 * which signals in scGenComp_PU_Abstract are monitored by the scGenComp_Simulator
 */
enum class GenCompObserve_t
{   gcob_ObserveGroup,              ///< The module's group is observed (by the simulator)
    gcob_ObserveModule,             ///< The module is observed (by the simulator)
    gcob_ObserveComputingBegin,     ///< Observe 'ComputingBegin'
    gcob_ObserveComputingEnd,       ///< Observe 'ComputingEnd'
    gcob_ObserveDeliveringBegin,    ///< Observe 'DeliveringBegin'
    gcob_ObserveDeliveringEnd,      ///< Observe 'DeliveringEnd'
    gcob_ObserveFailed,             ///< Observe 'Failed's of the PU
    gcob_ObserveHeartbeat,          ///< Observe 'Heartbeat's of the PU
    gcob_ObserveInputReceived,      ///< Observe 'Input's of the PU
    gcob_ObserveInitialize,         ///< Observe 'Initialize' of the PU
    gcob_ObserveRelaxingBegin,      ///< Observe 'RelaxingBegin'
    gcob_ObserveRelaxingEnd,        ///< Observe 'RelaxingEnd'
    gcob_ObserveSynchronize,        ///< Observe 'Synchronize'
    gcob_ObserveValueChanged,       ///< Observe 'Value changed'
    gcob_SIZE };                       // just maintains the number of bits used
//};

 /** @}*/

#endif // GENCOMPOBSERVETYPE_H

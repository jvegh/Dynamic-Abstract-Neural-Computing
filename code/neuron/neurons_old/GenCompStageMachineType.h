/** @file GenCompStageMachineType.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *
 *  @brief Descriptive enums for the stage machine
 *  Just event and stage handling, no real activity
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef GENCOMPSTAGEMACHINETYPE_H
#define GENCOMPSTAGEMACHINE_H

/** @addtogroup GENCOMP_MODULE_DEMO Demos and tutorials
 *  @{
 */

#include <cstdint>
using namespace std;
//#include <bitset>

/*! \class GenCompStageMachine_t
 * The operation of an elementary computing unit of general computing is modelled as a multiple-state machine-like structure,
 * with internal stage stored a variable scGenComp_PU_Abstract::mStageFlag.
@verbatim

                   Sleeping<------> Relaxing     <---Initializing
                                   ^   ^     ^  \   ^
                                   |   HBeat |   \  |-InputReceived
                                   |         |   v  v
                                   |   Failed-   Computing
                                   | Synchronizing /
                                   | |            /
                                   | v           v
                                  Delivering    <-----Synchronizing
 @endverbatim

The stage changing are implemented in two steps. The non-virtual methods XXX_method()
(usually, hidden from the users of derived classes) process all needed activity
for the correct sequencing of stages and handling event-related activity.
(They are declared as 'private' to prevent non-intended activity of users.)
They call XXX_Do() functions (may be overloaded by users) which may implement
additional activity. If needed, the upstream object's XXX_Do()
can be called within those functions.

 * - The stages below are momentary: need action and pass to one of the above stages
 * - Failing : Something went wrong, reset unit and pass to Relaxing
 * - Synchronizing: deliver result, anyhow ;
 *   - in biological mode, deliver immediate spike
 *   - in technical mode, deliver immediate result (such and in a \gls{GPU})
 *   Passes to Relaxing (after issuing 'End Computing')
*/

/*! \var GenCompStageMachine_t::gcsm_Sleeping
 *  Essentially, same as gcsm_Relaxing, but no Heartbeats,
 *  and even the HW unit can go to sleep (not yet implemented)
 *
 *  Begins in stage gcsm_Relaxing, when sleeping condition met
 *  (not yet entirely implemented), and returns to
 *   gcsm_Relaxing (temporarily, to  gcsm_Computing
 */

/*! \var GenCompStageMachine_t::gcsm_Relaxing
  The abstract units are generated in this stage: everything is prepared, variables available.
  The unit is ready, waiting for EVENT_GenComp_type::ComputingBegin;
  after receiving it, passes to GenCompStageMachineType_t::gcsm_Computing.
  ComputingBegin can be provided by clock signal (if clock driven) or InputReceived.
  Important: changing to stage gcsm_Relaxing does not involve reseting!!

  Begins with   EVENT_GenComp_type::RelaxingBegin

    Normally passes to gcsm_Computing
*/

/*! \var GenCompStageMachine_t::gcsm_Delivering
    Makes internal result externally available (delivering its result to its output section).
    Normally goes to gcsm_Relaxing.  An external Synchronizing may force immediate Delivering
 * - Delivering: The unit is
 *   - after some activity, passes to 'Relaxing'
 *   (it sends the result to its chained unit(s))
 *   it Sends 'Begin Transmitting', a parallel activity)
 *
 *  Spans time between EVENT_GenComp_type::DeliveringBegin and EVENT_GenComp_type::DeliveringEnd
 */

/*! \var GenCompStageMachine_t::gcsm_Computing
  Computes (may receive inputs), then passes to  stage gcsm_Delivering.

  Spans time between EVENT_GenComp_type::ComputingBegin and EVENT_GenComp_type::ComputingEnd
*/

/*! \var GenCompStageMachine_t::gcsm_Initializing
 *  Initilizes the unit instantly. Passes to GenCompStageMachineType_t::gcsm_Relaxing
 *
 *
 * Begins at EVENT_GenComp_type::Initialize.
 */

/*! \var GenCompStageMachine_t::gcsm_InputReceived
 *  Receives input instantly. (input arguments are delivered to the input section)
 *  If not clocked, may pass to GenCompStageMachineType_t::gcsm_Computing
 *
 *  A momentary stage
 *
 * Begins at EVENT_GenComp_type::InputReceived
 */

/*! \var GenCompStageMachine_t::gcsm_Synchronizing
 *  Synchronizes the operation to fellow PUs instantly
 *  (passes immediately to GenCompStageMachineType_t::gcsm_Delivering,
 *  except when in state GenCompStageMachineType_t::gcsm_Delivering).
 *
 * A momentary stage
 *
 * Begins at EVENT_GenComp_type::Synchronize
 */

/*! \var GenCompStageMachine_t::gcsm_Failed
 *  Enables to simulate failed operations instantly.
 *  (Actually, re-initializes unit)
 *
 * Begins at EVENT_GenComp_type::Failed
 */


enum class  GenCompStageMachine_t :int8_t {
    gcsm_Sleeping=0, /*!< same as gcsm_Relaxing, but no Heartbeats */
    gcsm_Relaxing,/*!< Relaxing */
    gcsm_Computing,
    gcsm_Delivering,
    gcsm_Initializing,
    gcsm_InputReceived,
    gcsm_Synchronizing,
    gcsm_Failed,
    gcsm_SIZE
};

#include <string>

using namespace std;

//extern string GenCompStagesString[];
const char* GenCompStageMachineType2String(GenCompStageMachine_t e);
/** @}*/

#endif // SCGENCOMPSTAGEMACHINE_H

/** @file GenCompObserveType.cpp
 *  @brief enums for observing a module
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "GenCompObserveType.h"

//#include <iostream>
#include <map>
#include <cassert>

const char* GenCompObserveType2String(GenCompObserve_t e)
{
    using OType = std::map<GenCompObserve_t, const char*>;
    static const OType OMap =
        {
            {GenCompObserve_t::gcob_ObserveGroup, "Group"},
            {GenCompObserve_t::gcob_ObserveModule, "Module"},
            {GenCompObserve_t::gcob_ObserveComputingBegin, "ComputingBegin"},
            {GenCompObserve_t::gcob_ObserveComputingEnd, "ComputingEnd"},
            {GenCompObserve_t::gcob_ObserveDeliveringBegin, "DeliveringBegin"},
            {GenCompObserve_t::gcob_ObserveDeliveringEnd, "DeliveringEnd"},
            {GenCompObserve_t::gcob_ObserveFailed, "Failed"},
            {GenCompObserve_t::gcob_ObserveHeartbeat, "Heartbeat"},
            {GenCompObserve_t::gcob_ObserveInputReceived, "Input"},
            {GenCompObserve_t::gcob_ObserveInitialize, "Initialize"},
            {GenCompObserve_t::gcob_ObserveRelaxingBegin, "RelaxingBegin"},
            {GenCompObserve_t::gcob_ObserveRelaxingEnd, "RelaxingEnd"},
            {GenCompObserve_t::gcob_ObserveSynchronize, "Synchronize"},
            {GenCompObserve_t::gcob_ObserveSynchronize, "Value changed"}
          };
    assert(OMap.size() != (int)GenCompObserve_t::gcob_SIZE);
    return OMap.at(e);
}

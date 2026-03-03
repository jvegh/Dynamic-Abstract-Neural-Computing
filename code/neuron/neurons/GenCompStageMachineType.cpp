/** @file GenCompStageMachineType.cpp
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief  The abstract processing unit for generalized computing
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "GenCompStageMachineType.h"

//#include <iostream>
#include <map>
#include <cassert>


const char* GenCompStageMachineType2String(GenCompStageMachine_t e)
{
    using MapType = std::map<GenCompStageMachine_t, const char*>;
    static const MapType SMap =
        {
            {GenCompStageMachine_t::gcsm_Sleeping, "Sleeping"},
            {GenCompStageMachine_t::gcsm_Relaxing, "Relaxing"},
            {GenCompStageMachine_t::gcsm_Computing, "Computing"},
            {GenCompStageMachine_t::gcsm_Delivering, "Delivering"},
            {GenCompStageMachine_t::gcsm_Initializing,"Initializing"},
            {GenCompStageMachine_t::gcsm_InputReceived,"InputReceived"},
            {GenCompStageMachine_t::gcsm_Synchronizing, "Synchronizing"},
            {GenCompStageMachine_t::gcsm_Failed, "Failed"}
        };
    assert(SMap.size() == (int)GenCompStageMachine_t::gcsm_SIZE);
    return SMap.at(e);
}

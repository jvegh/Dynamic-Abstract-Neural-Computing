/** @file Constants.h
 *  @brief Constants used in the neuronal computations
 *  Header only
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include <cmath>
#include <cstdint>
#include <string>
#include <cmath>
#ifndef CONSTANTS_H
#define CONSTANTS_H
using namespace std;
/* * @addtogroup GENCOMP_MODULE_PROCESS
 *  @{
 */

/// Define neuron's constants
/// Sources:

/*!
 * \class NeuronConstant
 * \brief  Provide neuron-related constants in various relations
 *
 */

class NeuronConstants
{
public:
    /*!
     * \brief Creates a physics-based neuron unit
     * @param nm the SystemC name of the module
     */
    NeuronConstants(void):
         m_CurrentResolutionExponent(-9)
        ,m_ResistanceResolutionExponent(+3)
        ,m_TimeResolutionExponent(-6)
        ,m_VoltageResolutionExponent(-6)
        ,m_Membrane_R(100000) // Value of membrane resistance, stored in kOhm (100 MOhm)
        ,m_Membrane_C(100000) // Value of membrane capacitance, stored in fF (100 pF)
        ,m_Membrane_Tau(10000) // The membrane's time constant, stored in microsec (10 ms)
    {

    }

    virtual ~NeuronConstants(void){}// Must be overridden
    int8_t CurrentResolutionExponent_Get(void)
        {return m_CurrentResolutionExponent;}
    int8_t VoltageResolutionExponent_Get(void)
        {return m_CurrentResolutionExponent;}

    void CurrentResolution_Set(int8_t E)
        {   m_CurrentResolutionExponent = E; }
    string CurrentResolutionUnit_Get(void)
        {   switch(m_CurrentResolutionExponent)
            {
            default: return "Bad";
                case -9: return "pA";
                case -6: return "uA";
                case -3: return "mA";
            }
        }
       string VoltageResolutionUnit_Get(void)
        {   switch(m_CurrentResolutionExponent)
            {
            default: return "Bad";
                case -9: return "pV";
                case -6: return "uV";
                case -3: return "mV";
            }
        }
        double MembraneResistance_Get()
        {return m_Membrane_R*exp(m_ResistanceResolutionExponent);}
public:
    int8_t
         m_CurrentResolutionExponent
        ,m_ResistanceResolutionExponent
        ,m_TimeResolutionExponent
        ,m_VoltageResolutionExponent;
    int32_t m_Membrane_R; //< Value of membrane resistance, stored in kOhm (100 MOhm)
    int32_t m_Membrane_C; //< Value of membrane capacitance, stored in fF (100 pF)
    int32_t m_Membrane_Tau; //< The membrane's time constant, stored in microsec (10 ms)
 };// of class NeuronConstants


class UniversalConstants
{
public:
    /*!
     * \brief Creates a physics-based neuron unit
     * @param nm the SystemC name of the module
     */
    UniversalConstants(void){}

    virtual ~UniversalConstants(void)    {}// Must be overridden
    float Value_Get(double TT);
    float Gradient_Get(double TT);
protected:
};// of class NeuronConstants
/* * @}*/
#endif // CONSTANTS_H

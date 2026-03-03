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
/** @addtogroup GENCOMP_MODULE_BIOLOGY
 *  @{
 */

/// Sources:

/*!
 * \class NeuronConstants
 * \brief  Provide neuron-related constants in various relations for neural computing
 *
 * Define neuron's constants
 * Provides access functions to the parameters of the neuronal condenser and resistor
 *
 *The system uses numbers in units:
 * - resistivity GOhm so [R] (\f$10^9\f$ Ohm)
 * - capacity pF  so [C] (\f$10^{-12}\f$ Farad
 * - time ms so [t] (\f$10^{-3}\f$), so [R*C] is (\f$10^9*10^{-12}=10^{-3}\f$
 * - voltage mV so [V] is (\f$10^{-3}\f$)
 * - current pA (\f$10^{-12}\f$)
 * -  so current from V/R \f$ 10^{-3]/10^{9}=10^{-12} \f$ pA
 * -  so [I/C] (\f$10^{-12}/10^{-12}\f$ V/s)
 * -  so \f$ \frac{1}{C}\frac{dI}{dt} = [A/s/(As)*V] = [Cb/s^2] \f$
 * -  so [I*R] (\f$ 10^{-12}*10^9\f$ = mV)
 * -  so [V/C] (\f$10^{-3}/10^{-12} = 10^9 V/m\f$)
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
        ,m_TimeResolutionExponent(-3)
        ,m_VoltageResolutionExponent(-6)
/*        ,m_Membrane_R(0.1*1E9) // Value of membrane resistance, stored in GOhm (100 MOhm)
        ,m_Membrane_C(100.*1E-12) // Value of membrane capacitance, stored in pF (100 pF)
        ,m_Membrane_Tau(10.*1E-3) // The membrane's time constant, stored in msec (10 ms)
*/
    {
        Use_KochSet();
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
        /**
         * @brief Use data set from @cite KochBiophysics:1999
         */
        void Use_KochSet()
        {
            MembraneResistanceGOhm_Set(0.1);
            MembraneCapacityPF_Set(100.);
        }
        /**
         * @brief Use data set from @cite JohnstonWuNeurophysiology:1995
         */
        void Use_JohnstonSet()
        {

        }
        /**
         *
         * @return membrane's resistance [Ohm]
         */
        double MembraneResistanceOhm_Get()
        {   return m_Membrane_R*1e9;}
        /**
         *
         * @return membrane's resistance [GOhm]
         */
        double MembraneResistanceGOhm_Get()
        {   return m_Membrane_R; }
        void MembraneResistanceOhm_Set(double R)
        {   MembraneResistanceGOhm_Set(R*1E9);
        }
        /**
         * Set membrane's resistance
         * @param R resistance [GOhm]
         */
        void MembraneResistanceGOhm_Set(double R)
        {   m_Membrane_R = R;
            MembraneTauMSec_Set();
        }
        /**
         * @return mebrane's capacitance [pF]
         */
        double MembraneCapacityPF_Get(void)
        {   return m_Membrane_C; }
        //
        void MembraneCapacityPF_Set(double C)
        {   m_Membrane_C = C;
            MembraneTauMSec_Set();
        }
        void MembraneCapaciyF_Set(double C)
        {   m_Membrane_C = C*1E-12;
            MembraneTauMSec_Set();
        }
        double MembraneTauMSec_Get(void)
        {   return m_Membrane_Tau;}
        void MembraneFromRGOhm_TauMSec_Set(double R, double T)
        {
            MembraneResistanceGOhm_Set(R);
            m_Membrane_C = T/R;
        }
        void MembraneFromCPF_TauMSec_Set(double C, double T)
        {
            MembraneCapacityPF_Set(C);
            m_Membrane_R = T/C;
        }
        void MembraneTauMSec_Set(void)
        {   m_Membrane_Tau = m_Membrane_C* m_Membrane_R;}
        /// Set the membrane's time constant
        void MembraneFromRC_TauMSec_Set(double R, double C)
        {
            MembraneCapacityPF_Set(C);
            MembraneResistanceGOhm_Set(R);
        }

public:
    int8_t
         m_CurrentResolutionExponent
        ,m_ResistanceResolutionExponent
        ,m_TimeResolutionExponent
        ,m_VoltageResolutionExponent;
protected:
    double m_Membrane_R; //< Value of membrane resistance, stored in Ohm (100 MOhm)
    double m_Membrane_C; //< Value of membrane capacitance, stored in F (100 pF)
    double m_Membrane_Tau; //< The membrane's time constant, stored in sec (10 ms)
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
/** @}*/
#endif // CONSTANTS_H

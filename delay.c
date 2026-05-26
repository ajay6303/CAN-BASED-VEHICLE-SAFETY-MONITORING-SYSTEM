#include "delay.h"

typedef unsigned int u32;

/**
 * @brief Generates microsecond timing delays using basic CPU spin-lock loop math configurations.
 */
void delay_us(u32 dlyus)
{
    for(dlyus *= 12; dlyus > 0; dlyus--);
}

/**
 * @brief Generates millisecond timing delays using basic CPU spin-lock loop math configurations.
 */
void delay_ms(u32 dlyms)
{
    for(dlyms *= 12000; dlyms > 0; dlyms--);
}

/**
 * @brief Generates whole second timing delays using basic CPU spin-lock loop math configurations.
 */
void delay_s(u32 dlys)
{
    for(dlys *= 12000000; dlys > 0; dlys--);
}

#ifndef WAIT_H
#define WAIT_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma message ("wait.ms")
#if defined(__AVR__)
#pragma message ("__AVR__")
#   include <util/delay.h>
#   define wait_ms(ms)  _delay_ms(ms)
#   define wait_us(us)  _delay_us(us)
#elif defined PROTOCOL_CHIBIOS
#pragma message ("PROTOCOL_CHIBIOS")
#   include "ch.h"
#   define wait_ms(ms) chThdSleepMilliseconds(ms)
#   define wait_us(us) chThdSleepMicroseconds(us)
#elif defined PROTOCOL_ARM_ATSAM
#pragma message ("PROTOCOL_ARM_ATSAM")
#   include "clks.h"
#   define wait_ms(ms) CLK_delay_ms(ms)
#   define wait_us(us) CLK_delay_us(us)
#elif defined(__arm__)
#pragma message ("__arm__")
#   include "wait_api.h"
#else  // Unit tests
#pragma message ("else")
void wait_ms(uint32_t ms);
#define wait_us(us) wait_ms(us / 1000)
#endif

#ifdef __cplusplus
}
#endif

#endif

#ifndef AVR_COMMON_H
#define AVR_COMMON_H
#endif

/**
 * Define some useful macros that operate on individual bits
 * in a register.

*/
// Set a bit in a register to 0
#define DISABLE(REGISTER, BIT) (REGISTER) &= ~(1 << BIT)
// Set a bit in a register to 1
#define ENABLE(REGISTER, BIT) (REGISTER) |= (1 << BIT)
// Toggle a bit in a register
#define TOGGLE(REGISTER, BIT) (REGISTER) = (REGISTER) ^ (1 << BIT)
// Toggle a bit in a register twice
#define TRIGGER(REGISTER, BIT)                                                 \
  do {                                                                         \
    TOGGLE(REGISTER, BIT);                                                     \
    TOGGLE(REGISTER, BIT);                                                     \
  } while (0)
// Check if a bit in a register is 1
#define IS_SET(REGISTER, BIT) (REGISTER & (1 << BIT)) > 0
/**
 * @note: This bug has been fixed! I will leave everything in this file the way
 * that it is, the only thing I will do is explain the fix at the end of the
 * file.
 */

/**
 * @contact_info:
 * Author: dev_jeb
 * Email: developer_jeb@outlook.com
 *
 * @purpose: Here I will document and hopefully fix a bug that has kept me up at
 * night for the past 2 weeks. So much so that I have considered stopping
 * development on this repo and throwing in the towel. I would just use
 * avr-libc. I really do not want to do that.
 *
 * @important_notes:
 *
 * - This bug has immdeiate implications to how you use the
 *   usart module. Please see the implications section at the end of this file.
 *
 * - See the usart.h file in /utils/include for the macros of the usart module.
 *
 * - Notice that I have included a copy of the utils folder in this project. I
 * may have found a solution to this problem that will involve changing utils.
 * This directory contains everything needed to reproduce this bug at the
 * current state.
 *
 * - This bug was compiled using the avr-gcc compiler version 5.4.0.
 */

#include "avr-arch.h"
#include "common.h"
#include "types.h"
#include "usart.h"

/**
 * @bug_info: I am trying to achieve one simple task. Send a string over usart.
 * The best way I can explain this bug is with the example below. I dont think I
 * can solve this problem. But if I document the hell out of it maybe someone
 * smarter then me can.
 *
 * When SWITCH == 0 the program sends the string "Hello World" over usart twice.
 * Once from inside main and once from foo using a pointer as a medium. Notice
 * how I use a constant as the length of the string in foo. If I use
 * strlen(str_ptr) it loops forever.
 *
 * When SWITCH == 1 the program loops forever. I have distilled it down to the
 * call to strlen on the ptr.
 *
 * When SWITCH == 2 the usart modeule acts as expected.
 *
 * @it_gets_worse: This bug is dependant on the optimization level used to
 * compile it. When compiled with the flag -Os you get expected behavior from
 * SWITCH == 2. Other optimization levels cause undefined behavior. This also
 * has implications on how to use this module.
 */

#define SWITCH 0

#if SWITCH == 0

void foo(uint8_ptr_t str_ptr) { usart0_transmit_bytes(str_ptr, 11); }

int main(void) {
  usart0_init(103);
  uint8_ptr_t str_ptr = "Hello World";
  usart0_transmit_bytes(str_ptr, strlen(str_ptr));
  foo(str_ptr);
  return 0;
}

#elif SWITCH == 1

void foo(uint8_ptr_t str_ptr) {
  usart0_transmit_bytes(str_ptr, strlen(str_ptr));
}

int main(void) {
  usart0_init(103);
  uint8_ptr_t str_ptr = "Hello World";
  usart0_transmit_bytes(str_ptr, strlen(str_ptr));
  foo(str_ptr);
  return 0;
}

#elif SWITCH == 2

void foo() {
  uint8_ptr_t str1_ptr = "Hello World";
  usart0_transmit_bytes(str1_ptr, strlen(str1_ptr));
}

int main(void) {
  usart0_init(103);
  uint8_ptr_t str_ptr = "Hello World";
  usart0_transmit_bytes(str_ptr, strlen(str_ptr));
  foo();
  return 0;
}

#endif

/**
 * @steps_to_reproduce:
 * 1. Set the SWITCH value to 0 or 1 depending on what you want.
 * 2. Compile the code using the makefile. Command `make`
 * 3. Flash the code your microcontroller using avrdude
 *
 *    avrdude -p atmega328p -c arduino -P /dev/<serial_device_name> \
 *     -B 9600 -U flash:w:main.hex
 *
 * 4. Open a serial monitor and weap. I use
 *
 *    minicom -b 9600 -D /dev/<serial_device_name>
 */

/**
 * @fix:
 * The bug lies in the implementation of the strlen function. When we get the
 * length of the string we use the syntactic sugar str++ which expands to
 * str = str + 1. Therefore we are actually changing and reassiging the pointer.
 * If we wanted to print the string twice we would call strtlen() twice. The
 * second time we call it the pointer is pointing to the end of the string. We
 * get an infinite loop. This is the same reason that SWITCH == 1 fails if we
 * didn't use the constant 11. The fix would be to add an index variable to keep
 * track of how faw we have gone. The usart implementation looks different now
 * but we still have to apply this thinking.
 *
 *  void usart0_transmit_bytes(uint8_ptr_t ptr)
 *  {
 *    uint32_t index = 0;
 *    while (*(ptr + index))
 *    {
 *     usart0_transmit_byte(*(ptr + index));
 *     index++;
 *    }
 *  }
 *
 * take note of the index variable. This is the fix.
 */

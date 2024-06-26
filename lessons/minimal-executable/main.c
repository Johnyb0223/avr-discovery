/**
 * @contact_info:
 * Author: dev_jeb
 * Email: developer_jeb@outlook.com
 *
 * @usage:
 * Ensure you have successfully setup the development container described in the
 * README or ensure you have the necessary tools installed locally on you
 * machine. You can build this lesson using the `make` command
 *
 * @purpose:
 * The executable created by building this directory is a minimal program that
 * will properly setup the microcontroller and call the main function defined
 * here in this file. You have complete transparency to how this is achieved.
 * The makefile basically calls the compilation driver (avr-gcc) with a metric
 * shit ton of options to the compiler, assembler, and linker. The invocation
 * of this driver produces an executable file (main.elf) in the elf32-avr file
 * format. The single dependency of the executable (program) is crt.o
 * (built from crt.s) located in the directory '/common/'. This file is by
 * default linked against as it sets up the microcontroller to a state that main
 * expects.
 *
 * If you become intimately familiar with this program and how it goes from the
 * state of three input files (crt.s, main.c, default.ld) to the final
 * executable then you will be in a good place to define an arbitrarily laid out
 * executable (program) and implement said layout. This in my opinion
 * is a beautiful, rewarding, and deceivingly complex task. enjoy!
 *
 * Here are some questions to help point your exploration
 * @important_questions:
 * 1. what is the vector table and its purpose? Why is it the length that
 *    it is? (check hardware datasheet) Is its structure and location in the
 *    microcontrollers memory mandated by the hardware? (yes)
 * 2. why are the weak symbols necessary in defining the vector table in cst.s
 *    and what functionality does this provide? (see interrupts lesson for
 *    insight)
 * 3. why does the crt.s file do what it does? what does it do?
 * 4. do you see and understand the overall flow of the final executable? (use
 *    avr-objdump for this) reset_vector -> init -> load_data -> zero_bss ->
 *    main
 * 5. what happens if an interrupt is triggered and the user did not define
 *    an interrupt handler? Where are we jumping if this occurs? (again
 *    avr-objdump on the executable is great here)
 * 6. how would the user define an interrupt handler? (see question 2)
 *
 * The insights brought on from this exploration extend far beyond the scope of
 * this one program.
 *
 * @what_you_will_discover:
 * - how relocatable object files generated by the preprocessor ->
 *   compiler -> assembler sequence are linked together to form a final
 *   executable.
 * - What object file sections are and how they are handled by the linker.
 * - What object file symbols are and how does the linker resolve them. Where
 *   are symbols generated (compiler?, assembler?, linker?, all of them?) and
 *   why would we generate symbols at different points in the compilation
 *   process.
 **/

/**
 * We will define a few variables here that will end up in some interesting
 * sections of the final executable.
 */
char i_live_in_the_data_section = 0xaa;
char i_live_in_the_bss_section;
const char i_live_in_the_rodata_section = 0x33;
int main(void) {
  char i_live_on_the_stack = 0x55;
  i_live_on_the_stack += 1;
  return 0;
}
#include "eeprom.h"

/**
 * implementation of the eeprom_write_byte function
 */
uint8_t eeprom_read_byte(uint8_t *src) {
  // ensure that the EEPROM is ready
  while (eeprom_busy()) {
  };
  // load address into EEAR
  eeprom_set_addr(src);
  // set the EERE bit in EECR
  EECR |= (1 << EERE);
  // return the data
  return EEDR;
}

/**
 * implementation of the eeprom_write_byte function
 */
void eeprom_write_byte(uint8_t *dst, uint8_t data) {
  // ensure that the EEPROM is ready
  while (eeprom_busy()) {
  };
  // load address into EEAR
  eeprom_set_addr(dst);
  // load data into EEDR
  eeprom_set_data(data);
  // set the EEPME bit in EECR
  EECR |= (1 << EEPME);
  // set the EEPE bit in EECR
  EECR |= (1 << EEPE);
}
#ifndef ASCII_TO_TRYTES_H
#define ASCII_TO_TRYTES_H
#include <stdint.h>
#include <stddef.h>

//  Conversion of ascii encoded bytes to trytes.
//  Ported from https://github.com/iotaledger/iota.lib.js/blob/master/lib/utils/asciiToTrytes.js #b55f4e3
//  Check .c for details
uint32_t toTrytes(uint8_t* input_bytes, char* output_trytes, size_t input_length);

uint32_t fromTrytes(char* input_trytes, uint8_t* output_bytes, size_t input_length);

#endif

#include "asciiToTrytes.h"
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
//  Conversion of ascii encoded bytes to trytes.
//  Ported from https://github.com/iotaledger/iota.lib.js/blob/master/lib/utils/asciiToTrytes.js #b55f4e3
//  Input is a uint8_t array, output value is NULL-terminated string of Trytes
//  Vice versa for trytes to bytes.
//
//  How the conversion works:
//    2 Trytes === 1 Byte
//    There are a total of 27 different tryte values: 9ABCDEFGHIJKLMNOPQRSTUVWXYZ
//
//    1. We get the decimal value of an individual ASCII character
//    2. From the decimal value, we then derive the two tryte values by basically calculating the tryte equivalent (e.g. 100 === 19 + 3 * 27)
//      a. The first tryte value is the decimal value modulo 27 (27 trytes)
//      b. The second value is the remainder (decimal value - first value), divided by 27
//    3. The two values returned from Step 2. are then input as indices into the available values list ('9ABCDEFGHIJKLMNOPQRSTUVWXYZ') to get the correct tryte value
//
//   EXAMPLES
//      Lets say we want to convert the ASCII character "Z".
//        1. 'Z' has a decimal value of 90.
//        2. 90 can be represented as 9 + 3 * 27. To make it simpler:
//           a. First value: 90 modulo 27 is 9. This is now our first value
//           b. Second value: (90 - 9) / 27 is 3. This is our second value.
//        3. Our two values are now 9 and 3. To get the tryte value now we simply insert it as indices into '9ABCDEFGHIJKLMNOPQRSTUVWXYZ'
//           a. The first tryte value is '9ABCDEFGHIJKLMNOPQRSTUVWXYZ'[9] === "I"
//           b. The second tryte value is '9ABCDEFGHIJKLMNOPQRSTUVWXYZ'[3] === "C"
//        Our tryte pair is "IC"
//
//      RESULT:
//        The ASCII char "Z" is represented as "IC" in trytes.
//

// Encodes input_bytes into tryte string buffer output_trytes. 
// output_trytes must have length of input_bytes + 1 (NULL)
// 
// Return error code: 0 on success, probably hardfaults on memory overflow.
uint32_t toTrytes(uint8_t* input_bytes, char* output_trytes, size_t input_length)
{
    const char TRYTE_VALUES[] = "9ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (size_t i = 0; i < input_length; i++) {
        uint8_t firstValue = input_bytes[i] % 27;
        uint8_t secondValue = (input_bytes[i] - firstValue) / 27;

        output_trytes[i*2] = TRYTE_VALUES[firstValue];
        output_trytes[(i*2)+1] = TRYTE_VALUES[secondValue];
    }

    output_trytes[(input_length*2)] = 0x00; //Make sure output string is null-terminated
    return 0;
}


//
//  Trytes to bytes
//  Reverse operation from the byteToTrytes function.
//  2 Trytes == 1 Byte
//  String has to have an even length.
//  output_bytes needs to have length of input_trytes/2.
//  return error code, 0 on success, 1 if a input character was not valid Tryte, 2 if length is odd
uint32_t fromTrytes(char* input_trytes, uint8_t* output_bytes, size_t input_length) 
{
    // If input length is odd, return error code
    if ( input_length % 2 ) { return 2; }

    //Added null to charset
    char TRYTE_VALUES[] = "9ABCDEFGHIJKLMNOPQRSTUVWXYZ\0";

    for (size_t i = 0; i < input_length; i += 2) {
        // get a tryte pair
        // https://stackoverflow.com/questions/3217629/how-do-i-find-the-index-of-a-character-within-a-string-in-c
        char *e;
        size_t index;
        e = strchr(TRYTE_VALUES, input_trytes[i]);
        if(NULL == e){ return 1;}        
        index = (int)(e - TRYTE_VALUES);
        char firstValue = TRYTE_VALUES[index];
        
        e = strchr(TRYTE_VALUES, input_trytes[i+1]);
        if(NULL == e){ return 1;}        
        index = (int)(e - TRYTE_VALUES);
        char secondValue = TRYTE_VALUES[index];

        uint8_t decimalValue = firstValue + secondValue * 27;
        output_bytes[i/2] = decimalValue;
    }

    return 0;
}

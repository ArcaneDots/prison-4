/*
    Copyright (c) 2011 Ian Hollander <ian.hollander@gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef EAN13_DEFINES_H
#define EAN13_DEFINES_H

namespace ean13 
{
  static const int MIN_8 = 7; // EAN-8 without check digit 
  static const int MAX_8 = 8; // EAN-8 with check digit 
  static const int MIN_13 = 12; // EAN-13 without check digit 
  static const int MAX_13 = 13; // EAN-13 with check digit 
  static const int MIN_2 = 14; // EAN-2 without check digit 
  static const int MAX_2 = 15; // EAN-2 with check digit 
  static const int MIN_5 = 17; // EAN-5 without check digit   
  static const int MAX_5 = 18; // EAN-5 with check digit 
  static const int MAX_COMPRESS_LEN = 13; // EAN-8 (w/check digit) + EAN-5
  static const int MAX_LEN = MAX_5; // EAN-5 with check digit

 // http://www.barcodeisland.com/ean13.phtml
 // example
 // Digit 	L-code 		G-code		R-code
 // 0 		0001101 	0100111		1110010
 // 1 		0011001 	0110011		1100110
 // 2 		0010011 	0011011		1101100
 // 3 		0111101 	0100001		1000010
 // 4 		0100011 	0011101		1011100
 // 5 		0110001 	0111001		1001110
 // 6 		0101111 	0000101		1010000
 // 7 		0111011 	0010001		1000100
 // 8 		0110111 	0001001		1001000
 // 9 		0001011 	0010111		1110100
  
  // L-Code: Left hand "Odd" parity "O"
  const char LOCode [][8] = { 
    "0001101",
    "0011001",
    "0010011", 
    "0111101", 
    "0100011", 
    "0110001", 
    "0101111", 
    "0111011", 
    "0110111", 
    "0001011"
  };

  // G-code: Left hand "Even" parity "E"
  const char LECode [][8] = {
    "0100111", 
    "0110011", 
    "0011011", 
    "0100001", 
    "0011101", 
    "0111001", 
    "0000101", 
    "0010001", 
    "0001001", 
    "0010111"
  };

  // R-code: Right hand 
  const char RCode[][8] = {
    "1110010", 
    "1100110", 
    "1101100", 
    "1000010", 
    "1011100", 
    "1001110", 
    "1010000", 
    "1000100", 
    "1001000", 
    "1110100"
  };
 
 
  /** use value of first digit to encode the next 5 digits using their checksum value 
   * 
   * i.e. 4 34234
   *        010011
   */
  const char Parity13[][7] = { 
     "OOOOOO", 
     "OOEOEE", 
     "O0EE0E", 
     "OOEEEO", 
     "OEOOEE", 
     "OEEOOE", 
     "OEEEOO", 
     "OEOEOE", 
     "OEOEEO", 
     "OEEOEO"  
  };

  /**
   * encode last 2 digits using combined literal value mod 4 i.e. 3,4 = 34 % 4 
   */
  const char Parity2[][3] = { 
     "OO", 
     "OE",
     "EO",
     "EE"  
  };
  /**
   * encode last 5 digits using thier checksum value
   */ 
  const char Parity5[][6] = { 
     "EEOOO", 
     "EOEOO", 
     "EOOEO", 
     "EOOOE", 
     "OEEOO", 
     "OOEEO", 
     "OOOEE", 
     "OEOEO", 
     "OEOOE", 
     "OOEOE"  
  };

  static const QString guardBar("101");
  static const QString separator("01010");
  static const QString quietZone("000000000");
  static const QString lhGuardBar("1011");
  static const QString cSeparator("01");
};
#endif // EAN13_DEFINES_H
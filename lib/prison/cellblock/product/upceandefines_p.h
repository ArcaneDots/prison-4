/*
    Copyright (c) 2011 Ian Hollander <ian.hollander at gmail.com>

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

#ifndef UPC_EAN_DEFINES_H
#define UPC_EAN_DEFINES_H

#include "../shareddefines.h"
/**
 * values shared by all procduct codes; EAN-13, UPC-A, UPC-E, EAN-8
 * 
 * http://www.barcodeisland.com/ean13.phtml
 * Digit 	L-code 		G-code		R-code
 * 0 		0001101 	0100111		1110010
 * 1 		0011001 	0110011		1100110
 * 2 		0010011 	0011011		1101100
 * 3 		0111101 	0100001		1000010
 * 4 		0100011 	0011101		1011100
 * 5 		0110001 	0111001		1001110
 * 6 		0101111 	0000101		1010000
 * 7 		0111011 	0010001		1000100
 * 8 		0110111 	0001001		1001000
 * 9 		0001011 	0010111		1110100
 */
namespace upc_common
{
  /**
   * 10 symbols (0-9)
   */
  const int SYMBOL_TABLE_SIZE = 10;
  /**
   * Check sum modulus = 10
   */ 
  const int CHECKSUM_MODULUS_VALUE = 10;
  /**
   * One check digit
   */
  const int NUM_CHECK_DIGITS = 1;
  /**
   * One and only check digit which is always required
   */ 
  const int NUM_REQUIRED_CHECK_DIGITS = 1;
  /**
   * Symbol encoded length are all 8 bits long
   */
  const int ENCODE_LENGTH = 8; // 8 bit
  /**
   * number of procduct code digits included in an encoded block
   */
  const int ENCODE_BLOCK_SIZE = 6;
  
  //required bar pattern before the main barcode
  static const QString LEFT_HAND_GUARD_BARS("101");  
  //required bar pattern after the main barcode
  static const QString RIGHT_HAND_GUARD_BARS("101");
  //required bar pattern seperates the main barcode into two sections
  static const QString CENTER_GUARD_BARS("01010");
  //required space before and after the barcode
  static const QString QUIET_ZONE("00000000"); 
  //bar pattern before the extended barcode
  static const QString ADD_ON_GUARD_BARS("1011");
  //bar pattern seperates each encoded digit in the extended barcode
  static const QString ADD_ON_SEPERATOR("01");
  
 /**
   * symbols == {0,1,2,3,4,5,6,7,8,9}
   */
  const char SYMBOL_TABLE [SYMBOL_TABLE_SIZE][2] = {
	  "0", 
	  "1", 
	  "2", 
	  "3", 
	  "4", 
	  "5", 
	  "6", 
	  "7", 
	  "8", 
	  "9"
  };
  /**
   * Most product codes number system value is also used for formatting and encoding
   *  
   * @note EAN-8's number system value has no effect on either
   * @note EAN-2 and EAN-5 are handled by special functions in UPC-A  
   */
  const bool HAS_NUMBER_SYSTEM_VALUE = true;  
  // index of number system digit(s)
  const int  NUMBER_SYSTEM_INDEX = 0; 
  /**
   * Enumerate constants for all known product number system values
   */
  enum NUMBER_SYSTEM_VALUES {
    NS__REGULAR_UPC_CODES_1 		= 0,
    NS__RESERVED_1			= 1,
    NS__IN_STORE_WEIGHTED 		= 2,
    NS__NATIONAL_DRUG__HEALTH_RELATED 	= 3,
    NS__IN_STORE_NONFOOD 		= 4,
    NS__COUPONS 			= 5,
    NS__RESERVED_2			= 6,
    NS__REGULAR_UPC_CODES_2 		= 7,
    NS__RESERVED_3 			= 8,
    NS__RESERVED_4 			= 9
  }; 
  /**
   * Enumerated constants for all known product code types
   */
  enum PRODUCT_CODE_VALUES {
    PS__UNKNOWN = 0,
    /**
     * EAN-13
     */
    PS__EAN_13,
    /**
     * EAN-8
     */
    PS__EAN_8,
    /**
     * EAN-5
     */
    PS__EAN_5,
    /**
     * EAN-2
     */
    PS__EAN_2,
    /**
     * UPC-A
     */
    PS__UPC_A,
    /**
     * Upc-A w/o check digit
     * 
     * Used by the FDA(US) for drug and related health item codes.
     * replaced by EAN-13 number sytems value #3
     * 
     * @note not defined
     */
    PS__UPC_B, 
    /**
     * Upc-A w/check digit
     *
     * @note not defined
     */
    PS__UPC_C, 
    /**
     * @note not defined
     */
    PS__UPC_D,
    /**
     * Zero compressed UPC-A 
     * 
     * UPC-A number must contain at least four(4) contiguous zero(0)s
     * ex 112300004564 -> 11245634 : case #2
     */
    PS__UPC_E,
    /**
     * Additional five(5) digits commonly used to include a book price
     * 
     * UPC-5 has been replaced by the better defined EAN-5 value found 
     * in the current EAN standard. 
     * 
     * @note not defined
     */ // not defined
    PS__UPC_5
  };
  
  /**
   * L-Code: Left hand "Odd" parity symbol encoding
   *
   * @note 'O' in encoding patterns
   */
  const char LEFT_ODD_ENCODE_TABLE [][ENCODE_LENGTH] = { 
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

  /**
   * G-code: Left hand "Even" parity symbol encoding
   *
   * @note 'E' in encoding patterns
   */
  const char LEFT_EVEN_ENCODE_TABLE [][ENCODE_LENGTH] = {
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

  /**
   * R-code: Right hand parity symbol encoding
   *
   * @note 'R' in encoding patterns
   */
  const char RIGHT_HAND_ENCODE_TABLE[][ENCODE_LENGTH] = {
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
};

/**
 * EAN-2: two charactor extended barcode
 *
 * Often used to denote the issue number of a periodical; 
 * i.e. amagazine or comic book
 */
namespace ean2
{
  /**
   * default value
   */
  static const QString DEFAULT_VALUE("0 0");
  /** 
   * two charactor long
   */
  const int BLOCK_SIZE = 2; 
  /** 
   * Ean-2 uses 4 instead of the normal 10 check sum modulus value
   */
  const int CHECKSUM_MODULUS_VALUE = 4;
  /**
   * Number system digit do not appear in an EAN-2 number
   */
  const bool HAS_NUMBER_SYSTEM_VALUE = false;
  /**
   * use the combined literal value mod 4 to encode last 2 digits  
   * 
   * i.e. 3,4 = 34 % 4 = 2 -> OE
   */
  const char PARITY_2[][3] = { 
     "OO", 
     "OE",
     "EO",
     "EE"  
  };
};
/**
 * EAN-5: five charactor extended barcode
 *
 * Often used to denote the price related information  
 * when combined with a ISBN number
 */
namespace ean5
{ 
  /**
   * default value
   */
  static const QString DEFAULT_VALUE("9 9 9 9 9");
  /** 
   * five charactor long
   */
  const int BLOCK_SIZE = 5;
  /**
   * Number system digit do not appear in an EAN-5 number
   */
  const bool HAS_NUMBER_SYSTEM_VALUE = false;
  /**
   * use the checksum value to encode last 5 digits
   * 
   * 23456
   * 2 * 3 + 3 * 9 + 4 * 3 + 5 * 9 + 6 * 3 = 6 + 27 + 12 + 45 + 18
   * = 10(8) -> OEOOE
   */ 
  const char PARITY_5[][6] = { 
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
};
/**
 * UPC-E is a (zero) compressed version of a UPC-A
 *
 * UPC-A must contain four or more contiguous zeros and 
 * must have number system = (0 or 1) 
 */
namespace upcE
{
  /**
   * default value -> 1 23456-00007 8
   */
  static const QString DEFAULT_VALUE("1 2 3 4 5 6 7 8"); //0
  /**
   * UPC-E without check digit
   */
  const int MIN = 7;    
  /**
   * UPC-E with check digit
   */
  const int MAX = 8;   
  /**
   * EAN-2 without check digit 
   */
  const int MIN_2 = MIN + 2;  
  /**
   * EAN-2 with check digi
   */
  const int MAX_2 = MAX + 2;    
  /**
   * EAN-5 without check digit 
   */
  const int MIN_5 = MIN + 5;      
  /**
   * EAN-5 with check digit
   */
  const int MAX_5 = MAX + 5;   
  /**
   * EAN-5 with check digit 
   */
  const int MAX_LEN = MAX_5;  
  /**
   * Offset of the check digit
   */
  const int CHECK_DIGIT_OFFSET = 7;
  /** 
   * six charactors long
   */
  const int BLOCK_SIZE = 6;
  /**
   * Index of value indicating which compression method was used
   */ 
  const int COMPRESS_METHOD_INDEX = 6; 
  /**
   * Encoding pattern seperates the main barcode into two sections ?
   */
  static const QString SPECIAL_GUARD_BARS("010101");
  /**
   * Encoding pattern when the product number system value == '0'
   * 
   * The check sum value determines the actual pattern used
   */
  const char numberSystem0 [][BLOCK_SIZE+1] = {
	"EEEOOO", // 0
	"EEOEOO", // 1
	"EEOOEO", // 2
	"EEOOOE", // 3 
	"EOEEOO", // 4
	"EOOEEO", // 5
	"EOOOEE", // 6
	"EOEOEO", // 7
	"EOEOOE", // 8
	"EOOEOE"  // 9
  };
  /**
   * Encoding pattern when the product number system value == '1'
   * 
   * The check sum value determines the actual pattern used
   */
  const char numberSystem1 [][BLOCK_SIZE+1] = {
	"OOOEEE", // 0
	"OOEOEE", // 1
	"OOEEOE", // 2
	"OOEEEO", // 3
	"OEOOEE", // 4
	"OEEOOE", // 5
	"OEEEOO", // 6
	"OEOEOE", // 7
	"OEOEEO", // 8
	"OEEOEO"  // 9
  };
}

namespace upcA
{
  /**
   * default value
   */
  static const QString DEFAULT_VALUE("0 1 2 3 4 5 6 7 8 9 0 1");
  /**
   * UPC-A without check digit
   */
  const int MIN = 11;  
  /**
   * UPC-A with check digit
   */
  const int MAX = 12;  
  /**
   * EAN-2 without check digit
   */
  const int MIN_2 = 13;  
  /**
   * EAN-2 with check digit 
   */
  const int MAX_2 = 14; 
  /**
   * EAN-5 without check digit
   */
  const int MIN_5 = 16;   
  /**
   * EAN-5 with check digit 
   */ 
  const int MAX_5 = 17; 
  /**
   * Longest possible UPC-A (UPC-A + EAN-5 with check digit)
   */
  const int MAX_LEN = MAX_5; 
  /**
   * Offset of the check digit
   */
  const int CHECK_DIGIT_OFFSET = 11;
  /**
   * human-readable block size
   */
  const int BLOCK_SIZE = 5;
}

/**
 * EAN-8 is a product code that is identical to but not compatible with EAN-13/UPCs
 *
 * EAN-8 was designed to be a compressed product code using a simiar structure and 
 * layout as other EAN/UPC product codes. The main incompatiblity is that EAN-8 
 * uses a different databases of manufacture and product codes. This makes the 
 * convert to and from other product codes like UPCs and EAN-13 impossible without
 * having access both databases. 
 */
namespace ean8
{
  /**
   * default value
   */
  static const QString DEFAULT_VALUE("1 2 3 4 5 6 7 8");  
  /**
   * EAN-8 without check digit
   */
  const int MIN_8 = 7; //  
  /**
   * EAN-8 with check digit 
   */
  const int MAX_8 = 8; // 
  /**
   * EAN-2 without check digit 
   */
  const int MIN_2 = 9; // 
  /**
   * EAN-2 with check digit 
   */
  const int MAX_2 = 10; // 
  /**
   * EAN-5 without check digit 
   */
  const int MIN_5 = 12; //  
  /**
   * EAN-5 with check digit 
   */ 
  const int MAX_5 = 13; // 
  /**
   * Longest possible Ean-8 (EAN-5 with check digit)
   */
  const int MAX_LEN = MAX_5; 
  /**
   * Offset of the check digit
   */
  const int CHECK_DIGIT_OFFSET = 7;
  /** 
   * two sets of four characters blocks
   */
  const int BLOCK_SIZE = 4;
  /**
   * number of product code digits included in an encoded block
   */
  const int ENCODE_BLOCK_SIZE = 4;
  /**
   * Number system digit do not appear in an EAN-2 number
   */
  const bool HAS_NUMBER_SYSTEM_VALUE = false;
}

/**
 * EAN-13 is an expanded UPC-A code that can denote product type.
 *
 * EAN-13 uses a prepended digit to the existing UPC-A standard. 
 * This additional digit allows specification of country specific codes or special 
 * internationally agreed on types. The better known types include ISBN (book),
 * ISSN (magazines) and even different types of coupons.     
 */ 
namespace ean13 
{  
  /**
   * default value
   */
  static const QString DEFAULT_VALUE("1 2 3 4 5 6 7 8 9 0 1 2 8");
  /**
   * EAN-13 without check digit 
   */
  const int MIN = 12; // 
  /**
   * EAN-13 with check digit
   */
  const int MAX = 13; //  
  /**
   * EAN-2 without check digit 
   */
  const int MIN_2 = 14; // 
  /**
   * EAN-2 with check digit 
   */
  const int MAX_2 = 15; // 
  /**
   * EAN-5 without check digit 
   */
  const int MIN_5 = 17; //   
  /**
   * EAN-5 with check digit 
   */
  const int MAX_5 = 18; // 
  /**
   * Longest possible Ean-13 (EAN-5 with check digit)
   */
  const int MAX_LEN = MAX_5; 
  /**
   * Offset of the check digit
   */
  const int CHECK_DIGIT_OFFSET = 12;
  /** 
   * two sets of six characters blocks
   */
  const int BLOCK_SIZE = 6;   
  /** 
   * "parity" pattern used to encode first block (digits 1-7)
   * 
   * use value of first digit to determine which pattern 
   * i.e. 4 -> OEOOEE 
   */
  const char PARITY_13[][7] = { 
     "OOOOOO", // 0
     "OOEOEE", // 1
     "OOEEOE", // 2
     "OOEEEO", // 3
     "OEOOEE", // 4
     "OEEOOE", // 5
     "OEEEOO", // 6
     "OEOEOE", // 7
     "OEOEEO", // 8
     "OEEOEO"  // 9
  };
}
#endif // UPC_EAN_DEFINES_H

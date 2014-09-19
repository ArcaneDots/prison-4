/*
    Copyright (C) 2011  Ian gmail <ianhollander at gmail.com>

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


#ifndef ABSTRACTBARCODEENGINE_H
#define ABSTRACTBARCODEENGINE_H

#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>
#include <deque>
#include <set>

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QImage>
#include <QColor>

#include "shareddefines.h"
#include "indexedregexp.h"
#include "symbol.h"

/**
 * @section barcode_engine Barcode Engine
 * 
 * All index values arrays associate with a symbol in the order they are encoded
 * 
 * Most symbols can be directly encoded. Some symbols are mapped to a set of encodable 
 * symbols. This allows for larger number of user symbols a smaller set of actual encodings 
 */

class CodeEngine
{ 
public:
   /**
    * Error flags
    * 
    * call getStatusFlags to get current flag(s). Function should return "OK" otherwise 
    *  an error has been encountered.
    * @see getStatusFlags 
    */
   enum ErrorCode {
      /**
       * "Ok", no error found
       */
       OK = 0x0,    
       /** 
	* User value is empty 
	*/
       MissingInput = 0x1,
       /**
	* User value does not fit one or more acceptable length
	*/
       LengthOutOfRange = 0x2,
       /**
	* User value contains unrecognized symbol(s) 
	*/
       UnknownSymbols = 0x4,
       /**
	* Symbols are either out-of-range/not allow for their current positions
	*/ 
       InvalidSymbols = 16,
       /**
	* All of the required check digits were not found
	*/
       MissingRequiredCheckDigits = 32,
       /**
	* At lesat one check digit was invalid
	*/
       InvalidCheckDigits = 64,  
       /**
	* Unspecified violation of standard governing the barcode's contruct
	*/
       StandardsViolationError = 128, 
       UnknownError = -1
   };     
   Q_DECLARE_FLAGS(ErrorCodes, ErrorCode);
   
   /**
   * barcode validation/processing flags
   * 
   * @note not implimented
   */
  enum ConstructCode {
      /**
       * Barcode information must strictly adhere to the requirements.
       * 
       * All required data must be present and correct. 
       */
      Strict = 0,
//       /**
//        * Barcode information must be presented in a way that any errors can be detected and fixed automaticly 
//        * 
//        * data outside of specification limits will be considered invalid; length, unknown symbols.
//        * add missing check digits, fix or replace check digit in fixed locations (product codes),
//        */
//       Auto = 1,
      /**
       * don't return human-readable version of the barcode 
       * 
       * @note m_seperatedCharactors will remain empy 
       * 0x000000
       */
       NoText = 2,
       /**
	* Recalculate existing required check digit(s)
	* 
	* @note used by product codes
	*/
       UpdateCheckDigit = 4,
       /** 
	* Add required check digits
	*
	* Will add any required check digits as needed.
	*
	* @note adds missing internal(product code) or 
	* @note appends required check digits as needed
	*/
       AddCheckDigits = 8,
       /**
	* Add all optional check digits 
	* 
	* 0x000010  
	*/
       UseAllCheckDigits = 16,  
       /**
	* Allow user input to be "compressed" if possible
	*
	* @note used by product codes; UPC-E -> UPC-A -> EAN-13 
	*/
       ScaledValue = 32,    
       /**
	* 
	*/
       AutoLinear = AddCheckDigits,
       /**
	* 
	*/
       AutoProduct = AddCheckDigits | UpdateCheckDigit | ScaledValue
       // ... some more options with value which is a power of two
   };     
   Q_DECLARE_FLAGS(ConstructCodes, ConstructCode);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CodeEngine::ErrorCodes);
Q_DECLARE_OPERATORS_FOR_FLAGS(CodeEngine::ConstructCodes);

namespace barcodeEngine
{
  
  
  class AbstractBarcodeEnginePrivate;
  

  /**
   * Base for all barcodeEngine class
   * 
   * Contains shared functionality used to contruct linear barcodes
   */
  class AbstractBarcodeEngine
  {
  public:     
    /**
     * Default constructor
     */
    AbstractBarcodeEngine();
    /**
     * destructor
     */
    virtual ~AbstractBarcodeEngine(); 
    // -- inline get/set members --
    /** 
     * Default value
     */
    virtual const QStringList codeDefault() const = 0;
    /**
     * Get user's input string
     */
    virtual QString userInput() const = 0;
    /**
     * initial list of parsed symbols
     */
    virtual QString parsedSymbolList() const = 0;
    /**
     * final list of symbols
     */
    virtual QString finalSymbolList() const = 0;  
    /**
     * Get QImage of barcode data
     * 
     * @note virtual
     *
     * @param requestedSize size of wigdet viewable area
     * @param minimumSize size reqired to correctly display the barcode information
     * @param foregroundColor text and bar color
     * @param background color of "white" space
     * @return QImage
     */
    virtual QImage image(const QSizeF &requestedSize, QSizeF &minimumSize, 
		    const QColor &foregroundColor, const QColor &backgroundColor) = 0;
  protected:
    /**
     * Calculate check sum digit for a array of symbol look-up indexes
     * 
     * Uses the most common method of calculating a check digit; 
     * = Sum(index values) % modulus value
     * @note default version returns 0; 
     * @note please override with correct checksum function
     * 
     * @param symbolArray array of symbol's look-up index
     * @return number of valid check digit
     */
    virtual int calculateCheckDigit() const = 0;  
  private:
    /**
     * Not used but included for ABI
     */
    //class AbstractBarcodeEnginePrivate;
    AbstractBarcodeEnginePrivate *d;     
  };

  //class LinearMultiple;
  // helper functions

  /**
   * Calculate "Odd/Even" check sum pattern for a array of symbol look-up indexes
   * 
   * Move left -> right, the left-most index treated as "odd".
   * Uses the  method of calculating a check sum; 
   * @sa = NextMultipleCheckDigit(
   * @sa sum of all odd indexes * oddMultipler + sum of all even indexes * evenMultipler)
   * 
   * @param symbolArray array of symbol's look-up index
   * @param oddMultipler value multipled with indexes in the "Odd" position
   * @param evenMultipler value multiplied with indexes in the "Odd" position
   * @param reverse move right -> left, the right-most index treated as "odd" 
   * @return value of valid check digit
   */
  int CommonChecksumOddEven(int checksumModulus,
			  const barcodeEngine::SymbolList& symbols, 
			  int oddMultipler,
			  int evenMultipler,
			  bool reverse = false);
  /**
   * Calculate "Linear" check sum digit for a array of symbol look-up indexes
   * 
   * Uses the  method of calculating a check digit; 
   * = modulus - Sum(index * (counter < modulus)) % modulus value
   * 
   * @param symbolArray array of symbol's look-up index
   * @param reverse
   * @return number of valid check digit
   */
  int CommonChecksumLinear(int checksumModulus,
			  const barcodeEngine::SymbolList& symbols,
			  bool reverse = false);
  /**
   * Value + checksum == multiple of the "Modulus" value
   * 
   * @param checksum total of all the individual symbol's index and positional value 
   * @return modulus value - (checksum & modulus value)
   */
  int NextMultipleCheckDigit(int modulusValue, int checksum);
};


			       
#endif // ABSTRACTBARCODEENGINE_H


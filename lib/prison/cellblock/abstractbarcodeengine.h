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
#include "shareddefines.h"
#include "indexedregexp.h"

/**
 * @section barcode_engine Barcode Engine
 * 
 * All index values arrays associate with a symbol in the order they are encoded
 * 
 * Most symbols can be directly encoded. Some symbols are mapped to a set of encodable 
 * symbols. This allows for larger number of user symbols a smaller set of actual encodings 
 */


namespace CodeEngine
{ 
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
       UnknownSymbols = 8,
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

namespace shared
{
/**
 * Base for all barcodeEngine class
 * 
 * Contains shared functionality used to contruct linear barcodes
 */
class AbstractBarcodeEngine
{
public:     
  /**
   * constructor
   *
   * @note Add multi-charactor symbols by append their regexp pattern(s) to the 
   * @note m_regexpCollection object i.e code128
   *
   * @param defaultString default input used when user input is invalid
   * @param minLength minimum length of user input 
   * @param maxLength maximum length of user input
   * @param multiCharSymbols can symbol be longer than one charactor
   * @param checksumModulus integer value use to calculate the check sum
   * @param internalCheckDigit does a check digit appear inside the barcode string
   * @param requiredCheckDigits number of required check digits/symbols
   * @param maxCheckDigits maximum number of check digits/symbols allowed
   **/
  AbstractBarcodeEngine(const QString &userBarcode, 
			CodeEngine::ConstructCodes flags,
			const QString &defaultString,
			int minLength, 
			int maxLength,
			bool multiCharSymbols,
			int checksumModulus,
			bool internalCheckDigit,
			int requiredCheckDigits,
			int maxCheckDigits);
  /**
   * destructor
   */
  virtual ~AbstractBarcodeEngine(); 
protected:
  /**
   * Class specicfic initialization
   */
  void initialize();
  /**
   * Set current barcode string
   * 
   * @param userBarcode user string containing barcode symbols 
   * @param flag construction hints; Defaults to "Auto".
   * 
   * @sa Calls related functions;
   * @sa fillSymbolList() to initialize a set of symbols
   * @sa fillWidthEncodingList() to initialize a set encoding for each symbols 
   * @sa parseSymbolString() to parse symbols from input string
   * @sa processSymbolList() process and validate the list of symbols found
   * @sa formatSymbols() to build human-readable output
   * @sa encodeSymbols() to build machine readable output "barcode"
   **/
  virtual void setBarcodeString();
public:
  // -- inline get/set members --
  /** 
   * Default value
   */
  virtual QString codeDefault() const;
  /**
   * User input string
   */
  virtual QString userInput() const;
  /**
   * list of parsed symbols
   */
  virtual QStringList parsedSymbolList();
  /**
   * final list of symbols
   */
  virtual QStringList finalSymbolList();
  /**
   * Get a string of symbols encoded according to the barcode's specification
   *  
   * @param barLocations [in|out](preferably empty)set of barcode positions
   * @param barThicknessMultiple
   * 
   * @return symbols separated into blocks defined by the barcode's encoding
   */ 
  QString encodedSymbols(shared::BarPositionsMap &barLocations, 
				       int barThicknessMultiple = 1);
  /**
   * Get a list of symbol blocks formatted according to the barcode's specification
   */
  inline QStringList formatedSymbols() const
  {
    return m_formatedSymbols;
  }
  /**
   * Is the user input valid for the current barcode
   * 
   * The barcode will display a default value in case the user's is not  
   */
  inline CodeEngine::ErrorCodes statusFlags() const
  {
    return m_isValid;
  }   
    
protected:            
  /**
   * Parse user input string into list of valid symbols (strings)
   *    
   * @note never affects the flags in m_isValid member
   * @note Uses regexp pattern(s) stored in m_regexpCollection object to the
   * @note to parse multi-character symbols i.e code128
   * 
   * @param symbolString user input string
   * @returns similarly ordered list of single and multi-character symbols
   */
  virtual QStringList parseSymbolString(const QString &symbolString) const;  
  /**
   * Performs in-depth input validation
   * 
   * Determine whether the user appended the minimum number of check digit(s). 
   * Override if the input requires additional or different validation.
   *
   * @note base class version assumes that current barcode should have a 
   * @note minimum number of appended check symbols defined by constructor  
   * @note parameters;
   * @note requiredCheckDigits and maxCheckDigits
   * 
   * @returns number of check digits found 
   */
  virtual QStringList processSymbolList(const QStringList &userSymbols);
  /**
   * Calculate check sum digit for a array of symbol look-up indexes
   * 
   * Uses the most common method of calculating a check digit; 
   * = Sum(index values) % modulus value
   * @note used by Code 39, MSI (2 Mod 10)
   * 
   * @param symbolArray array of symbol's look-up index
   * @return number of valid check digit
   */
  virtual int calculateCheckDigit(const shared::LookupIndexArray& symbolArray) const;       
  /**
   * Get the value when added to the check sum makes it a multiple of the modulus
   * 
   * @param checksum total of all the individual symbol's index and positional value 
   * @return = modulus value - (checksum & modulus value)
   */
  int NextMultipleCheckDigit(int checksum) const;
  /**
   * Get the remainder of the check sum divided by the modulus value
   * 
   * @param checksum total of all the individual symbol's index and positional value
   * @return = (checksum % modulus value
   */
  int SimpleRemainderCheckDigit(int checksum) const;
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
  int CommonChecksumOddEven(const shared::LookupIndexArray& symbolArray, 
			   int oddMultipler,
			   int evenMultipler,
			   bool reverse = false) const;
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
  int CommonChecksumLinear(const shared::LookupIndexArray& symbolArray,
			   bool reverse = false) const;
  /**
   * Convert list of symbols to an array of their indexes  
   * 
   * @note assumes symbol <-1 to 1-> index value, 
   * @note subclass to handle symbol sets that have special expanded encodings 
   * 
   * @param symbolList ordered list of symbols
   * @returns a matching of list of symbol indexes
   */
   virtual shared::LookupIndexArray convertSymbolsToIndexes(
     const QStringList &symbolList) const;
  /**
   * Convert symbol's width encoding to the equivalent binary encoding
   * 
   * @note assumes linear encodings i.e. space = 0, bar = 1,
   * @note subclass to handle encoding with more than two states
   * 
   * @param widthEncodedSymbol simplified version of a symbol encoding
   * @return actual symbol encoding
   */
   const QString convertWidthEncodingToBinary(
     const QString &widthEncodedSymbol) const;        
  /**
   * Separate set of symbols into logical blocks based on encoded layout
   *
   * @note assume no special formatting 
   * @sa see upcA::formatSymbols() for guidance on complex formatting 
   * 
   * @param symbolSrc list of all valid symbols
   * @param splitIndex index of second list
   */
  virtual void formatSymbols(const QStringList &symbolSrc);     
  /**
   * Encode set of symbols according to current barcode type
   *  
   * @note assume no special formatting 
   * @sa see upcA::encodeSymbolsSymbols() for guidance on complex encoding 
   * 
   * @param symbolSrc list of all valid symbols
   * @param splitIndex index of second list
   */ 
  virtual void encodeSymbols(const QStringList &symbolSrc);
  
  // -- symbol look-ups --
  
  /**
   * Get a symbol's look-up table index
   * 
   * @param symbol 1 or more character long "symbol"
   * @returns index of symbol or NOT_FOUND in case symbol does not exist
   */
  int lookupSymbolIndex(const QString &symbol) const;  
  /**
   * Get the symbol at a particular look-up table index
   * @return symbol string
   */
  const QString lookupSymbolAtIndex(int symbolIndex) const;
  /**
   * Encode an individual symbol 
   * 
   * @note overload in case the symbol encoding deviates from most linear barcodes 
   * 
   * @param symbol symbol to be encoded
   * @return actual encoding
   */
  virtual QString lookupSymbolEncoding(const QString &symbol) const;
  /**
   * Add a regular expression to match a multi-character symbol(s)
   *
   * @note 
   */
  void AddMultiSymbolRegexpString(const QString &symbolPattern);
  /**
   * Convert a symbol or symbol index to the binary barcode encoding
   * 
   * @tparam V symbol
   * @tparam T string-like barcode information 
   */
  template <class V, class T> class ConvertSymbolToEncoding :
       public std::unary_function<V, T> 
    {
    public:
      ConvertSymbolToEncoding (AbstractBarcodeEngine * barcodeEngine) :
      m_barcodeEngine(barcodeEngine)
      {
	Q_ASSERT(barcodeEngine != 0);
      }
      /**
       * Get encoded version of symbol
       * 
       * @param symbol index
       * @returns string-like symbol encoding information
       */
      T operator() (int symbol) {
	Q_ASSERT(symbol != NOT_FOUND);
	return  m_barcodeEngine->lookupSymbolEncoding(
	  m_barcodeEngine->lookupSymbolAtIndex(symbol));
      }	
      /**
       * Get encoded version of symbol
       * 
       * @param symbol text
       * @returns string-like symbol encoding information
       */
      T operator() (V symbol) {
	Q_ASSERT(m_barcodeEngine->lookupSymbolIndex(symbol) != NOT_FOUND);
	return  m_barcodeEngine->lookupSymbolEncoding(symbol);
      }	
    private:
      AbstractBarcodeEngine * m_barcodeEngine;
    };  
  /**
   * Change the symbol and symbol encoding look-up tables 
   * 
   * @sa lookupSymbolIndex
   * @sa lookupSymbolAtIndex
   * @sa lookupSymbolEncoding
   */
  void fillSymbolAndEncodingLookupTable(const QStringList &symbolSet, 
			       const QStringList &encodingSet);    
  /**
   * Set the active symbol look-up table 
   * 
   * @sa getSymbolIndex, getSymbolAtIndex
   */
  void fillSymbolLookupTable(const QStringList &symbolSet);    
  /**
   * Get the active symbol look-up table 
   * 
   * @sa setSymbolLookupTable
   */
  QStringList fillSymbolLookupTable() const;    
  /** 
   * Set the active symbol encoding look-up table 
   * 
   * @sa getSymbolEncoding
   */
  void setSymbolEncodingLookupTable(const QStringList& symbolEncodingSet);
  /** 
   * Get the active symbol encoding look-up table 
   * 
   * @sa setSymbolEncodingLookupTable
   */
  QStringList symbolEncodingLookupTable() const;
  /**
   * Fill m_symbolList list with all symbol ordered by index
   *
   * Default symbol list = {0-9}
   * @note must be defined by subclass
   */
  virtual void fillSymbolList();
  /**
   * Fill m_widthEncodingList list with all symbol's width encoding ordered by index
   *
   * There is no default encoding because each individual/family barcode use 
   * a different symbol encoding.
   * @note must be defined by subclass
   */
  virtual void fillWidthEncodingList() = 0;   
  /** 
   * barcode's minimum number of symbols 
   * @note NOT_FOUND == no minimum
   */ 
  int m_minLength;
  /** 
   * barcode's maximum number of symbols
   * @note NOT_FOUND == no maximum
   */ 
  int m_maxLength;
  /**
   * maximum number of check digits allow by the barcode
   */
  int m_maxCheckDigits;
  /**
   * minimum number of check digits required by the barcode
   * 
   * @note 0 == none required 
   * @note X < m_checkDigits; "X" number required
   */
  int m_requiredCheckDigits;    
  /**
   * default string used if user input is invalid
   */
  QString m_defaultString;    
  /**
   * copy of original user input
   */
  QString m_userInputString;      
  /**
   * symbol list created from parsed user input
   */
  QStringList m_userParsedSymbols;
  /**
   * symbol list based on final validated user input
   */
  QStringList m_finalSymbolList;
  /**
   * modulus value used to calculate the check digit
   */
  int m_checksumModulus;
  /**
   * barcode supports multi-character symbols 
   */
  bool m_multiCharSymbols;
  /**
   * check digit is located inside the barcode
   * 
   * @note "true" for product codes, 
   * @note "false" for most other linear barcodes
   */
  bool m_internalCheckDigit;
  /**
   * text/human readable version of encoded information
   */
  QStringList m_formatedSymbols;
  /** 
   * machine readable/encoded barcode information
   */
  QString m_encodedSymbols;
  /**
   * Actual pixel-based index locations of encoded sections used to layout text
   */
  shared::BarPositionsMap m_barPositions;
  /**
   * is user input string valid
   * 
   * @note current behavior: used default string when user input is invalid
   */
  mutable CodeEngine::ErrorCodes m_isValid;
  /**
   * Input processing flags
   * 
   * @note not currently implemented
   */
  CodeEngine::ConstructCodes m_constructionFlags;
    
private:
  
  class Private;
  Private *d;   

};
};
#endif // ABSTRACTBARCODEENGINE_H


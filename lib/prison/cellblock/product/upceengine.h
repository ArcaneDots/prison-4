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


#ifndef UPCEENGINE_H
#define UPCEENGINE_H

#include "upcaengine.h"

using namespace shared;

namespace product 
{  
  const int NAME_SYSTEM0(upc_common::NS__REGULAR_UPC_CODES_1);
  const int NAME_SYSTEM1(upc_common::NS__RESERVED_1);

class UpcEEngine : public ProductEngine
{
  friend class UpcAEngine;
public:
  /**
   * default constructor
   */
  UpcEEngine();
  /**
   * constructor
   *
   * @param userBarcode 
   * @param flags
   **/
  UpcEEngine(const QString &userBarcode, 
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct); 
  /**
   * constructor
   *
   * @param userBarcode 
   * @param flags
   **/
  UpcEEngine(const QList<Symbol> &userBarcode, 
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);   
  /**
   * @brief Product copy constructor
   * 
   * @param existingUpcA
   */
  UpcEEngine(const UpcAEngine &existingUpcA);
//   /**
//    * @brief Product copy constructor
//    * 
//    * @param existingUpcA
//    */
//   UpcEEngine(const Ean13Engine &existingEan13);
  /**
   * destructor
   */
  virtual ~UpcEEngine();
  /**
   * Assigmnet operator = Upc-E 
   */
  UpcEEngine &operator=(const UpcEEngine &rhs);
  /**
   * Assigmnet operator = Upc-A 
   */
  UpcEEngine &operator=(const UpcAEngine &rhs);  
  /**
   * Assigmnet operator = EAN-13 
   */
  UpcEEngine &operator=(const Ean13Engine &rhs);
  /**
   * Expanded UPC 
   * 
   * UPC-E -> UPC-A
   */
  inline SymbolList expanded() const {
    return expandUpc(m_userParsedSymbols) ;
  }
  /**
   * Encoded barcode sections
   * 
   * [block1][block2(empty)][extendedBlock(optional)]
   */
  const QStringList encoded() const;

  const QStringList formatedSymbols() const;

//   /**
//    * Attempt to get UPC-E version of the inputed product code
//    * 
//    * @note not tested
//    * 
//    * @returns product code or empty list in case conversion is not possible
//    */
//   virtual const QString &toUpcE() const;
 
protected:
  /**
   * UPC-E specicfic initialization
   * 
   * Validate underlining UPC-A value and associated check digit
   * 
   * @sa fillSystemEncodingList
   * @sa populateSections
   */
  void localInitialize();
  /**
   * Populate the values associated with each section of the barcode
   */
  void populateSections();  
  /**
   * Calculate correct checkdigit - UPC-E version
   */
  Symbol CalculateCheckDigit();
  /**
   * Validate product codes and splits off the extended code (EAN-2/5)
   * 
   * Verifies the check digit value and the valid sizes with the range 
   * 
   * @param symbolSrc list containing all symbol
   * @returns valid symbol list
   */
  shared::SymbolList processSymbolList(const shared::SymbolList& inputSymbols);
  /**
   * Seperate digits into logical blocks based on encoded layout
   *
   * @note UPC-E  format  [0][1-6][(7)][8-9|13]
   * 
   * @param mainBlock first portion of the list of symbols 
   */
  QStringList formatMainBlock(const SymbolList& mainBlock) const; 
//   /**
//    * Encode complete number according to current barcode type
//    *
//    * UPC-E  format  [-][1-6][-][8-9|13]
//    * 
//    * @param symbolSrc full list of symbols
//    * @param splitIndex index of the "end" of the first half
//    */
//   void encodeSymbols(const product::SymbolList& symbolSrc);   
  /**
   * Encode complete number according to current barcode type
   * 
   * Overload UPC-A version because UPC-E only has 1 block of digits
   *  
   * @note UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param mainBlock first portion of the list of symbols 
   */ 
  QStringList encodeMainBlock(const shared::SymbolList& mainBlock) const;   
  /**
   * Calculate UPC-E checksum digit
   *
   * Retrive check sum digit from expanded UPC-A product code
   *
   * @param symbolArray array of symbol indexes
   * @return valid check digit
   */
  int calculateCheckDigit() const;
  /**
   * Load all encoding patterns based on combo of system number (0-1) and check digit
   */
  void fillSystemEncodingList();      
  /**
   * Attempt to get UPC-A version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  const UpcAEngine &toUpcA() const;
  /**
   * Attempt to get EAN-13 version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
   const QString toEan13() const;
   /**
   * Expand the UPC-E barcode information into UPC-A format 
   *
   * 	UPC-E 		->  		     UPC-A
   * 1. mmppp[0-2] 	-> 	manf dd[0-2]00, prod code 00ddd
   * 2. mmmpp3 		-> 	manf dd[3-9]00, prod code 000dd
   * 3. mmmmp4 		-> 	manf dddd0, prod code 0000d 	 
   * 4. mmmmm[5-9] 	-> 	manf ddddd, prod code 0000[5-9] 
   * 
   * @param compressedUpc list of symbols, not including check digit
   * @param compressionMethod method used to compress the UPC-A code
   */
   shared::SymbolList expandUpc(const shared::SymbolList& compressedList) const;
private:
  UpcAEngine m_UpcA;
  bool m_fromUpcA;
  //QStringList m_parsedInput;
  //SymbolList m_upcE_finalSymbols;
  /**
   * system number "0" encoding list
   */
  QStringList m_system0EncodingList;       
  /**
   * system number "1" encoding list
   */
  QStringList m_system1EncodingList;    
};
};
#endif // UPCEENGINE_H

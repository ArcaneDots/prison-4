/*
    Copyright (C) 2011  Ian Hollander <ianhollander at gmail.com>

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


#ifndef PRODUCTENGINE_H
#define PRODUCTENGINE_H

#include <QtCore/QtDebug>
#include <QImage>

#include "../abstractbarcodeengine.h"
//#include "../symbol.h"
#include "upceandefines_p.h"


namespace product 
{
/**
 * handles all operation common to all GS1 product bar code
 * 
 * Performs basic symbology and length validation based on values 
 * passed to member variables during child object construction.
 * With the exception of extended codes (EAN-2,EAN-5) all product 
 * code advanced validation and construction is preformed by 
 * during virtualfunctions defines in the child classes.
 *
 * @note The seperate EAN-2 and EAN-5 classes exist only for the sake of  
 * @note completeness and for testing purposes. 
 */
class ProductEngine : public shared::AbstractBarcodeEngine
{    
public:
  /**
   * @brief QString input constructor
   * 
   * Parse user input into a main block, check digit and extended block
   * 
   * @param userBarcode 
   * @param flags construction flags
   * @param productCode constant indicating the current product code
   */
  ProductEngine(const QString &userBarcode, 
	      CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct,
	      upc_common::PRODUCT_CODE_VALUES productCode = upc_common::PS__UNKNOWN);
  
  /**
   * @brief SymbolList input constructor
   * 
   * @param barcodeSymbols ...
   * @param flags construction flags
   * @param productCode constant indicating the current product code
   */
  ProductEngine(const shared::SymbolList &barcodeSymbols, 
		CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct,
		upc_common::PRODUCT_CODE_VALUES productCode = upc_common::PS__UNKNOWN);
  /**
   * destructor
   */
  virtual ~ProductEngine();
  
  // -- inline get/set members --
  
  /** 
   * Default value
   * 
   * @note parsed into a QStringList
   */
  QStringList codeDefault() const;
  /**
   * Get construction flags
   */
  CodeEngine::ConstructCodes constructionFlags() const;
  /**
   * Is the user input valid for the current barcode
   * 
   * The barcode will display a default value in case the user's is not  
   */
  const CodeEngine::ErrorCodes & statusFlags() const;
  /**
   * Add flags
   */
  const CodeEngine::ErrorCodes & addFlags(CodeEngine::ErrorCode flags);
  /**
   * Remove flags
   */
  const CodeEngine::ErrorCodes & removeFlags(CodeEngine::ErrorCode flags);
  /**
   * get Product type assiocated current engine 
   */
  upc_common::PRODUCT_CODE_VALUES productCode() const;
  /**
   * Length of primary section + checkDigit
   */
  int primaryLength() const;  
  /**
   * User input string
   */
  QString userInput() const;
  /**
   * Parsed symbols (user or defaultlt value)
   */
  QStringList parsedSymbolList() const;
  /**
   * Final list of symbols used to generate barcode
   * 
   * @note set by inherited constructor
   */
  QStringList finalSymbolList() const;
  /**
   * Validated full list of underling symbols
   */
  const shared::SymbolList & symbols() const;
  /**
   * Get a list of symbol blocks formatted according to the barcode's specification
   * 
   * [systemdigit][block1][block2][checkdigit][extendedBlock]
   */
  virtual const QStringList formatedSymbols() const = 0;
  /**
   * Main block (not including check digit)
   */
  const QStringList mainBlock() const;
  /**
   * Check digit string
   */
  const QString checkDigit() const; 
  /**
   * Extended block
   */
  const QStringList extendedBlock() const;
  /**
   * Number System 
   * 
   * @note may be blank since EAN-8 dosn't use one
   */
  const QString numberSystem() const;
  /**
   * First block
   */
  const QStringList block1() const;
  /**
   * Second block
   * 
   * @note  may be blank since UPC-E doesn't have one
   */
  const QStringList block2() const;  
  
  const QString systemDigit() const;
  /**
   * Encoded barcode sections
   * 
   * [block1][block2(optinal)][extendedBlock(optional)]
   */
  virtual const QStringList encoded() const = 0;
  
  void swap(ProductEngine &other);  
  
  /**
   * Get QImage of barcode data
   *
   * @param requestedSize size of wigdet viewable area
   * @param minimumSize size reqired to correctly display the barcode information
   * @param foregroundColor text and bar color
   * @param background color of "white" space
   * @return QImage
   **/
  virtual QImage image(const QSizeF &requestedSize, QSizeF &minimumSize, 
		  const QColor &foregroundColor, const QColor &backgroundColor);
  /**
   * Zero - Symbol
   */
  static shared::Symbol ZERO;  
  /**
   * ONE - Symbol
   */
  static shared::Symbol ONE;
protected:  
  /**
   * Initialize -> primary code - check digit - extended code
   * 
   * @sa fillExtendedEncodingList
   */
  void initialize(); 
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
   * get blocksize assiocated current engine 
   */
  int fmtBlockSize() const;
  /**
   * get encoded blocksize of current engine
   */
  int encBlockSize() const;
//   /**
//    * Parse Block
//    * 
//    * called by initialize()
//    */
//    Symbol findCheckDigit(const SymbolList& parsedInput);  
   /**
    * Generate error codes if the supplied and calculated checkdigit are different
    */
   CodeEngine::ErrorCodes validateCheckDigit(const shared::Symbol& foundDigit, 
					     const shared::Symbol& calculated) const;	    
  /**
   * Calculate correct checkdigit - standard "product" version
   * 
   * @note overrides AbstractBarcodeEngine::calculateCheckDigit()
   * 
   * @sa AbstractBarcodeEngine::calculateCheckDigit()
   */
  shared::Symbol CalculateCheckDigit();
  /**
   * Calculate Ean-2 check digit
   * 
   * @param 
   * 
   * @return check digit 
   */
  int calculateEan2CheckDigit(const QList<shared::Symbol >& symbolArray) const;
  /**
   * Calculate Ean-5 check digit
   * 
   * @return check digit 
   */
  int calculateEan5CheckDigit(const QList<shared::Symbol >& symbolArray) const;
  
  const shared::SymbolList local_primaryBlock() const;
  
  const shared::SymbolList local_extendedBlock() const;
  
  /**
    * Number System 
    * 
    * @note may be blank since EAN-8 dosn't use one
    */
  const shared::Symbol local_numberSystem() const;
  /**
    * First block
    */
  const shared::SymbolList local_block1() const;
  /**
    * Second block
    * 
    * @note  may be blank since UPC-E doesn't have one
    */
  const shared::SymbolList local_block2() const;  

  const shared::Symbol setCheckDigit(const shared::Symbol &s);
  const shared::Symbol local_checkDigit() const;  
  /**
   * Format digits in the main block
   * 
   * UPC-A  format: [0][1-5][6-(10)][12-13|15]
   * @param mainBlock list of symbols
   */
  /**
   * Encode complete number according to current barcode type
   * 
   * Overload UPC-A version because UPC-E only has 1 block of digits
   *  
   * @note UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param mainBlock first portion of the list of symbols 
   */ 
  virtual QStringList encodeMainBlock(const shared::SymbolList& mainBlock) const = 0; 
  /**
   * Attempt to encode any remaining symbols as UPC SUPPLEMENTAL digits
   * 
   * Encodes list of 2 or 5 symbols according to the related extended EAN rules.
   * EAN-2/EAN-5
   * 
   * @note tested but only EAN-5 verified
   * 
   * @param extendedBlock list of symbols
   * @returns encoded version of passed symbol list 
   */
  QStringList encodeExtendedBlock(const shared::SymbolList& extendedBlock) const;
  /**
   * Generates "user" for default constructor
   */
  QString generateDefaultInputString(int size) const;
  QList<shared::Symbol> generateDefaultInputSymbols(int size) const;
  /**
   * 
   */
  int drawBars(QPainter * painter, int width, int height, QString strBars) const; 
  /**
   * default string used if user input is invalid
   * 
   * @note product code specific
   */
  //QString defaultString() const;    
  
  static shared::Symbol m_emptySymbol;
  /** 
   * barcode's minimum number of symbols 
   * 
   * @note product code specific,
   * @note NOT_FOUND == no minimum
   */ 
  int minLength() const;
  /** 
   * barcode's maximum number of symbols
   * 
   * @note product code specific,
   * @note NOT_FOUND == no maximum
   */ 
  int maxLength() const;
  /**
   * maximum number of check digits allow by the barcode
   * 
   * @note product code specific
   */
  int maxCheckDigits() const;
  /**
   * Zero-based index of the product code's check digit
   * 
   * @note product code specific,
   * @note "-1" == all check digit appear at end of barcode
   * @note only used for product codes (UPC/EAN-8/EAN-13)
   */
  int checkDigitOffset() const;     
  /** 
   * Digits / block 
   * 
   * @note product code specific
   */
  int digitsPerBlock() const;  
  /**
   * Zero-based offset of the product code's check digit
   * 
   * @note "-1" == all check digit appear at end of barcode
   * @note only used for EAN-8/13
   */
//   //int m_primaryCodeLength; 
//   /**
//    * encoding patterns for EAN-2 
//    */
//   QStringList parity2WidthEncoding() const;
//   /**
//    * encoding patterns for EAN-5
//    */
//   QStringList parity5WidthEncoding() const;    
//   

  // user modifible
  
  /**
   * copy of original user input
   */
  QString m_userInputString;      
  /**
   * symbol list created directly from parsed user input
   */
  shared::SymbolList m_userParsedSymbols;
//   /**
//    * Primary block - primary code minus check digit
//    */
//   SymbolList 	m_primaryBlock;
//   /**
//    * Extended code - all digits in EAN-2 or EAN-5 code
//    */
//   SymbolList 	m_extendedBlock;
//   /**
//    * Check digit
//    */
//   Symbol 	m_checkDigit;  
//   /**
//    * System digit
//    */
//   Symbol	m_systemDigit;
  /**
   * symbol list based on final validated user input
   */
  shared::SymbolList 	m_finalSymbolList;
//   /**
//    * Block 1
//    */
//   SymbolList	m_block1;
//   /**
//    * Block 2 
//    * 
//    * @note will be empty if case of UPC-E/EAN-8
//    */
//   SymbolList	m_block2;
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
  /** 
   * machine readable/encoded barcode information
   */
  QString m_encodedSymbols;
//   /**
//    * 
//    */
//   shared::SymbolPositionsMap m_symbolPositions;
//   /**
//    * Actual pixel-based index locations of encoded sections used to layout text
//    */
//   shared::BarPositionsMap m_barPositions;
  /**
   * Private barcode information
   */
  class Private;
  Private * d;
  
  QStringList m_parity2WidthEncoding;
  QStringList m_parity5WidthEncoding;
private:    
 /**
  * Exteneded barcode encoding patterns - EAN-2, EAN-5 
  */
  void fillExtendedEncodingList();   
};


};
#endif // PRODUCTENGINE_H

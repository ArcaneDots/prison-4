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

#include <QtGui/QImage>
#include <QtCore/QtDebug>

#include "../abstractbarcodeengine.h"
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
 * @note EAN-2 : UPC-A, UPC-E, EAN-13
 * @note EAN-5 : UPC-A, UPC-E, EAN-13
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
    const QStringList codeDefault() const;
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
  QString parsedSymbolList() const;
  /**
   * Final list of symbols used to generate barcode
   * 
   * @note set by inherited constructor
   */
  QString finalSymbolList() const;
  /**
   * Validated full list of underling symbols
   */
  const shared::SymbolList symbols() const;
  /**
   * Get a list of symbol blocks formatted according to the barcode's specification
   * 
   * [systemdigit][block1][block2][checkdigit][extendedBlock]
   * @note set unused blocks to null 
   */
  virtual const QStringList formatedSymbols() const = 0;
  /**
   * Main block (not including check digit)
   * @sa local_mainBlock
   */
  const QStringList mainBlock() const;
  /**
   * Check digit string
   * @sa local_checkDigit
   */
  const QString checkDigit() const; 
  /**
   * Extended block
   * @sa local_checkDigit
   */
  const QStringList extendedBlock() const;
  /**
   * Number System 
   * 
   * @note may be blank since EAN-8 dosn't use one
   * @sa local_numberSystem
   */
  const QString numberSystem() const;
  /**
   * First block
   * 
   * @sa local_block1
   */
  const QStringList block1() const;
  /**
   * Second block
   * 
   * @note  may be blank since UPC-E doesn't have one
   * @sa local_block2
   */
  const QStringList block2() const;
  /**
   * Encoded barcode sections
   * 
   * [block1][block2(optinal)][extendedBlock(optional)]
   * @note set unused blocks to null 
   */
  virtual const QList<QStringList> encoded() const = 0;
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
protected:  
  /**
   * Swap in private data
   */
  void swap(ProductEngine &other);    
  /**
   * Initialize -> primary code - check digit - extended code
   * 
   * @sa fillExtendedEncodingList
   */
  void initialize(); 
  /**
   * Validate basic length and splits off the extended code (EAN-2/5) if any 
   * 
   * Verifies the sizes with the range and check digit value if found
   * Invalid input is replaced with the default value  
   * 
   * @param symbolSrc list containing all symbol
   * @returns valid symbol list
   */
  void processSymbolList(const shared::SymbolList& inputSymbols); 
  /**
   * get blocksize assiocated current engine 
   */
  int fmtBlockSize() const;
  /**
   * get encoded blocksize of current engine
   */
  int encBlockSize() const;
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
  int calculateCheckDigit() const;
  /**
   * Calculate Ean-2 check digit
   * 
   * @param symbolArray
   */
  int calculateEan2CheckDigit(const QList<shared::Symbol>& symbolArray) const;
  /**
   * Calculate Ean-5 check digit
   * 
   * @return check digit 
   */
  int calculateEan5CheckDigit(const QList<shared::Symbol>& symbolArray) const;
  /**
   * Set the raw parsed input from user
   */
  void setRawInput(shared::SymbolList symbolBlock);
  /**
   * Raw parsed input from user
   */
  const QList<shared::Symbol> local_rawInput() const;
  /**
   * Set the primary code block
   */
  void setPrimaryBlock(shared::SymbolList symbolBlock);
  /**
   * Primary code block
   */
  const QList<shared::Symbol> local_primaryBlock() const;
  /**
   * Save checksum value
   */
  void setCheckDigit(const shared::Symbol &s);
  /**
   * Get checksum value
   */
  const shared::Symbol local_checkDigit() const;  
  /**
   * Set the extended code block
   */
  void setExtendedBlock(shared::SymbolList symbolBlock);  
  /**
   * extended code block
   */
  const QList<shared::Symbol> local_extendedBlock() const;  
  /**
   * Main code block (primary code + checksum)
   */
  const QList<shared::Symbol> local_mainBlock() const;
  /**
    * Number System 
    * 
    * @note may be blank since EAN-8 dosn't use one
    */
  const shared::Symbol local_numberSystem() const;
  /**
    * First block
    */
  const QList<shared::Symbol> fmt_block1() const;
  /**
    * Second block
    * 
    * @note  may be blank; UPC-E doesn't have one
    */
  const QList<shared::Symbol> fmt_block2() const;
  /**
   * Encode complete number according to current barcode type
   * 
   * Overload UPC-A version because UPC-E only has 1 block of digits
   *  
   * @note format  [systemDigit][block1][block2][checkDigit][extended]
   * @note each engine must overload
   * 
   * @param mainBlock first portion of the list of symbols 
   */ 
  virtual QList<QStringList> encodeMainBlock(const shared::SymbolList& mainBlock) const = 0; 
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
  QString defaultInputString(int size) const;
  QList<shared::Symbol> defaultInputSymbols(int size) const;
  /**
   * Draw "bars" on image
   */
  int drawBars(QPainter* painter,
	       int top, int bottom, int startOffset,
	       QString strBars) const;
  /**
   * Draw "bars" on image
   */
  int drawBars(QPainter* painter,
	       int top, int bottom, int startOffset,
	       QStringList strLstBars) const;
  /**
   * default symbol used preform parsing 
   * 
   */
  shared::Symbol m_emptySymbol;
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

  // user modifible - I plan on moving these in to Private
  
  /**
   * copy of original user input
   */
  QString m_userInputString;
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
private:
  /**
   * Private barcode information
   */
  class Private;
  Private * d;
 /**
  * Exteneded barcode encoding patterns - EAN-2, EAN-5 
  */
  void fillExtendedEncodingList();   
};


};
#endif // PRODUCTENGINE_H

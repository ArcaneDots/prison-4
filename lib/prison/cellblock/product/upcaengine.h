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


#ifndef UPCAENGINE_H
#define UPCAENGINE_H


#include "productengine.h"

namespace product
{
  class UpcEEngine;
  class Ean13Engine;
/**
 * UPC-A barcode generator
 */
class UpcAEngine : public ProductEngine 
{

public:
  /**
   * @brief default constructor
   */
  UpcAEngine();
  /**
   * String constructor
   *
   * @param productCode constant indicating the current product code
   **/
  UpcAEngine(const QString &userBarcode, 
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct,
	     upc_common::PRODUCT_CODE_VALUES productCode
		= upc_common::PS__UPC_A); 
  /**
   * Pass-thru constructor
   *
   * @param productCode constant indicating the current product code
   **/
  UpcAEngine(const shared::SymbolList &userBarcode, 
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct,
	     upc_common::PRODUCT_CODE_VALUES productCode
	     = upc_common::PS__UPC_A);
  
//   /**
//    * @brief Construct UPC-A from a UPC-E object
//    * 
//    * @param existingUpcE ...
//    */
//   UpcAEngine(const UpcEEngine &existingUpcE);
//   /**
//    * @brief Construct UPC-A from a EAN-13 object
//    * 
//    * @param existingEan13 ...
//    */
//   UpcAEngine(const Ean13Engine &existingEan13);
  /**
   * destructor
   */
  virtual ~UpcAEngine();  
  
  inline shared::SymbolList compressed() const {
    return compressUpc(m_userParsedSymbols);
  }  
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
  /**
   * Encoded barcode sections
   * 
   * [block1][block2(optinal)][extendedBlock(optional)]
   */
  const QStringList encoded() const;
  /**
   * Get a list of symbol blocks formatted according to the barcode's specification
   */
  const QStringList formatedSymbols() const;
  
  void swap(const UpcAEngine &other);
protected:
  /**
   * UpcAEngine Class specicfic initialization
   * 
   * Validate the check digit value and 
   * populate the value of each section
   * 
   * @sa populateSections
   * @sa validateCheckDigit
   */
  void localInitialize();  
  /**
   * Populate the values associated with each section of the barcode
   */
  void populateSections();  
  /**
   * Encode complete number according to current barcode type
   *  
   * @note UPC-A  format  [0][1-5][6-10][11]
   * 
   * @param mainBlock first portion of the list of symbols 
   */ 
  virtual QStringList encodeMainBlock(const shared::SymbolList& mainBlock) const;
  /**
    * Compress UPC-A barcode information into UPC-E format
    *
    *  		UPC-A				->  	UPC-E 			    
    * 1. manf dd[0-2]00, prod code 00ddd		->    	mmppp[0-2] 	
    * 2. manf dd[3-9]00,	prod code 000dd		->    	mmmpp3 		
    * 3. manf dddd0, prod code 0000d 		->    	mmmmp4 			 
    * 4. manf ddddd, prod code 0000[5-9] 	->	mmmmm[5-9] 	 
    * 
    * @param compressedUpc list of symbols, not including check digit
    * @param compressionMethod method used to compress the UPC-A code
    */
  shared::SymbolList compressUpc(const shared::SymbolList& inputSymbolList) const;
};
};
#endif // UPCAENGINE_H

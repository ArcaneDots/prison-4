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
  class UpcAEnginePrivate;
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
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  /**
   * Pass-thru constructor
   *
   * @param productCode constant indicating the current product code
   **/
  UpcAEngine(const QList<Symbol> &userBarcode, 
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  /**
   * destructor
   */
  virtual ~UpcAEngine();
  /**
   * Encoded barcode sections
   * 
   * [block1][block2][extendedBlock(if defined)]
   */
    const QList<QStringList> encoded() const;
  /**
   * Get a list of symbol blocks formatted according to the barcode's specification
   */
  const QStringList formatedSymbols() const;
protected:
  /**
   * Encode complete number according to current barcode type
   *  
   * @note UPC-A  format  [0][1-5][6-10][11]
   * 
   * @param mainBlock first portion of the list of symbols 
   */ 
  QList< QStringList > encodeMainBlock(const QList<Symbol>& mainBlock) const;
  /**
    * Compress UPC-A barcode information into UPC-E format
    *
    *  		UPC-A				->  	UPC-E   
    * 1. manf dd[0-2]00, prod code 00ddd	->    	mmppp[0-2]
    * 2. manf dd[3-9]00, prod code 000dd	->    	mmmpp3 
    * 3. manf dddd0, prod code 0000d 		->    	mmmmp4  
    * 4. manf ddddd, prod code 0000[5-9]	->    	mmmmm[5-9] 	 
    * 
    * @param inputSymbolList list of symbols, not including the extended block
    */
  QList<Symbol> compressUpc(const QList<Symbol>& inputSymbolList) const;
  /**
    * Expand UPC-E barcode information into UPC-A format
    *
    *  		UPC-A				->  	UPC-E 			    
    * 1. mmppp[0-2] 	->    	manf dd[0-2]00, prod code 00ddd		
    * 2. mmmpp3 	->    	manf dd[3-9]00,	prod code 000dd	
    * 3. mmmmp4 	->    	manf dddd0, prod code 0000d 				 
    * 4. mmmmm[5-9] 	->	manf ddddd, prod code 0000[5-9] 	 
    * 
    * @param inputSymbolList list of symbols, not including the extended block
    */
  QList<Symbol> expandUpc(const QList<Symbol>& inputSymbolList) const;

 UpcAEngine(UpcAEnginePrivate &d);
private:
  Q_DECLARE_PRIVATE(UpcAEngine);
    
};
}
#endif // UPCAENGINE_H

/*
    Copyright (C) 2011  Ian gmail <ianhollander@gmail.com>

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


#ifndef EAN13ENGINE_H
#define EAN13ENGINE_H

#include "upcaengine.h"

namespace product
{
  
/**
 * EAN-13 barcode generator
 */
class Ean13Engine :  public ProductEngine 
{
public:
  /**
   * @brief default constructor
   */
  Ean13Engine();
  /**
   * String Constructor
   *
   * @param productCode constant indicating the current product code
   */
  Ean13Engine(const QString &userBarcode, 
	      CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  /**
   * Symbol Constructor
   *
   * @param productCode constant indicating the current product code
   **/
  Ean13Engine(const QList<shared::Symbol>& userBarcode,
	      CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  /**
   * destructor
   */
  virtual ~Ean13Engine();     
  /**
   * Encoded barcode sections
   * 
   * [block1][block2][extendedBlock(optional)]
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
   * @note EAN-13  format  [0][1-6][7-12][]
   *
   * @param mainBlock first portion of the list of symbols
   */ 
  QList<QStringList> encodeMainBlock(const shared::SymbolList& mainBlock) const; 
private:
  /**
   * Private barcode information
   */
  class Private;
  Private * d;
};
};
#endif // EAN13ENGINE_H

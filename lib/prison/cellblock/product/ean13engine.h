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
class UpcEEngine;

class Ean13Engine :  public ProductEngine 
{
public:
  /**
   * Default Constructor
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
  Ean13Engine(const QList<shared::Symbol>& userBarcode, CodeEngine::ConstructCodes flags = 
CodeEngine::AutoProduct);
  /**
   * @brief UpcAEngine copy constructor
   * 
   * @param existingUpcA ...
   */
  Ean13Engine(const UpcAEngine& existingUpcA);
  /**
   * @brief UpcEEngine copy constructor
   * 
   * @param existingUpcE ...
   */
  Ean13Engine(const UpcEEngine &existingUpcE);
  //Ean13Engine(const Ean13Engine &existingEan13);
  /**
   * Destructor
   */
  virtual ~Ean13Engine();
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
protected:  
  /**
   * Ean13Engine Class specicfic initialization
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
   * 
   * @sa fillSystemEncodingList
   */
  void populateSections();  
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
   * Get productCode specific encoding pattern for the first block of symbols
   * 
   * @param indexedPattern index of assiocated pattern
   */
  QString getFirstBlockEncodePattern(int indexedPattern = 0) const;
    /**
   * Load all encoding patterns based on combo of system number (0-1) and check digit
   */
  void fillSystemEncodingList(); 
  /**
   * encoding patterns for EAN-13 first block
   */
  QStringList m_parity13WidthEncoding;
};
};
#endif // EAN13ENGINE_H

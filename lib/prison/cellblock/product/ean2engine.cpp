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

#include "ean2engine.h"

using namespace product;

Ean2Engine::Ean2Engine(const QString& defaultString,
		       int blockSize,
		       upc_common::PRODUCT_CODE_VALUES productCode) : 
		       UpcAEngine(defaultString,
					blockSize,
					blockSize,
					NOT_FOUND,
					blockSize,
					productCode)
{
  qDebug("UpcAEngine constructor()"); 
}

Ean2Engine::~Ean2Engine()
{
  // empty
}

QStringList Ean2Engine::processSymbolList(const QStringList &userSymbols)
{
  qDebug("Ean2Engine processSymbolList()"); 
  Q_ASSERT(upc_common::PS__EAN_2 == m_productCode);
  // don't bother processing two digits
  return userSymbols;
}


int Ean2Engine::calculateCheckDigit(const shared::LookupIndexArray& symbolArray) const
{
  qDebug("Ean2Engine calculateCheckDigit()"); 
  return UpcAEngine::calculateEan2CheckDigit(symbolArray);
}

void Ean2Engine::formatSymbols(const QStringList &symbolSrc, int splitIndex)
{
  qDebug("Ean2Engine formatSymbols()"); 
  Q_ASSERT(upc_common::PS__EAN_2 == m_productCode);
  UpcAEngine::formatExtendedBlock(symbolSrc);
  //m_formatedSymbols << m_userSymbols.join("");
}

void Ean2Engine::encodeSymbols(const QStringList &symbolSrc, int splitIndex)
{
  qDebug("Ean2Engine encodeSymbols()"); 
  Q_ASSERT(upc_common::PS__EAN_2 == m_productCode);
  UpcAEngine::encodeExtendedBlock(symbolSrc);
  //UpcAEngine::encodeExtendedDigits(convertSymbolsToIndexes(m_userSymbols));
}





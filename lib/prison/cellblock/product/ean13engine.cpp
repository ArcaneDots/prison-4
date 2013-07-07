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

#include <algorithm>
#include "ean13engine.h"

using namespace product;
Ean13Engine::Ean13Engine(const QString &userBarcode, 
			 CodeEngine::ConstructCodes flags): 
	UpcAEngine(userBarcode, 
		  flags,
		  ean13::DEFAULT_VALUE,
		  ean13::MIN,
		  ean13::MAX_LEN,
		  ean13::CHECK_DIGIT_OFFSET,
		  ean13::BLOCK_SIZE,
		  upc_common::PS__EAN_13)
{
  qDebug("Ean13Engine constructor");
  initialize();
  UpcAEngine::setBarcodeString();
}

Ean13Engine::~Ean13Engine()
{
  qDebug("Ean13Engine destructor");
} 

void Ean13Engine::initialize()
{
  qDebug("Ean13Engine initialize");
  product::ProductEngine::initialize();
  fillWidthEncodingList();
}

QStringList Ean13Engine::toUpcE() const
{
  qDebug("Ean13Engine toUpcE");
  if (m_userParsedSymbols.at(1) == "0" || m_userParsedSymbols.at(1) == "1") {
    return compressUpc(m_userParsedSymbols.mid(1));
  }
  return QStringList();
}

QStringList Ean13Engine::toUpcA() const
{  
  qDebug("Ean13Engine toUpcA");
  if (m_userParsedSymbols.at(0) == "0") {
    return m_userParsedSymbols.mid(1);
  }
  return QStringList();
}

QStringList Ean13Engine::toEan13() const
{
  qDebug("Ean13Engine toEan13");
  return m_userParsedSymbols;
}

QString Ean13Engine::getFirstBlockEncodePattern(int indexedPattern) const
{
  qDebug("Ean13Engine encodeMainBlock() : EAN-13 pattern "); 
  Q_ASSERT(indexedPattern >= 0 && 
    indexedPattern <= m_parity13WidthEncoding.size()); 
  return m_parity13WidthEncoding.at(indexedPattern);
}

void Ean13Engine::fillWidthEncodingList()
{  
  qDebug("Ean13Engine fillWidthEncodingList() : start");
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
    m_parity13WidthEncoding.append(ean13::PARITY_13[i]);
  }  
  qDebug("Ean13Engine fillWidthEncodingList() : end");
}

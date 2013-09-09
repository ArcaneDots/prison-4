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
#include "upceengine.h"

using namespace product;

// default constructor
Ean13Engine::Ean13Engine(): 
  ProductEngine("", CodeEngine::AutoProduct, upc_common::PS__EAN_13)
{
  qDebug("Ean13Engine default constructor");
  localInitialize();  
}

// user constructor
Ean13Engine::Ean13Engine(const QString &userBarcode, 
			 CodeEngine::ConstructCodes flags): 
  ProductEngine(userBarcode, flags, upc_common::PS__EAN_13)
{
  qDebug("Ean13Engine user constructor");
  localInitialize();
}

Ean13Engine::Ean13Engine(const QList<Symbol> & userSymbols, 
			 CodeEngine::ConstructCodes flags) : 
  ProductEngine(userSymbols, flags, upc_common::PS__EAN_13)
{
  qDebug("Ean13Engine symbol constructor");
  localInitialize();
}


// UpcAEngine copy constructor
Ean13Engine::Ean13Engine(const UpcAEngine& existingUpcA) :
  ProductEngine(existingUpcA.symbols(), CodeEngine::AutoProduct, 
		upc_common::PS__EAN_13)
  
{
  qDebug("Ean13Engine copy constructor");
  QStringList userParsedSymbols = existingUpcA.parsedSymbolList();
  userParsedSymbols.prepend("0");  
  Ean13Engine temp13(userParsedSymbols.join(""));
  //localInitialize();
  
  swap(temp13);
}


Ean13Engine::~Ean13Engine()
{
  qDebug("Ean13Engine destructor");
}

const QStringList Ean13Engine::formatedSymbols() const
{
  QStringList formatedString;
  formatedString << systemDigit();
  formatedString << block1().join("");
  formatedString << block2().join("");
  return formatedString;
}

const QStringList Ean13Engine::encoded() const
{
  QStringList encoded(encodeMainBlock(m_finalSymbolList));
  encoded << encodeExtendedBlock(local_extendedBlock());
  return encoded;
}

QStringList Ean13Engine::encodeMainBlock(const SymbolList& mainBlock) const
{
  qDebug("UpcAEngine encodeSymbols() : start");    
  int blockSize = ean13::BLOCK_SIZE;
  // Zeros
  QString defaultSymbols = QString(blockSize, '0');
  SymbolList block1, block2 = m_emptySymbol.parse(defaultSymbols);
  // "O" and "R"
  QString pattern1 = QString(blockSize, 'O');
  QString pattern2 = QString(blockSize, 'R');
  
  if (mainBlock.size() == ean13::CHECK_DIGIT_OFFSET + 1) {
    block1 = mainBlock.mid(1, blockSize);
    block2 = mainBlock.mid(blockSize + 1);
  }
  Symbol systemSymbol = m_finalSymbolList.at(0);
  if (m_parity13WidthEncoding.contains(systemSymbol)) {
    pattern1 = m_parity13WidthEncoding.at(systemSymbol);
  }
  
  QStringList encodeMainBlock;
  encodeMainBlock[0] = encodeSymbolParity(block1, pattern1).join("");  
  encodeMainBlock[1] = encodeSymbolParity(block2, pattern2).join("");
  qDebug("UpcAEngine encodeSymbols() : end");  
}


const QString Ean13Engine::numberSystem() const
{
  return m_finalSymbolList.at(0);
}

const QStringList Ean13Engine::block1() const
{
  return toStringList(m_finalSymbolList.mid(1, fmtBlockSize()));
}

const QStringList Ean13Engine::block2() const
{
  return toStringList(m_finalSymbolList.mid(1 + fmtBlockSize(), fmtBlockSize()));
}

void Ean13Engine::localInitialize()
{
  qDebug("Ean13Engine initialize");
  fillSystemEncodingList();
  
  m_finalSymbolList = processSymbolList(m_userParsedSymbols);
  Symbol calculatedCheckDigit = CalculateCheckDigit();
  m_isValid = validateCheckDigit(local_checkDigit(), calculatedCheckDigit);
  setCheckDigit(calculatedCheckDigit);
  
  populateSections();
}

void Ean13Engine::populateSections()
{
  qDebug() << "MAIN_BLOCK "<< mainBlock(); 
  qDebug() << "EXTENDED_BLOCK "<< extendedBlock();
  
  qDebug() << "MAIN_SYSTEM " << systemDigit();
  qDebug() << "MAIN_BLOCK_1 "<< block1();
  qDebug() << "MAIN_BLOCK_2 "<< block2();  
  qDebug() << "MAIN_CHECK_DIGIT "<< checkDigit();
}

QString Ean13Engine::getFirstBlockEncodePattern(int indexedPattern) const
{
  qDebug("Ean13Engine encodeMainBlock() : EAN-13 pattern "); 
  Q_ASSERT(indexedPattern >= 0 && 
    indexedPattern <= m_parity13WidthEncoding.size()); 
  return m_parity13WidthEncoding.at(indexedPattern);
}

void Ean13Engine::fillSystemEncodingList()
{  
  qDebug("Ean13Engine fillWidthEncodingList() : start");
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
    m_parity13WidthEncoding.append(ean13::PARITY_13[i]);
  }  
  qDebug("Ean13Engine fillWidthEncodingList() : end");
}

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

#include <QtDebug>
#include "ean8engine.h"

using namespace product;
using namespace shared;

Ean8Engine::Ean8Engine(): 	
	ProductEngine("", 
		CodeEngine::AutoProduct,
		upc_common::PS__EAN_8)
{
  qDebug("Ean8Engine constructor");
  localInitialize();
}

Ean8Engine::Ean8Engine(const QString &userBarcode, 
		       CodeEngine::ConstructCodes flags): 	
	ProductEngine(userBarcode, 
		flags,
		upc_common::PS__EAN_8)
{
  qDebug("Ean8Engine constructor");
  localInitialize();
}

Ean8Engine::~Ean8Engine()
{
  qDebug("Ean8Engine destructor");
  localInitialize();
}

void Ean8Engine::localInitialize()
{
  m_finalSymbolList = processSymbolList(m_userParsedSymbols);
  Symbol calculatedCheckDigit = CalculateCheckDigit();
  m_isValid = validateCheckDigit(local_checkDigit(), calculatedCheckDigit);
  setCheckDigit(calculatedCheckDigit);
  
  populateSections();
}

const QStringList Ean8Engine::formatedSymbols() const
{
  QStringList formatedString;
  formatedString << systemDigit();
  formatedString << block1().join("");
  formatedString << block2().join("");
  formatedString << checkDigit();
  return formatedString;
}

const QString Ean8Engine::numberSystem() const
{
  return m_finalSymbolList.at(0);
}

const QStringList Ean8Engine::block1() const
{
  return toStringList(m_finalSymbolList.mid(1, fmtBlockSize()));
}

const QStringList Ean8Engine::block2() const
{
  return toStringList(m_finalSymbolList.mid(1 + fmtBlockSize(), fmtBlockSize()));
}

const QStringList Ean8Engine::encoded() const
{  
  QStringList encodedBlocks(encodeMainBlock(m_finalSymbolList));
  encodedBlocks << encodeExtendedBlock(local_extendedBlock()).join("");
}

void Ean8Engine::populateSections()
{  
  qDebug() << "MAIN_BLOCK "<< mainBlock(); 
  qDebug() << "EXTENDED_BLOCK "<< extendedBlock();
  
  qDebug() << "MAIN_SYSTEM " << systemDigit();
  qDebug() << "MAIN_BLOCK_1 "<< block1();
  qDebug() << "MAIN_BLOCK_2 "<< block2();  
  qDebug() << "MAIN_CHECK_DIGIT "<< checkDigit();
}

QStringList Ean8Engine::encodeMainBlock(const SymbolList &mainBlock) const
{  
  qDebug("Ean8Engine encodeMainDigits() : start");
  int blockSize = ean8::BLOCK_SIZE;
  // Zeros
  QString defaultSymbols = QString(blockSize, '0');
  SymbolList block1, block2 = m_emptySymbol.parse(defaultSymbols);
  // "O" and "R"
  QString patternOs = QString(blockSize, 'O');
  QString patternRs = QString(blockSize, 'R');
  
  if (mainBlock.size() == ean8::CHECK_DIGIT_OFFSET + 1) {
    block1 = mainBlock.mid(0, blockSize);
    block2 = mainBlock.mid(blockSize);
  }
  
  QStringList encodeMainBlock;
  encodeMainBlock[0] = encodeSymbolParity(block1, patternOs).join("");  
  encodeMainBlock[1] = encodeSymbolParity(block2, patternRs).join("");
  qDebug("Ean8Engine encodeMainDigits() : end");
  return encodeMainBlock;
}
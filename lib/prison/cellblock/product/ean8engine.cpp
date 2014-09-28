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
#include "ean8engine_p.h"

using namespace ean8;



Ean8Engine::Ean8Engine(const QString &userBarcode, 
	CodeEngine::ConstructCodes flags) : 
	ProductEngine(*new Ean8EnginePrivate())
	//ProductEngine(userBarcode, flags, upc_common::PS__EAN_8)
{
  qDebug("Ean8Engine constructor");
}

Ean8Engine::Ean8Engine(const barcodeEngine::SymbolList& userBarcode, 
		       CodeEngine::ConstructCodes flags) :
	ProductEngine(*new Ean8EnginePrivate())
	//ProductEngine(toStrings(userBarcode), flags,
	//	      upc_common::PS__EAN_8)
{
  qDebug("UpcAEngine constructor::symbol");
}

Ean8Engine::Ean8Engine(Ean8EnginePrivate& d): 	
	//ProductEngine("", CodeEngine::AutoProduct, upc_common::PS__EAN_8)
	ProductEngine(d)
{
  qDebug("Ean8Engine constructor");
}

Ean8Engine::~Ean8Engine()
{
  qDebug("Ean8Engine destructor");
}

const QStringList Ean8Engine::formatedSymbols() const
{
  qDebug("Ean8Engine::formatedSymbols : start"); 
  QStringList formatedString;
  formatedString << QString();
  formatedString << block1().join("");
  formatedString << block2().join("");
  formatedString << QString();
  formatedString << extendedBlock().join("");
  qDebug() << "UpcAEngine::formatedSymbols : " << formatedString;
  qDebug("Ean8Engine::formatedSymbols : end");
  return formatedString;
}

const QList<QStringList> Ean8Engine::encoded() const
{  
  QList<QStringList> encodedBlocks(encodeMainBlock(local_mainBlock()));
  encodedBlocks += encodeExtendedBlock(local_extendedBlock());
  return encodedBlocks;
}

QList<QStringList> Ean8Engine::encodeMainBlock(const barcodeEngine::SymbolList& mainBlock) const
{  
  qDebug("Ean8Engine encodeMainDigits() : start");
  int encBlockSize = ean8::ENCODE_BLOCK_SIZE;
  QList<barcodeEngine::Symbol> l_mainBlock(local_mainBlock());

  QList<QStringList> encodeMainBlock;
  if (l_mainBlock.isEmpty()) {
    QStringList e_block;
    for (int count = 0; count < encBlockSize; count++) {
      e_block.append(barcodeEngine::Symbol::ERROR_ENCODING);
    }
    encodeMainBlock.append(e_block);
    encodeMainBlock.append(e_block);
  } else {
    barcodeEngine::SymbolList l_block1 = l_mainBlock.mid(0, encBlockSize);
    barcodeEngine::SymbolList l_block2 = l_mainBlock.mid(encBlockSize, encBlockSize);
    // "O" and "R"
    QString patternOs = QString(encBlockSize, 'O');
    QString patternRs = QString(encBlockSize, 'R');

    encodeMainBlock.append( encodeSymbolParity(l_block1, patternOs) );
    encodeMainBlock.append( encodeSymbolParity(l_block2, patternRs) );
    qDebug() << "Ean8Engine::encodeMainBlock() : l_block1" << encodeMainBlock.at(0);
    qDebug() << "Ean8Engine::encodeMainBlock() : l_block2" << encodeMainBlock.at(1);
  }
  qDebug("Ean8Engine encodeMainDigits() : end");
  return encodeMainBlock;
}
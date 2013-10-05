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

namespace product {
/**
 * @cond PRIVATE
 */
class Ean13Engine::Private
{
public:
  Private();
  virtual ~Private();
  /**
   * Get productCode specific encoding pattern for the first block of symbols
   *
   * @param indexedPattern index of assiocated pattern
   */
  QString getFirstBlockEncodePattern(int indexedPattern = 0) const;
private:
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
/**
 * @endcond
 */

using namespace product;
using namespace shared;
// ----------------- Ean13Engine::Private -----------------------
Ean13Engine::Private::Private() : 
  m_parity13WidthEncoding()
{
  fillSystemEncodingList();
}

Ean13Engine::Private::~Private()
{
  // empty
}

void Ean13Engine::Private::fillSystemEncodingList()
{
  qDebug("ProductEngine::Private fillWidthEncodingList() : start");
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
    m_parity13WidthEncoding.append(ean13::PARITY_13[i]);
  }
  qDebug("ProductEngine::Private fillWidthEncodingList() : end");
}

QString Ean13Engine::Private::getFirstBlockEncodePattern(int indexedPattern) const
{
  qDebug("Ean13Engine getFirstBlockEncodePattern() : EAN-13 pattern ");
  if(indexedPattern >= 0 &&
    indexedPattern <= m_parity13WidthEncoding.size()) {
    return m_parity13WidthEncoding.at(indexedPattern);
  } else {
    qDebug("Ean13Engine getFirstBlockEncodePattern() : bad index ");
    return QString("");
  }
}
// ----------------------- Ean13Engine -----------------------
Ean13Engine::Ean13Engine(): 
  ProductEngine("", CodeEngine::AutoProduct, upc_common::PS__EAN_13)
{
  qDebug("Ean13Engine default constructor");
}

// user constructor
Ean13Engine::Ean13Engine(const QString &userBarcode, 
			 CodeEngine::ConstructCodes flags): 
  ProductEngine(userBarcode, flags, upc_common::PS__EAN_13), d(new Private())
{
  qDebug("Ean13Engine user constructor");
}

Ean13Engine::Ean13Engine(const QList<shared::Symbol> & userSymbols, 
			 CodeEngine::ConstructCodes flags) : 
  ProductEngine(toStrings(userSymbols), flags, upc_common::PS__EAN_13), d(new Private())
{
  qDebug("Ean13Engine symbol constructor");
}

Ean13Engine::~Ean13Engine()
{
  qDebug("Ean13Engine destructor");
  delete d;
}

const QStringList Ean13Engine::formatedSymbols() const
{
  QStringList formatedString;
  formatedString << local_numberSystem();
  formatedString << toStrings(fmt_block1());
  formatedString << toStrings(fmt_block2());
  formatedString << QString("");
  formatedString << extendedBlock().join("");
  return formatedString;
}

const QList<QStringList> Ean13Engine::encoded() const
{
  QList<QStringList> encodedBlocks(encodeMainBlock(local_mainBlock()));
  encodedBlocks << encodeExtendedBlock(local_extendedBlock());
  return encodedBlocks;
}

QList< QStringList > Ean13Engine::encodeMainBlock(const shared::SymbolList& mainBlock) const
{
  qDebug("Ean13Engine encodeSymbols() : start");    
  int blockSize = ean13::BLOCK_SIZE;
  QList<shared::Symbol> l_mainBlock(local_mainBlock());

  QList<QStringList> encodeMainBlock;
  if (l_mainBlock.isEmpty()) {
    QStringList e_block;
    for (int count = 0; count < encBlockSize(); count++) {
      e_block.append(shared::Symbol::ERROR_ENCODING);
    }
    encodeMainBlock.append(e_block);
    encodeMainBlock.append(e_block);
  } else {
    shared::SymbolList l_block1 = l_mainBlock.mid(1, encBlockSize());
    shared::SymbolList l_block2 = l_mainBlock.mid(1 + encBlockSize(), encBlockSize());

    // block1
    int numberSystemIndex = local_numberSystem();
    qDebug() << "Ean13Engine encodeSymbols() : system digit = " << numberSystemIndex;
    QString pattern1 = QString(encBlockSize(), 'O');
    if (numberSystemIndex != Symbol::NOT_FOUND) {
      pattern1 = d->getFirstBlockEncodePattern(numberSystemIndex);
    }
    qDebug() << "Ean13Engine encodeSymbols() : pattern = " << pattern1;
    qDebug() << "Ean13Engine encodeSymbols() : symbols = " << l_block1;
    // block2
    QString patternRs = QString(encBlockSize(), 'R');
    qDebug("Ean13Engine encodeSymbols() : encode");
    encodeMainBlock.append( encodeSymbolParity(l_block1, pattern1) );
    encodeMainBlock.append( encodeSymbolParity(l_block2, patternRs) );
    qDebug() << "Ean13Engine::encodeMainBlock() : l_block1" << encodeMainBlock.at(0);
    qDebug() << "Ean13Engine::encodeMainBlock() : l_block2" << encodeMainBlock.at(1);
  }
  qDebug("UpcAEngine encodeMainBlock() : end");
  return encodeMainBlock; 
}





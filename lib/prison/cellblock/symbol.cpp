/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Ian gmail <ianhollander@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "symbol.h"


// class Symbol::BarcodeInfo : public QSharedData
// {
// public:
//   BarcodeInfo();
//   BarcodeInfo(const BarcodeInfo &other); 
//   /**
//    * symbol list and one to one encoding
//    */
//   BarcodeInfo(const QStringList &encodableSymbols, const QStringList &symbolEncodings);
//   /**
//    * visble symbols -> expanded to list of encodable symbols => symbol encodings
//    */
//   BarcodeInfo(const QStringList &expanedSymbols, const QStringList &encodableSymbols,   
// 	 const  const QStringList &symbolEncodings);
//   /**
//    * encodable symbol => mulitple symbol encodabling (based on list id)
//    */
//   BarcodeInfo(const QStringList &expanedSymbols, const EncodingList &symbolEncodings);
// protected:
//   /**
//    * list of QString pointer based on parser user input
//    */ 
//   std::list<QStringRef> m_userSymbolList;
//   /**
//    * user defined blocks of user parsed symbols  
//    */
//   std::map<QString, QStringRef> m_userBlocks;
//   /**
//    * list of user visible symbols 
//    */
//   QStringList m_visibleSymbolList;
//   /**
//    * list of encodable symbols 
//    */
//   QStringList m_encodableSymbolList;
//   /**
//    * 
//    */
//   std::map<QString, QStringList> m_encodingList;
// };
// 
// Symbol::BarcodeInfo::BarcodeInfo() : 
//   m_userSymbolList(),
//   m_userBlocks(),
//   m_visibleSymbolList(),
//   m_encodableSymbolList(),
//   m_encodingList()  
// {
//   // empty
// }
// Symbol::BarcodeInfo::BarcodeInfo(const BarcodeInfo &other) : 
//   QSharedData(other), 
//   m_userSymbolList(other.m_userSymbolList),
//   m_userBlocks(other.m_userBlocks),
//   m_visibleSymbolList(other.m_visibleSymbolList),
//   m_encodableSymbolList(other.m_encodableSymbolList),
//   m_encodingList(other.m_encodingList)
// { 
//   // empty
// }
// 
// Symbol::BarcodeInfo::BarcodeInfo(const QStringList& encodableSymbols, 
// 				 const QStringList& symbolEncodings): 
//   m_userSymbolList(),
//   m_userBlocks(),
//   m_visibleSymbolList(),
//   m_encodableSymbolList(encodableSymbols),
//   m_encodingList(symbolEncodings)
// {
// 
// }
// 
// Symbol::BarcodeInfo::BarcodeInfo(const QStringList& expanedSymbols, 
// 				 const QStringList& encodableSymbols, 
// 				 const QStringList& symbolEncodings): 
//   m_userSymbolList(),
//   m_userBlocks(),
//   m_visibleSymbolList(expanedSymbols),
//   m_encodableSymbolList(encodableSymbols),
//   m_encodingList(symbolEncodings)
// {
// 
// }
// 
// Symbol::BarcodeInfo::BarcodeInfo(const QStringList& expanedSymbols,
// 				 const EncodingList& symbolEncodings): 				 
//   m_userSymbolList(other.m_userSymbolList),
//   m_userBlocks(other.m_userBlocks),
//   m_visibleSymbolList(other.m_visibleSymbolList),
//   m_encodableSymbolList(other.m_encodableSymbolList),
//   m_encodingList(other.m_encodingList)
// {
// 
// }



// Symbol::Symbol(const QStringList& encodableSymbols, 
// 	       const QStringList& symbolEncodings)
// {
//   Q_ASSERT(encodableSymbols.size() == symbolEncodings.size());
//   
//   if (d != 0) {
//     d = new BarcodeInfo(sy);
//   }
// }
// 
// Symbol::Symbol(const QStringList& expanedSymbols, const QStringList& encodableSymbols, const QStringList& symbolEncodings)
// {
// 
// }
// 
// Symbol::Symbol(const QStringList& expanedSymbols, const EncodingList& symbolEncodings)
// {
// 
// }



Symbol::Symbol() :
  d(0),
  m_symbolIndex(NOT_FOUND),
  m_usesExtendedSet(false)
{
  
}

Symbol::Symbol(const Symbol& other) :
  d(other.d),
  m_symbolIndex(other.m_symbolIndex),
  m_usesExtendedSet(false)
{

}

Symbol::Symbol(const QSharedDataPointer<BarcodeSymbolSet> &BarcodeSymbolSet) :
  d(BarcodeSymbolSet),
  m_symbolIndex(NOT_FOUND),
  m_usesExtendedSet(false)
{
  
}

Symbol::Symbol(const QString &stringSymbol, 
	       const QSharedDataPointer<BarcodeSymbolSet> &BarcodeSymbolSet) :
  d(BarcodeSymbolSet),
  m_symbolIndex(BarcodeSymbolSet->getSymbolIndex(stringSymbol)),
  m_usesExtendedSet(false)
{
  
}

Symbol::Symbol(const QString &stringSymbol, bool fromExtendedSet,
	 const QSharedDataPointer<BarcodeSymbolSet> &BarcodeSymbolSet) :
  d(BarcodeSymbolSet),
  m_symbolIndex(BarcodeSymbolSet->getSymbolIndex(stringSymbol)),
  m_usesExtendedSet(fromExtendedSet)
{
  
}
	 

Symbol::~Symbol()
{

}

bool Symbol::isValid() const
{
  return (d != 0 && m_symbolIndex != NOT_FOUND);
}

QSharedDataPointer<BarcodeSymbolSet> Symbol::getFullsymbolSet() const 
{
  return d;
}
bool Symbol::isExtendedSet() const
{
  return m_usesExtendedSet;
}

QStringList Symbol::getEncoding() const
{
  return d->getSymbolEncoding(SymbolData(m_symbolIndex, m_usesExtendedSet));
}

QStringList Symbol::getEncoding(const QString& encodingKey) const
{
  return d->getSymbolEncoding(SymbolData(m_symbolIndex, m_usesExtendedSet), 
			      encodingKey);
}

QString Symbol::getString() const
{
  return d->getSymbolAtIndex(m_symbolIndex);
}

int Symbol::getIndex() const
{
  return m_symbolIndex;
}


// Symbol::Symbol(const QChar* unicode, int size)
// {
//   
// }
// 
// Symbol::Symbol(const QChar* unicode)
// {
// 
// }
// 
// Symbol::Symbol(QChar c)
// {
// 
// }
// 
// Symbol::Symbol(int size, QChar c)
// {
// 
// }
// 
// Symbol::Symbol(const QLatin1String& latin1)
// {
// 
// }
// 
// Symbol::Symbol(const QString& )
// {
// 
// }
// 
// Symbol::Symbol(const char* ch)
// {
// 
// }
// 
// Symbol::Symbol(const QByteArray& a)
// {
// 
// }
// 
// Symbol::Symbol(const QString::Null& )
// {
// 
// }
// 
// Symbol::Symbol(int size, Qt::Initialization )
// {
// 
// }
// 
// Symbol::Symbol()
// {
// 
// }

Symbol& Symbol::operator=(const Symbol& other)
{
    return *this;
}

bool Symbol::operator==(const Symbol& other) const
{
  return (d == other.d && m_symbolIndex == other.m_symbolIndex);
}



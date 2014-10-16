 
#ifndef ABSTRACTSYMBOLOGY_PRIVATE
#define ABSTRACTSYMBOLOGY_PRIVATE

#include <QMap>

#include "symbol.h"
#include "symbolnode.h"
#include "abstractsymbology.h"

namespace linearSymbology{
    
  

      

  // -------- Private ----------
class AbstractSymbologyPrivate 
{
//   typedef linearSymbology::hidden::SymbolNode SymbolNode;
public:
  AbstractSymbologyPrivate();
  AbstractSymbologyPrivate(const QScopedPointer<AbstractSymbologyPrivate> & other);
  virtual ~AbstractSymbologyPrivate();
  
  
  /// Initialize
  void init();
  
  virtual SymbolNode * findNode(int index,
	   const QString & symbolSet = shared::DEFAULT_SET) const;
  virtual SymbolNode * findNode(const QString& userSymbol,
	   const QString & symbolSet = shared::DEFAULT_SET) const;
 
  /**
    * Attempt to parse a single QString into a list of ordered Symbols
    * 
    * @must be "valid"
    */
    QList<Symbol> parse(const QString& userInput) const;
  /**
    * Attempt to parse a list of QStrings into a list of ordered Symbols
    * 
    * @must be "valid"
    */
  virtual QList<Symbol> parse(const QStringList& userInput) const;
      
  /// string version of integer value
  virtual QString str() const { return m_name; };

  QString m_name;
  SymbolNodeSet m_nodeSet;
  SymbolNodeSet m_expandedNodeList;
};


AbstractSymbologyPrivate::AbstractSymbologyPrivate() : 
  m_name("Abstract Symbology"),
  m_nodeSet(),
  m_expandedNodeList()
{  
  qDebug("SymbolPrivate::SymbolPrivate default constructor");
}

// SymbolPrivate::SymbolPrivate(const SymbolPrivate &other) : 
//   //QSharedData(other), 
//   m_data(other.m_data)
// {
//   // empty
// }

AbstractSymbologyPrivate::AbstractSymbologyPrivate(
  const QScopedPointer<AbstractSymbologyPrivate>& other) :
  m_name("Abstract Symbology"),
  m_nodeSet(other->m_nodeSet),
  m_expandedNodeList(other->m_expandedNodeList)
{

}


AbstractSymbologyPrivate::~AbstractSymbologyPrivate()
{
  // empty
}
//const AbstractSymbologyPrivate::NameString_AbstractSymbology = QString("Abstract Symbology");

void AbstractSymbologyPrivate::init()
{

}


QList<Symbol> AbstractSymbologyPrivate::parse(const QString& userInput) const
{  
  if (m_nodeSet.isEmpty() || userInput.isEmpty()) { 
    return QList<Symbol>();  
  }
  
  QList<Symbol> parsedSymbols;
  QString::const_iterator itrFirstString = userInput.begin();
  QString::const_iterator itrLastString = userInput.end(); 
  SymbolNodeSet::const_iterator itrFirstNode = m_nodeSet.constBegin();
  SymbolNodeSet::const_iterator itrLastNode = m_nodeSet.constEnd();
  while (itrFirstString != itrLastString) {
    //itrFirstNode = m_nodeList.find(Sy(*itrFirstString));
    if (itrFirstNode != itrLastNode) {
      Symbol wrapped(static_cast<SymbolNode *>(*itrFirstNode));
      parsedSymbols.append(wrapped);
    }
  }
  return parsedSymbols;
}

SymbolNode* AbstractSymbologyPrivate::findNode(int index, const QString& symbolSet) const
{
  SymbolNode * result = 0;
  
  SymbolNodeSet::const_iterator itrFirstNode = m_nodeSet.constBegin();
  SymbolNodeSet::const_iterator itrLastNode = m_nodeSet.constEnd(); 
  for(; itrFirstNode != itrLastNode; ++itrFirstNode) {
    if (static_cast<SymbolNode *>(*itrFirstNode)->index() == index) {
      result = *itrFirstNode;
    }
  }
  
  return result;
}

SymbolNode* AbstractSymbologyPrivate::findNode(const QString& userSymbol, const QString& symbolSet) const
{
  // SymbolNodeSet nodeList = m_nodeSet.toList();
  SymbolNodeSet::const_iterator itrEnd = m_nodeSet.constEnd();  
  SymbolNodeSet::const_iterator itrStart = m_nodeSet.constBegin();  
  
  SymbolNode * foundNode = 0;
  for (; itrStart != itrEnd; ++itrStart) {
    if (static_cast<SymbolNode *>(*itrStart)->toString() >= userSymbol) { 
      foundNode = static_cast<SymbolNode *>(*itrStart);
    }
  }
  
  return foundNode;
}

// void AbstractSymbologyPrivate::copy(QScopedPointer< AbstractSymbologyPrivate >& data)
// {
//   m_data = data->m_data;
// }
// 
// void AbstractSymbologyPrivate::copy(const QScopedPointer< AbstractSymbologyPrivate >& data)
// {
//   m_data = data->m_data;
// }


// /**
//  * 
//  * subclass need to layer conditions
//  */
// bool AbstractSymbologyPrivate::operator==(const AbstractSymbology& other) const
// {
//   return operator==(other.getIndex());
// }
// 
// bool AbstractSymbologyPrivate::operator==(int index) const
// {
//   return getIndex() == index;
// }


  
}

#endif // ABSTRACTSYMBOLOGY_PRIVATE
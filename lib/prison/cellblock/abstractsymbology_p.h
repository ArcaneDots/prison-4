 
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
public:
  AbstractSymbologyPrivate() : 
    m_name("Abstract Symbology"),
    m_nodeSet(),
    m_expandedNodeList()
  {  
    qDebug("SymbolPrivate::SymbolPrivate default constructor");
  }
  AbstractSymbologyPrivate(const QScopedPointer<AbstractSymbologyPrivate> & other):
    m_name("Abstract Symbology"),
    m_nodeSet(other->m_nodeSet),
    m_expandedNodeList(other->m_expandedNodeList){  /* empty */ }
  
  virtual ~AbstractSymbologyPrivate(){  /* empty */ }
  
  
  /// Initialize
  void init(){  /* empty? */ }
  
  virtual SymbolNode * findNode(int index,
	   const QString & symbolSet = shared::DEFAULT_SET) const
  {
    SymbolNode * foundNode = 0;
    
    SymbolNodeSet::const_iterator result = 
      std::find_if(m_nodeSet.constBegin(), 
		   m_nodeSet.constEnd(), 
		   find_index(index));
    if(result != m_nodeSet.constEnd()) {
      foundNode = static_cast<SymbolNode *>(*result);
    }
    
    return foundNode;
  }
 
 virtual SymbolNode * findNode(const QString& userSymbol,
	   const QString & symbolSet = shared::DEFAULT_SET) const
  {
    SymbolNode * foundNode = 0;
    
    SymbolNodeSet::const_iterator result = 
      std::find_if(m_nodeSet.constBegin(), 
		   m_nodeSet.constEnd(), 
		   find_string(userSymbol));
    if(result != m_nodeSet.constEnd()) {
      foundNode = static_cast<SymbolNode *>(*result);
    }
    
    return foundNode;
  }
 
  /**
    * Attempt to parse a single QString into a list of ordered Symbols
    * 
    * @note this version only handles single charactor symbols
    */
  virtual QList<Symbol> parse(const QString& userInput) const
  {  
    if (m_nodeSet.isEmpty() || userInput.isEmpty()) { 
      return QList<Symbol>();  
    }
    
    QList<Symbol> parsedSymbols;
    
    // try matching whole QString
    SymbolNode * foundNode = findNode(userInput);
    if (foundNode != 0) {
      parsedSymbols.append(foundNode);
    } else {
      QString::const_iterator itrFirstChar = userInput.begin();
      QString::const_iterator itrLastChar = userInput.end(); 
      for (;itrFirstChar != itrLastChar; ++itrFirstChar) {
	SymbolNode * foundNode = findNode(QString(*itrFirstChar));
	if (foundNode != 0) {
	  parsedSymbols.append(foundNode);
	}
      }
    }
    
    return parsedSymbols;
  }
  /**
    * Attempt to parse a list of QStrings into a list of ordered Symbols
    * 
    * @must be "valid"
    */
 virtual QList<Symbol> parse(const QStringList& userInput) const
 {
   if (m_nodeSet.isEmpty() || userInput.isEmpty()) { 
      return QList<Symbol>();  
    }
    
    QList<Symbol> parsedSymbols;
    QStringList::const_iterator itrFirstString = userInput.begin();
    QStringList::const_iterator itrLastString = userInput.end(); 
    while (itrFirstString != itrLastString) {
      parsedSymbols += parse(userInput);
    }
    return parsedSymbols;
 }
      
  /// string version of integer value
  virtual QString str() const { return m_name; };

  QString m_name;
  SymbolNodeSet m_nodeSet;
  SymbolNodeSet m_expandedNodeList;
};
  
}

#endif // ABSTRACTSYMBOLOGY_PRIVATE
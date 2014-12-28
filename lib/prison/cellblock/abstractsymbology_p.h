 
#ifndef ABSTRACTSYMBOLOGY_PRIVATE
#define ABSTRACTSYMBOLOGY_PRIVATE

#include <QMap>

#include "symbol.h"
#include "symbolnode_p.h"
#include "abstractsymbology.h"

namespace linearSymbology{
    
  struct find_string {
    find_string(const QString & string) : symbolString(string) {}
    bool operator()(const SymbolNode & node) const {
      return node.toString() == symbolString;
    }
  private:
    QString symbolString;
  };
  
  struct find_index {
    find_index(int index) : m_index(index) {}
    bool operator()(const SymbolNode & node) const {
      return node.index() == m_index;
    }
  private:
    int m_index;
  };
  
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
  AbstractSymbologyPrivate(const AbstractSymbologyPrivate & other):
    m_name("Abstract Symbology"),
    m_nodeSet(other.m_nodeSet),
    m_expandedNodeList(other.m_expandedNodeList){  /* empty */ }
  
  virtual ~AbstractSymbologyPrivate(){  /* empty */ }
  
  
  /// Initialize
  void init(){  /* empty? */ }
  
  virtual const SymbolNode * findNode(int index,
	   const QString & symbolSet = shared::DEFAULT_SET) const
  {
    const SymbolNode * foundNode = 0;
    
    SymbolNodeSet::const_iterator result = 
      std::find_if(m_nodeSet.constBegin(), 
		   m_nodeSet.constEnd(), 
		   find_index(index));
    if(result != m_nodeSet.constEnd()) {
      //foundNode = static_cast<SymbolNode *>(*result);
      foundNode = static_cast<const SymbolNode *>(&*result);
    }
    
    return foundNode;
  }
 
 virtual const SymbolNode * findNode(const QString& userSymbol,
	   const QString & symbolSet = shared::DEFAULT_SET) const
  {
    const SymbolNode * foundNode = 0;
    
    SymbolNodeSet::const_iterator result = 
      std::find_if(m_nodeSet.constBegin(), 
		   m_nodeSet.constEnd(), 
		   find_string(userSymbol));
    if(result != m_nodeSet.constEnd()) {
      foundNode = static_cast<const SymbolNode *>(&*result);
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
    const SymbolNode * foundNode = findNode(userInput);
    if (foundNode != 0) {
      parsedSymbols.append(foundNode);
    } else {
      QString::const_iterator itrFirstChar = userInput.begin();
      QString::const_iterator itrLastChar = userInput.end(); 
      for (;itrFirstChar != itrLastChar; ++itrFirstChar) {
	SymbolNode const * foundNode = findNode(QString(*itrFirstChar));
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
  QString m_errorEncoding;
  SymbolNodeSet m_nodeSet;
  SymbolNodeList m_expandedNodeList;
  
protected:
     
 
};
  
}

#endif // ABSTRACTSYMBOLOGY_PRIVATE
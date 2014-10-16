 
#ifndef SYMBOLS_NODE_H
#define SYMBOLS_NODE_H

#include <QMap>
#include <QStringList>

//#include "symbol.h"
#include "abstractsymbology.h"

namespace linearSymbology {
     
//     class Symbol;
    class AbstractSymbology;
    class AbstractSymbologyPrivate;

 
   class SymbolNode;
    
   //     typedef QMap<QString, QStringList> One2Many;
   typedef QMap<QString, QString> SymbolEncoding;
   //     typedef QMap<QString, QStringList> NamedSet;
   //     
   typedef QList<SymbolNode *> SymbolNodeList;
   typedef QSet<SymbolNode *> SymbolNodeSet;
   //    typedef QMap<QString, SymbolDataList> SymbolDataListMap;
   typedef QSet<SymbolNode> ExpandedSymbolsSet;
   typedef QList<SymbolNode> ExpandedSymbolsList;
   

   /**
    * Underling indiviual shared Symbol data info object
    */
   class SymbolNode 
   {
     friend AbstractSymbologyPrivate;
     
    public:      
      
      /**
       * default constructor - NULL
       */
      SymbolNode();
      
      /**
      * Copy constructor 
      */
      SymbolNode(const SymbolNode &other);
	
	   
      /**
       * "Empty Symbol" constructor
       */		
      SymbolNode(const AbstractSymbology* symbology);
      	      
      const AbstractSymbology* getSymbology() const;
      /**
       * Get syymbology name
       */
      QString symbologyName() const;
      /**
       * Named set symbol belongs to
       */
      QString namedSet() const;
      
      /**
       * Has an assocated symbology
       */
      bool isValid() const;
      /**
       * Represents a valid symbol
       */
      bool hasValue() const;
      
      QString toString() const { return m_string; }
      /**
       * Symbol's set index
       */
      int index() const;      
      /**
       * default encoding
       */
      QStringList encoding() const;      
      /**
       * single encoding
       */
      QStringList encoding(QChar encodingSetName) const;      
      /**
       * single encoding
       */
      QStringList encoding(const QString &encodingSetName) const;
      /**
       *
       */
      bool operator() (const QString& lhs, const QString& rhs) const;
	
      bool operator< (const SymbolNode &other) const;
      bool operator== (const SymbolNode &other) const;
      bool operator== (const QString &string) const;
      
      operator QString() const {return m_string;}
    protected:   
      /**
       * "Empty Symbol" constructor
       */	
      SymbolNode(const linearSymbology::AbstractSymbology& symbology, 
		 int index, const QString& string, 
		 const QString& namedSymbolSet = shared::DEFAULT_SET, 
		 bool isVisible = true);
//       /**
//       * 1 to 1 - Visible symbology to Encoding pair
//       */
//      SymbolNode(const AbstractSymbology &symbology, 
// 		int index, 
// 		QString setName = DEFAULT_SET) :
// 	  m_symbology(symbology), 
// 	  m_index(index),
// 	  m_setName(setName)
//       {
// 	
//       }
//       
//       
//       SymbolNode(QString SymbolString, 
// 		  SymbolEncoding &EncodingPairList,
// 		  QString SetName = DEFAULT_SET) :
// 	m_symbol(SymbolString),
// 	m_setName(SetName),
// 	       m_expandedNodes(0),
// 	m_encoding(&EncodingPairList) 
//       {
// 	Q_ASSERT(!SymbolString.isEmpty());
//       
//       }
//       
//       SymbolNode(QString SymbolString, 
// 		  ExpandedSymbolsSet &ExpandedSymbols, 
// 		  QString SetName = DEFAULT_SET) :
// 	m_symbol(SymbolString),
// 	m_setName(SetName),
// 	       m_expandedNodes(&ExpandedSymbols),
// 	m_encoding(0) 
//       {
// 	Q_ASSERT(!SymbolString.isEmpty());
//       
//       }
      
    private:
      
      const AbstractSymbology * m_symbology;
      
      // set name; null if in the DEFAULT_SET 
      QString m_namedSet;
      // index of symbol in "symbol set" list 
      const int m_index;
      QString m_string;
      bool m_isVisible;
      // pointer to expanded SymbolNodes
      QScopedPointer<ExpandedSymbolsList> m_expandedNodes;
      // pointer to encoding map
      QScopedPointer<SymbolEncoding> * m_encoding;
      QStringList m_defaultEncoding;
    };  
  
bool operator<= (const SymbolNode& left, const SymbolNode& right);
bool operator>= (const SymbolNode& left, const SymbolNode& right);

}


#endif // SYMBOLS_NODE_H
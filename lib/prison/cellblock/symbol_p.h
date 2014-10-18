 
#ifndef SYMBOLS_PRIVATE
#define SYMBOLS_PRIVATE

#include <QMap>

#include "symbol.h"
#include "abstractsymbology.h"

namespace linearSymbology
{         
//    const int NOT_FOUND = -1;  
//    const QString DEFAULT_SET("__DEFAULT_SET__");
//    const QString EmptyString("");
//    
//    class SymbolNode;
//     
//    //     typedef QMap<QString, QStringList> One2Many;
//    typedef QMap<QString, QString> SymbolEncoding;
//    //     typedef QMap<QString, QStringList> NamedSet;
//    //     
//    typedef QList<SymbolNode *> SymbolNodeList;
//    typedef QSet<SymbolNode *> SymbolNodeSet;
//    //    typedef QMap<QString, SymbolDataList> SymbolDataListMap;
//    typedef QSet<SymbolNode> ExpandedSymbolsSet;
//    typedef QList<SymbolNode> ExpandedSymbolsList;
//    
// 
//    /**
//     * Underling indiviual shared Symbol data info object
//     */
//    class SymbolNode 
//    {
//      friend Symbol;
//      friend AbstractSymbologyPrivate;
//      
//     public:      
//       
//       /**
//        * default constructor - NULL
//        */
//       SymbolNode();
//       
//       /**
//       * Copy constructor 
//       */
//       SymbolNode(const SymbolNode &other);
// 	
// 	   
//       /**
//        * "Empty Symbol" constructor
//        */		
//       SymbolNode(const linearSymbology::AbstractSymbology* symbology);
//       	      
//       const linearSymbology::AbstractSymbology* getSymbology() const;
//       /**
//        * Get syymbology name
//        */
//       QString symbologyName() const;
//       /**
//        * Named set symbol belongs to
//        */
//       QString namedSet() const;
//       
//       /**
//        * Has an assocated symbology
//        */
//       bool isValid() const;
//       /**
//        * Represents a valid symbol
//        */
//       bool hasValue() const;
//       
//       QString toString() const { return m_string; }
//       /**
//        * Symbol's set index
//        */
//       int index() const;      
//       /**
//        * default encoding
//        */
//       QStringList encoding() const;      
//       /**
//        * single encoding
//        */
//       QStringList encoding(QChar encodingSetName) const;      
//       /**
//        * single encoding
//        */
//       QStringList encoding(const QString &encodingSetName) const;
//       /**
//        *
//        */
//       bool operator() (const QString& lhs, const QString& rhs) const;
// 	
//       bool operator< (const SymbolNode &other) const;
//       bool operator== (const SymbolNode &other) const;
//       bool operator== (const QString &string) const;
//       
//       operator QString() const {return m_string;}
//     protected:   
//       /**
//        * "Empty Symbol" constructor
//        */	
//       SymbolNode(const linearSymbology::AbstractSymbology& symbology, 
// 		 int index, const QString& string, 
// 		 const QString& namedSymbolSet = linearSymbology::DEFAULT_SET, 
// 		 bool isVisible = true);
// //       /**
// //       * 1 to 1 - Visible symbology to Encoding pair
// //       */
// //      SymbolNode(const AbstractSymbology &symbology, 
// // 		int index, 
// // 		QString setName = DEFAULT_SET) :
// // 	  m_symbology(symbology), 
// // 	  m_index(index),
// // 	  m_setName(setName)
// //       {
// // 	
// //       }
// //       
// //       
// //       SymbolNode(QString SymbolString, 
// // 		  SymbolEncoding &EncodingPairList,
// // 		  QString SetName = DEFAULT_SET) :
// // 	m_symbol(SymbolString),
// // 	m_setName(SetName),
// // 	       m_expandedNodes(0),
// // 	m_encoding(&EncodingPairList) 
// //       {
// // 	Q_ASSERT(!SymbolString.isEmpty());
// //       
// //       }
// //       
// //       SymbolNode(QString SymbolString, 
// // 		  ExpandedSymbolsSet &ExpandedSymbols, 
// // 		  QString SetName = DEFAULT_SET) :
// // 	m_symbol(SymbolString),
// // 	m_setName(SetName),
// // 	       m_expandedNodes(&ExpandedSymbols),
// // 	m_encoding(0) 
// //       {
// // 	Q_ASSERT(!SymbolString.isEmpty());
// //       
// //       }
//       
//     private:
//       
//       const AbstractSymbology * m_symbology;
//       
//       // set name; null if in the DEFAULT_SET 
//       QString m_namedSet;
//       // index of symbol in "symbol set" list 
//       const int m_index;
//       QString m_string;
//       bool m_isVisible;
//       // pointer to expanded SymbolNodes
//       QScopedPointer<ExpandedSymbolsList> m_expandedNodes;
//       // pointer to encoding map
//       QScopedPointer<SymbolEncoding> * m_encoding;
//       QStringList m_defaultEncoding;
//     };  
//   
// SymbolNode::SymbolNode() : 
// 	m_symbology(),
// 	m_index(linearSymbology::NOT_FOUND),
// 	m_string(linearSymbology::EmptyString),
// 	m_isVisible(true), 
// 	m_namedSet(linearSymbology::DEFAULT_SET),
// 	m_expandedNodes(0),
// 	m_encoding(0),
// 	m_defaultEncoding(linearSymbology::EmptyString)
// 	{
// 	  
// 	}
// 	
// SymbolNode::SymbolNode(const SymbolNode& other) : 
// 	m_symbology(other.m_symbology),
// 	m_index(other.m_index),
// 	m_string(other.m_string),
// 	m_isVisible(other.m_isVisible), 
// 	m_namedSet(other.m_namedSet),
// 	m_expandedNodes(other.m_expandedNodes.data()),
// 	m_encoding(other.m_encoding),
// 	m_defaultEncoding(other.m_defaultEncoding)/**/
// 	{
// 	  
// 	}
// 
// SymbolNode::SymbolNode(const linearSymbology::AbstractSymbology* symbology) :
//       m_symbology(symbology),      
//       m_index(linearSymbology::NOT_FOUND),
//       m_string(linearSymbology::EmptyString), 
//       m_namedSet(linearSymbology::DEFAULT_SET),
//       m_isVisible(true),
//       m_expandedNodes(0),
//       m_encoding(0),
//       m_defaultEncoding(linearSymbology::EmptyString)
//       {
// 	//m_defaultEncoding = ;
//       }
//  const linearSymbology::AbstractSymbology* SymbolNode::getSymbology() const
// {
//   return m_symbology;
// }
//       
// QString SymbolNode::symbologyName() const
//       {
// 	return !m_symbology ? linearSymbology::EmptyString : m_symbology->getName();
// 	//return m_symbology.isNull() ? EmptyString : m_symbology->getName();
//       }
// 
// QString SymbolNode::namedSet() const{
// 	return m_namedSet;
//       }
//       
// int SymbolNode::index() const {return m_index;}
// 
// QStringList SymbolNode::encoding() const
//       {
// 	return m_defaultEncoding;
//       }
// 
// QStringList SymbolNode::encoding(QChar encodingSetName) const
//       { 
// 	return encoding(QString(encodingSetName));
//       }
//       
// QStringList SymbolNode::encoding(const QString& encodingSetName) const
//       {	
// 	QStringList result;
// 	if (!m_expandedNodes.isNull()) {	
// 	  int count = m_expandedNodes->count();
// 	  for (int i = 0; i > count; ++i) {
// 	    result << m_expandedNodes->at(i).encoding(encodingSetName);
// 	  }
// 	} else if (m_encoding->isNull())  {
// 	    result <<  m_defaultEncoding;
// 	} else {
// 	  SymbolEncoding::const_iterator itEnc, itEnd = m_encoding->data()->constEnd();
// 	  itEnc = m_encoding->data()->find(encodingSetName);
// 	  if (itEnc != itEnd && itEnc.value().count() > m_index) {
// 	   result << itEnc->at(m_index);
// 	  }	
// 	}
// 	return result;
//       }
// 
// // int SymbolNode::getIndex() const
// // {
// //   return m_index;
// // //   return (!m_symbology) ? 0 : m_symbology->getIndex(index);
// // }
// 
// bool SymbolNode::isValid() const
// {
//   return (!m_symbology) ? false : true;
// }
// 
// bool SymbolNode::hasValue() const
// {
//   return (m_index != linearSymbology::NOT_FOUND && !m_symbology) ? false : true;
// }
// 
// bool SymbolNode::operator<(const SymbolNode& other) const
// {
//   return m_index < other.m_index;
// }
// 
// bool SymbolNode::operator==(const SymbolNode& other) const
// {
//  return (m_index == other.m_index) ;
// }
// 
// 
// bool SymbolNode::operator==(const QString& string) const
// {
//   return (m_string == string);
// }
// 
//       
// bool SymbolNode::operator()(const QString& lhs, const QString& rhs) const
//       {
// 	return lhs < rhs;
//       }
//       
// 
// 
// SymbolNode::SymbolNode(const linearSymbology::AbstractSymbology& symbology, int index, const QString& string, const QString& namedSymbolSet, bool isVisible) :
//       m_symbology(symbology),
//       m_index(index),
//       m_string(EmptyString), 
//       m_namedSet(namedSymbolSet),
//       m_isVisible(isVisible),
//       m_expandedNodes(0),
//       m_encoding(0),
//       m_defaultEncoding()
//       {
// 	if (!m_symbology) { return; }
// 	
// 	m_defaultEncoding << EmptyString;
// // 	m_expandedNodes = m_symbology.constData()->expandedSymbols();
// // 	m_encoding = m_symbology.constData()->encodings();
// 	
// 	SymbolEncoding::const_iterator itDefEncoding = m_encoding->data()->constEnd();
// 	SymbolEncoding::const_iterator itEncodingEnd = m_encoding->data()->constEnd();
// // 	itDefEncoding = m_encoding->data()->find(m_symbology->defaultEncoding());	
// 	if (itDefEncoding != itEncodingEnd) {
// 	  m_defaultEncoding << itDefEncoding.value();
// 	}	  
//       }  



// class SymbolPrivate 
//   {
//   public:  
//     /**
//      * Null
//      */
//     SymbolPrivate();
//     /**
//      * empty
//      */
//     SymbolPrivate(const linearSymbology::AbstractSymbology* symbology);
//     /**
//      * SymbolNode
//      */
//     SymbolPrivate(const SymbolNode &node);
//     
//     virtual ~SymbolPrivate();
//   
//     const SymbolNode* operator=(const linearSymbology::AbstractSymbology* other);
//     const SymbolNode* operator=(int index);
//     
//     /// Initialize
//     virtual void init(){};
//     
//     /// Valid object
//     bool isValid() const
//     { 
//       return !m_node ? false : m_node->isValid();
//     }
//     
//     /// Is set to a valid value
//     bool hasValue() const
//     { 
//       return !m_node ? false : m_node->isValid() && m_node->hasValue();
//     }
//     
//     // parse symbols - no "empty" symbols
//             
//     void setSymbolNode(const SymbolNode *node);
//     void setSymbology(const AbstractSymbology &symbology);
//     
//     // get symbol encoding
//     QString encoding() const;
//     QString encoding(const QChar& encodingType) const;
//     QString encoding(const QString& encodingType) const;
//         
//     /**
//      * Only compares index
//      */
//     bool operator==(int index) const;
//     bool operator!=(int index) const { return !(operator==(index)); }
//     
//     bool operator==(const SymbolNode* other) const;  
//         
//     /**
//     * Set symbol's value by index
//     */
//     SymbolNode* setSymbol(int index);
//     /**
//     * Set symbol's value by string version
//     * 
//     * @note case sensitive 
//     */
//     SymbolNode* setSymbol(const QString& string);
//     /**
//      * 
//      */
//     int getIndex() const; 
//     
//     
//     const SymbolNode * m_node; 
//     AbstractSymbology * m_symbology;
//   };
//   
//   SymbolPrivate::SymbolPrivate() : 
//     m_node(0),
//     m_symbology(0)
//   {
//     //qDebug("SymbolPrivate::SymbolPrivate default constructor");
//     init();    
//   }
// 
//   
//   SymbolPrivate::SymbolPrivate(const AbstractSymbology * symbology) :
//   m_node(0),
//   m_symbology(const_cast<AbstractSymbology *>(symbology))
//   {
//   }
// 
//   SymbolPrivate::SymbolPrivate(const SymbolNode& node) :
//   m_node(&node),
//   m_symbology() //static_cast<AbstractSymbology &>( node.m_symbology))
//   {
//     if (m_node != 0) { 
//       m_symbology = const_cast<AbstractSymbology *>(node.getSymbology()); 
//     }
//   }
// 
// 
// // SymbolPrivate::SymbolPrivate(const SymbolPrivate &other) : 
// //   //QSharedData(other), 
// //   m_data(other.m_data)
// // {
// //   // empty
// // }
// 
//   SymbolPrivate::~SymbolPrivate()
//   {
//     // empty
//   }
// 
// 
// 
// SymbolNode * SymbolPrivate::setSymbol(int index)
// {
//   SymbolNode * resultNode = const_cast<SymbolNode *>(m_node);
//   if (index != m_node->index()) {
//     const AbstractSymbology * symbology = m_node->getSymbology();
//     resultNode = symbology->findNode(index, m_node->namedSet()); 
//   }
//   
//   return resultNode;     
// }
// 
// SymbolNode * SymbolPrivate::setSymbol(const QString& userString)
// {
//   SymbolNode * resultNode = const_cast<SymbolNode *>(m_node);
//   if (userString != m_node->toString()) {
//     const AbstractSymbology * symbology = m_node->getSymbology();
//     resultNode = symbology->findNode(userString, m_node->namedSet()); 
//   }
//   
//   return resultNode;     
// }
// 
// void SymbolPrivate::setSymbology(const AbstractSymbology& symbology)
// {
// 
// }
// 
// QString SymbolPrivate::encoding() const
// {
// 
// }
// 
// QString SymbolPrivate::encoding(const QChar& encodingType) const
// {
// 
// }
// 
// QString SymbolPrivate::encoding(const QString& encodingType) const
// {
// 
// }
// int SymbolPrivate::getIndex() const
// {
// 
// }
// 
// 
// 
// // void SymbolPrivate::setSymbol(const QString& string)
// // {
// // 
// // }
// // void SymbolPrivate::setSymbolNode(const linearSymbology::SymbolNode& node)
// // {
// //   m_node = node;
// //   if (!m_node == false) { m_symbology = m_node->m_symbology; }
// // }
// // void SymbolPrivate::setSymbology(const AbstractSymbology& symbology)
// // {
// //   m_node = 0
// //   m_symbology = 0;
// // }
// 
// const SymbolNode* SymbolPrivate::operator=(int index)
// {
// 
// }
// 
// const SymbolNode* SymbolPrivate::operator=(const AbstractSymbology* other)
// {
//   SymbolNode * resultNode = const_cast<SymbolNode *>(m_node);
//   if ((const_cast<AbstractSymbology*>(other) != this->m_symbology) && (m_node != 0)) {
//     m_symbology->getSymbol(m_node->toString());
//   }
// }
//   
// 
// 
//   bool SymbolPrivate::operator==(int index) const
//   {
//     return getIndex() == index;
//   }
// 
//   bool SymbolPrivate::operator==(const SymbolNode* other) const
//   {
//     return (*m_node == *other);
//   }

  
}

#endif // SYMBOLS_PRIVATE
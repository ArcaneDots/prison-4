#ifndef SYMBOLNODE_PRIVATE_H
#define SYMBOLNODE_PRIVATE_H

#include "symbolnode.h"

namespace linearSymbology
{
 
  
  class SymbolNodePrivate 
  {
  public:
//     /**
//      * default constructor - Empty node 
//      */
//     SymbolNodePrivate() :
// 	m_symbology(0),
// 	m_string(shared::EmptyString),
// 	m_index(shared::NOT_FOUND),
// 	m_symbolSetName(shared::EmptyString),
// 	m_isVisible(false),
// 	m_defaultEncoding(shared::EmptyString),
// 	m_encodings(),
// 	m_encodingSetName(shared::EmptyString),
// 	m_expandedNodes(){ /* Empty */ }
//     /**
//      * copy constructor - Duplicate node
//      */
//     SymbolNodePrivate(linearSymbology::SymbolNodePrivate& other):
// 	m_symbology(other.m_symbology),
// 	m_string(other.m_string),
// 	m_index(other.m_index),
// 	m_symbolSetName(other.m_symbolSetName),
// 	m_encodingSetName(other.m_encodingSetName),
// 	m_isVisible(other.m_isVisible),
// 	m_defaultEncoding(other.m_defaultEncoding){ /* Empty */ }
//     /**
//      * 1 or many symbol sets to 1 encoding
//      * 
//      * @note code 39 or EAN-128
//      */
//     SymbolNodePrivate(const AbstractSymbology* symbology, 
// 		      const QString& symbolString, 
// 		      int index, 
// 		      QString defaultEncoding,
// 		      const QString& symbolSetName = shared::DEFAULT_SET, 
// 		      const QString& encodingSetName = shared::DEFAULT_SET, 
// 		      bool isVisible = true) :
// 	m_symbology(symbology),
// 	m_string(symbolString),
// 	m_index(index),
// 	m_defaultEncoding(defaultEncoding),
// 	m_symbolSetName(symbolSetName),
// 	m_encodingSetName(encodingSetName),
// 	m_isVisible(isVisible){ /* Empty */ }
// 	
//     /**
//      * 1 to many encodings
//      * 
//      * @note Product code (UPC/EAN-13/EAN-8)
//      */
//     SymbolNodePrivate(const linearSymbology::AbstractSymbology* symbology, 
// 		      const QString& symbolString, 
// 		      int index, 
// 		      const SymbolEncoding& encodings, 
// 		      const QString& symbolSetName = shared::DEFAULT_SET, 
// 		      const QString& encodingSetName = shared::DEFAULT_SET, 
// 		      bool isVisible = true):
// 	m_symbology(symbology),
// 	m_string(symbolString),
// 	m_index(index),
// 	m_encodings(encodings),
// 	m_symbolSetName(symbolSetName),
// 	m_encodingSetName(encodingSetName),
// 	m_isVisible(isVisible),
// 	m_defaultEncoding(shared::EmptyString),
// 	m_expandedNodes(){ /* Empty */ }
	
//     /**
//      * Expanded symbol node
//      * 
//      * m_index = shared::NOT_FOUND,
//      * m_encodingSetName = shared::EmptyString
//      */
//     SymbolNodePrivate(const linearSymbology::AbstractSymbology* symbology, 
// 		      const QString& symbolString, 
// 		      //composite symbols don't have an index 
// 		      const QString& encodingSetName = shared::DEFAULT_SET,
// 		      bool isVisible = true) :
// 	m_symbology(symbology),
// 	m_string(symbolString),
// 	m_index(shared::NOT_FOUND), // extend nodes don't have an index
// 	m_symbolSetName(symbolSet),
// 	m_isVisible(isVisible),
// 	m_defaultEncoding(shared::EmptyString),
// 	m_encodings(), // encoding are in expanded nodes
// 	m_encodingSetName(encodingSetName),
// 	m_expandedNodes()
//     {  
//       if (m_expandedNodes.isEmpty()) { return; }
//       
//       m_defaultEncoding.clear();
// 	
//       int count = m_expandedNodes.count();
//       for (int i = 0; i > count; ++i) {
// 	m_defaultEncoding << m_expandedNodes.at(i).encoding(m_encodingSetName);
//       }
//     }
//     /**
//      * Visible node with expanded symbol node encoding i.e. code 93
//      * 
//      * 
//      * m_encodings = 0
//      * expandedSymbolNodes != 0 
//      * m_encodingSetName = shared::EmptyString
//      */
//     SymbolNodePrivate(const linearSymbology::AbstractSymbology* symbology, 
// 		      const QString& symbolString, 
// 		      int index,
// 		      const SymbolNodePtrList& expandedSymbolNodes, 
// 		      const QString& symbolSet = shared::DEFAULT_SET,
// 		      bool isVisible = true) :	
// 	m_symbology(symbology),
// 	m_string(symbolString),
// 	m_index(index),
// 	m_symbolSetName(symbolSet),
// 	m_isVisible(isVisible),
// 	m_defaultEncoding(shared::EmptyString),
// 	m_encodings(), // encoding are in expanded nodes
// 	m_encodingSetName(shared::EmptyString),
// 	m_expandedNodes(expandedSymbolNodes)
//     {  
//       if (m_expandedNodes.isEmpty()) { return; }
//       
//       m_defaultEncoding.clear();
// 	
//       int count = m_expandedNodes.count();
//       for (int i = 0; i > count; ++i) {
// 	m_defaultEncoding << m_expandedNodes.at(i)->encoding(m_encodingSetName);
//       }
//     }
    

 //   ~SymbolNodePrivate(){ /* Empty */ };
    
    // pointer to source symbology object
    const AbstractSymbology * m_symbology;
    
    // Symbol
    QString m_string;
    // index of symbol in "symbol set" list 
    int m_index;
    // set name; null if in the DEFAULT_SET 
    QString m_symbolSetName;
    // Symbol displayed to user
    bool m_isVisible;
    
    // default encoding
    QStringList m_defaultEncoding;
    // set name of default encoding ("default" if only one set in symbology) 
    QString m_encodingSetName;
    // pointer to expanded SymbolNodes
    SymbolNodePtrList m_expandedNodes;
    SymbolEncoding m_encodings; 
    
  };

  


}

#endif // SYMBOLNODE_PRIVATE_H
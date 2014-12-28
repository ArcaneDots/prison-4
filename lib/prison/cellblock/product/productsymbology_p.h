 
#ifndef PRODUCT_SYMBOLS_PRIVATE
#define PRODUCT_SYMBOLS_PRIVATE

#include <QMap>

#include "../abstractsymbology_p.h"
#include "upceandefines_p.h"
namespace product{
  

//   
//   class InternalSymbolList
//   {
//     InternalSymbolList(){}
//     /**
//      * 1 to 1 mapping
//      */
//     InternalSymbolList(QStringList &VisibleSymbolsSet,
// 		       QStringList &EncodingParity) :
// 		       m_symbolListMap(new SymbolDataListMap),
// 		       m_expandedShards() // no expanded symbols		      
//     {
//       SharedSymbolDataList symbolDataSet;
//       if (VisibleSymbolsSet.length() == VisibleSymbolsSet.length()) 
//       {
// 	
// 	QStringList::const_iterator symbolBegin = VisibleSymbolsSet.constBegin();
// 	QStringList::const_iterator symbolEnd =  VisibleSymbolsSet.constEnd();
// 	QStringList::const_iterator encodingBegin = VisibleSymbolsSet.constBegin();
// 	QStringList::const_iterator encodingEnd =  VisibleSymbolsSet.constEnd();
// 	
// 	
// 	while (symbolBegin != symbolEnd) {
// 	  //SharedSymbolsData symbolData(*symbolBegin,QPair<QString, QString>(DEFAULT_SET, *encodingBegin
// 	  symbolDataSet += ProductNode(*symbolBegin, *encodingBegin);
// 	  ++symbolBegin;
// 	  ++encodingBegin;
// 	}
//       }
//       if (!symbolDataSet.isEmpty()) {
// 	m_symbolListMap->insert(DEFAULT_SET, symbolDataSet);
//       }
//     }
//     /**
//      * 1 to many mapping (expanded syboml set)
//      */
//     InternalSymbolList(QStringList &VisibleSymbols,
// 		       QStringList &ExpandedSymbols,
// 		       One2Many &VisibleToExpandedSymbols,
// 		       QStringList &ExpandedSymbolEncoding){}
//     
//     InternalSymbolList(NamedSet &VisibleSymbolSet,
// 		       NamedSet &EncodingSet)
//     {
//       NamedSet::const_iterator vfirst = VisibleSymbolSet.constBegin();
//       NamedSet::const_iterator vlast = VisibleSymbolSet.constEnd();
//       NamedSet::const_iterator efirst = EncodingSet.constBegin();
//       NamedSet::const_iterator elast = EncodingSet.constEnd();
//       
//       //  1 to 1 sets
//       if (VisibleSymbolSet.count() == EncodingSet.count()) {
// 	
// 	for (; vfirst != vlast; ++vfirst) {
// 	  const QString setName = vfirst.key();
// 	  NamedSet::const_iterator itFind = EncodingSet.constFind(setName);
// 	    if (itFind != elast) {
// 	      int l_symbols = vfirst.value().count();
// 	      
// 	      if (l_symbols == itFind.value().count()){
// 		SharedSymbolDataList l_symbolList;
// 	
// 		for (int i = 0; i < l_symbols; ++i) {
// 		  l_symbolList.append(vfirst.value().at(i), efirst.key().at(i));
// 		}
// 		
// 		if (l_symbolList.count() == l_symbols) {
// 		  m_symbolListMap->insert(setName, l_symbolList);
// 		}		
// 	      }	      
// 	    }	  
// 	}
//       }
//       // UPC/EAN (product codes) - 1 symbol to multiple encodings
//       else if (VisibleSymbolSet.count() = 1 && EncodingSet.count() > 1) {
// 	
// 	int l_symbols = vfirst.value().count();
// 	
// 	for (; efirst != elast; ++efirst) {
// 	  const QString setName = efirst.key();
// 	  
// 	  if (l_symbols == efirst.value().count()){
// 	    SharedSymbolDataList l_symbolList;
// 	   
// 	    for (int i = 0; i < l_symbols; ++i) {
// 	      l_symbolList.append(vfirst.value().at(i), efirst.key().at(i));	      
// 	    }
// 
// 	    if (l_symbolList.count() == l_symbols) {
// 	      m_symbolListMap->insert(setName, l_symbolList);
// 	    }
// 	      
// 	    
// 	}
//       } 
//       // EAN-128  - multiple symbol per index to 1 encodings 
//       else if (VisibleSymbolSet.count() > 1 && EncodingSet.count() == 1) {
// 	
// 	int l_symbols = efirst.value().count();
// 		
// 	for (; vfirst != vlast; ++vfirst) {
// 	  const QString setName = vfirst.key();
// 	  
// 	  if (l_symbols == vfirst.value().count()){
// 	    SharedSymbolDataList l_symbolList;
// 	   
// 	    for (int i = 0; i < l_symbols; ++i) {
// 	      l_symbolList.append(vfirst.value().at(i), efirst.key().at(i));	      
// 	    }
// 
// 	    if (l_symbolList.count() == l_symbols) {
// 	      m_symbolListMap->insert(setName, l_symbolList);
// 	    }	    
// 	 }
//       }
//     }
// 	
//       }
//       
//     }
//       
// 	
//       
// 		       
//   protected:
//     SymbolNodeSet m_symbolNodeList;
// //     SymbolDataListMap * m_symbolListMap;
// //     ExpandedSymbolsSet * m_expandedShards;*/
//   };
  
  using namespace linearSymbology;
  
  // -------- Private ----------
class ProductSymbologyPrivate : public AbstractSymbologyPrivate
{
public:  
  ProductSymbologyPrivate() : AbstractSymbologyPrivate()  { init(); }
  
  ProductSymbologyPrivate(const ProductSymbologyPrivate &other): 
    AbstractSymbologyPrivate(other){  /* empty */ }
 
 virtual ~ProductSymbologyPrivate(){  /* empty */ }
  
  /// Initialize
  void init()
  {
    m_name = "Product";
    m_errorEncoding = "1010101";
        
    for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
      
      SymbolEncoding encodingPairs;
      encodingPairs.insert("E", upc_common::LEFT_EVEN_ENCODE_TABLE[i]);
      encodingPairs.insert("0", upc_common::LEFT_ODD_ENCODE_TABLE[i]);
      encodingPairs.insert("R", upc_common::RIGHT_HAND_ENCODE_TABLE[i]);
      
      m_nodeSet.insert(
	SymbolNode(dynamic_cast<AbstractSymbology *>(this),
		      upc_common::SYMBOL_TABLE[i],
		      i,
		      encodingPairs,
		      shared::DEFAULT_SET,
		      QString("E")
		      )
		    );
    }        
  }
  
  
  const ProductSymbologyPrivate & operator=(const ProductSymbologyPrivate * other){
    m_name = other->m_name;
    m_nodeSet = other->m_nodeSet;
    m_expandedNodeList = other->m_expandedNodeList;
    
    return *this;    
  }

};


}

#endif // PRODUCT_SYMBOLS_PRIVATE
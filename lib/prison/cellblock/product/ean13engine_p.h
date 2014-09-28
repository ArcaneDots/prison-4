 
#ifndef EAN13ENGINE_PRIVATE
#define EAN13ENGINE_PRIVATE

#include "productengine_p.h"
#include "ean13engine.h"

namespace product{

  class Ean13EnginePrivate : public product::ProductEnginePrivate
  {
  public:
    Ean13EnginePrivate(){
      qDebug("Ean13EnginePrivate default constructor");  
      setProductCode(upc_common::PS__EAN_13);
      fillSystemEncodingList();
    }
    
    /** 
     * destructor 
     */
    virtual ~Ean13EnginePrivate()
    {  
      qDebug("Ean13EnginePrivate destructor");
    }
    
    QStringList m_parity13WidthEncoding;
    
      
    void fillSystemEncodingList()
    {
      qDebug("ProductEngine::Private fillWidthEncodingList() : start");
      for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
	m_parity13WidthEncoding.append(ean13::PARITY_13[i]);
      }
      qDebug("ProductEngine::Private fillWidthEncodingList() : end");
    }
    
    QString getFirstBlockEncodePattern(int indexedPattern) const
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
  };

};
#endif // EAN13ENGINE_PRIVATE



// /**
//  * @cond PRIVATE
//  */
// class Ean13Engine::Private
// {
// public:
//   Private();
//   virtual ~Private();
//   /**
//    * Get productCode specific encoding pattern for the first block of symbols
//    *
//    * @param indexedPattern index of assiocated pattern
//    */
//   QString getFirstBlockEncodePattern(int indexedPattern = 0) const;
// private:
//   /**
//    * Load all encoding patterns based on combo of system number (0-1) and check digit
//    */
//   void fillSystemEncodingList(); 
//   /**
//    * encoding patterns for EAN-13 first block
//    */
//   QStringList m_parity13WidthEncoding;
// };
// };
// /**
//  * @endcond
//  */
// 
// using namespace product;
// using namespace shared;
// // ----------------- Ean13Engine::Private -----------------------
// Ean13Engine::Private::Private() : 
//   m_parity13WidthEncoding()
// {
//   fillSystemEncodingList();
// }
// 
// Ean13Engine::Private::~Private()
// {
//   // empty
// }
// 
// void Ean13Engine::Private::fillSystemEncodingList()
// {
//   qDebug("ProductEngine::Private fillWidthEncodingList() : start");
//   for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
//     m_parity13WidthEncoding.append(ean13::PARITY_13[i]);
//   }
//   qDebug("ProductEngine::Private fillWidthEncodingList() : end");
// }
// 
// QString Ean13Engine::Private::getFirstBlockEncodePattern(int indexedPattern) const
// {
//   qDebug("Ean13Engine getFirstBlockEncodePattern() : EAN-13 pattern ");
//   if(indexedPattern >= 0 &&
//     indexedPattern <= m_parity13WidthEncoding.size()) {
//     return m_parity13WidthEncoding.at(indexedPattern);
//   } else {
//     qDebug("Ean13Engine getFirstBlockEncodePattern() : bad index ");
//     return QString("");
//   }
// }
// ----------------------- Ean13Engine -----------------------
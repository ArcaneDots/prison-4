 
#ifndef EAN13ENGINE_PRIVATE
#define EAN13ENGINE_PRIVATE

#include "productengine_p.h"
#include "ean13engine.h"

namespace product{
 
  class Ean13EnginePrivate : public ProductEnginePrivate
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
    /**
     * encoding patterns for EAN-13 first block
     */
    QStringList m_parity13WidthEncoding; 
  };

 

}
#endif // EAN13ENGINE_PRIVATE

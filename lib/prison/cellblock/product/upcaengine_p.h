 
#ifndef UPCAENGINE_PRIVATE
#define UPCAENGINE_PRIVATE

#include "productengine_p.h"
#include "upcaengine.h"

namespace product{
  
  class UpcAEnginePrivate : public ProductEnginePrivate
  {
  public:
    UpcAEnginePrivate(){
      qDebug("UpcAEnginePrivate default constructor");  
      setProductCode(upc_common::PS__UPC_A);
    }
    
    /** 
     * destructor 
     */
    virtual ~UpcAEnginePrivate()
    {  
      qDebug("UpcAEnginePrivate destructor");
    }
  };
  
  
};

#endif // UPCAENGINE_PRIVATE


#ifndef PRODUCTENGINE_PRIVATE
#define PRODUCTENGINE_PRIVATE

#include "../abstractbarcodeengine_p.h"
#include "productengine.h"

namespace product{

typedef QMap<shared::CODE_SECTIONS, QList<ProductSymbology> >CodeSections;

/**
 * @cond PRIVATE
 */
class ProductEnginePrivate : public linearBarcode::AbstractBarcodeEnginePrivate
{
public: 
  
  /**
   * Symbols 
   */
  CodeSections m_symbolSections;
  
  // ---------------
  // Barcode definition
  
  /**
   * default symbol used preform parsing 
   * 
   */
  ProductSymbology m_emptySymbol;
   /**
   * The product code number system digit is used by object
   * 
   * @note product code specific
   */
  upc_common::PRODUCT_CODE_VALUES m_productCode;

  /**
   * The first digit is treated as a "NumberSystem" and used for encoding
   */
  bool m_hasNumberSystem;
  /**
   * starting index of first text block
   *
   * @note All  = 1 but EAN-8 = 0
   */
  int m_fmtFirstBlockOffset;
  /**
   * text block size
   * 
   * @note format blockSize may/not be equal to m_encodeBlockSize    
   */
  int m_fmtBlockSize;
  /**
   * Has a second text block
   * 
   * @note UPC-E and EAN-8 do not contain a second block(s)
   */
  bool m_fmtHasSecondBlock;
  /**
   * starting index of first encoded block
   * 
   * @note All  = 1 but EAN-8 = 0
   */
  int m_encFirstBlockOffset;
  /**
  * encoded block size
  * 
  * @note format blockSize may/not be equal to m_encodeBlockSize    
  */
  int m_encBlockSize;
  /**
   * Has a second encoded block
   * 
   * @note UPC-E and EAN-8 do not contain a second block(s)
   */
  bool m_encHasSecondBlock; 
 
  QStringList m_parity2WidthEncoding;
  QStringList m_parity5WidthEncoding;
  // ---------------
  
   
  // user modifible - I plan on moving these in to Private
  
  /**
   * copy of original user input
   */
  QList<ProductSymbology> m_userParsedSymbols;
 
  /** 
   * machine readable/encoded barcode information
   */
  QString m_encodedSymbols;

  
 void setProductCode(upc_common::PRODUCT_CODE_VALUES productCode) 
 {
   m_productCode = productCode;
      
   if (m_productCode ==  upc_common::PS__EAN_13) {
      m_defaultString = ean13::DEFAULT_VALUE;
      m_minLength = ean13::MIN;
      m_maxLength = ean13::MAX_LEN;
      m_checkDigitOffset = ean13::CHECK_DIGIT_OFFSET;
      // [systemDigit][block1][block2]
      m_fmtBlockSize = ean13::BLOCK_SIZE;
      m_fmtFirstBlockOffset = 1;
      m_fmtHasSecondBlock = true;
      // [systemDigit][block1][block2]
      m_encBlockSize = ean13::ENCODE_BLOCK_SIZE;
      m_encFirstBlockOffset = 1;
      m_encHasSecondBlock = true;
  } else if (m_productCode == upc_common::PS__UPC_A) {
      m_defaultString = upcA::DEFAULT_VALUE;
      m_minLength = upcA::MIN;
      m_maxLength = upcA::MAX_LEN;
      m_checkDigitOffset = upcA::CHECK_DIGIT_OFFSET;
      // [systemDigit][block1][block2][checkDigit]
      m_fmtBlockSize = upcA::BLOCK_SIZE; 
      m_fmtFirstBlockOffset = 1;
      m_fmtHasSecondBlock = true;
      // [block1][block2]
      m_encBlockSize = upcA::ENCODE_BLOCK_SIZE;
      m_encFirstBlockOffset = 1;
      m_encHasSecondBlock = true;
  } else if (m_productCode == upc_common::PS__EAN_8) {
      m_defaultString = ean8::DEFAULT_VALUE;
      m_minLength = ean8::MIN;
      m_maxLength = ean8::MAX_LEN;
      m_checkDigitOffset = ean8::CHECK_DIGIT_OFFSET;
      // [block1][block2]
      m_fmtBlockSize = ean8::BLOCK_SIZE;
      m_fmtFirstBlockOffset = 0;
      m_fmtHasSecondBlock = true;
      // [block1][block2]
      m_encBlockSize = ean8::ENCODE_BLOCK_SIZE;
      m_encFirstBlockOffset = 1;
      m_encHasSecondBlock = true;
  }  else if (m_productCode == upc_common::PS__UPC_E) {
      m_defaultString = upcE::DEFAULT_VALUE,
      m_minLength = upcE::MIN,
      m_maxLength = upcE::MAX_LEN,
      m_checkDigitOffset = upcE::CHECK_DIGIT_OFFSET;
      // [systemDigit][block1][checkDigit]
      m_fmtBlockSize = upcE::BLOCK_SIZE;
      m_fmtFirstBlockOffset = 0;
      m_fmtHasSecondBlock = false;
      // [systemDigit] X [checkDigit]->[block1
      m_encBlockSize = upcE::ENCODE_BLOCK_SIZE;
      m_encFirstBlockOffset = 1;
      m_encHasSecondBlock = true;
  } 
 }
   
 upc_common::PRODUCT_CODE_VALUES getProductCode()
 {
   return m_productCode;
 }
 
  /** 
   * destructor 
   */
  virtual ~ProductEnginePrivate()
  {  
    qDebug("ProductEngine::Private destructor");
  }
  
  void initialize() {
    qDebug("ProductEngine initialize");    
    fillExtendedEncodingList();
  }
  
protected:
  
  /**
  * Exteneded barcode encoding patterns - EAN-2, EAN-5 
  */
  void fillExtendedEncodingList()
  {
    if (m_parity2WidthEncoding.isEmpty()) {
      m_parity2WidthEncoding.append(ean2::PARITY_2[0]);
      m_parity2WidthEncoding.append(ean2::PARITY_2[1]);
      m_parity2WidthEncoding.append(ean2::PARITY_2[2]);
      m_parity2WidthEncoding.append(ean2::PARITY_2[3]);

      for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
	m_parity5WidthEncoding.append(ean5::PARITY_5[i]);
      }
    }
  }
};

/**
 * @endcond
 */

};
#endif // PRODUCTENGINE_PRIVATE
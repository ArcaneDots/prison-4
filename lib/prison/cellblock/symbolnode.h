 
#ifndef SYMBOLS_NODE_H
#define SYMBOLS_NODE_H

#include <QMap>
#include <QStringList>

#include "abstractsymbology.h"

namespace linearSymbology {

  class AbstractSymbology;
  class AbstractSymbologyPrivate;

 
  class SymbolNode;
    
  typedef QMap<QString, QString> SymbolEncoding;
  //     
  typedef QList<SymbolNode *> SymbolNodePtrList;
  typedef QSet<SymbolNode *> SymbolNodePtrSet;
  
  typedef QList<SymbolNode> SymbolNodeList;
  typedef QSet<SymbolNode> SymbolNodeSet;
  
  class SymbolNodePrivate;
      
  
  /**
   * Underling individual shared Symbol data info object
   * 
   * @note create an extended node by setting the index == shared::NOT_FOUND
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
            	
      /**
       * Standard constructor
       *
       *  @note 1 or many symbol sets to 1 encoding
       *
       * code 39 or EAN-128
       */
      SymbolNode(const AbstractSymbology* symbology, 
		const QString& symbolString, 
		int index, 
		const QString defaultEncoding,
		const QString& symbolSetName = shared::DEFAULT_SET, 
		const QString& encodingSetName = shared::DEFAULT_SET, 
		bool isVisible = true);
     
      /**
       * Multiple encoding
       * 
       * Product codes (UPC/EAN-13/EAN-8)
       */
      SymbolNode(const AbstractSymbology* symbology, 
	  const QString& symbolString, 
	  int index, 
	  const SymbolEncoding& encodings, 
	  const QString& symbolSetName = shared::DEFAULT_SET, 
	  const QString& encodingSetName = shared::DEFAULT_SET, 
	  bool isVisible = true);
      
      /**
       * Expanded symbols
       * 
       * Create expanded symbol node (code 93)
       * 
       * @note build expanded node set before creating user visible nodes
       */
      SymbolNode(const AbstractSymbology* symbology, 
		const QString& symbolString, 
		//extended symbols don't have a usable index 
		const QString& symbolSet = shared::DEFAULT_SET,
		const QString& encodingSetName = shared::DEFAULT_SET, 
		bool isVisible = true);
      
      /**
       * Visible nodes encoded using a special set of expanded symbols
       * 
       * example: code 93
       * 
       * @note create expand after expanded nodes are built
       */
      SymbolNode(const AbstractSymbology* symbology, 
		const QString& symbolString, 
		int index, 
		const SymbolNodePtrList& expandedSymbolNodes, 
		const QString& symbolSet = shared::DEFAULT_SET,
		const QString& encodingSetName = shared::DEFAULT_SET,
		bool isVisible = true);
      
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
      
      bool isVisible() const;
      
      QString toString() const; 
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
      QString operator() () const;
	
      bool operator< (const SymbolNode &other) const;
      bool operator== (const SymbolNode &other) const;
      bool operator== (const QString &string) const;
      
      operator QString() const; 
      
    protected:   

      

   protected:
     const QScopedPointer<SymbolNodePrivate> d_ptr;
   private:
     Q_DECLARE_PRIVATE(SymbolNode);
  };  
  
 
bool operator<= (const SymbolNode& left, const SymbolNode& right);
bool operator>= (const SymbolNode& left, const SymbolNode& right);

}


#endif // SYMBOLS_NODE_H
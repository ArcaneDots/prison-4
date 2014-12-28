 
#include "symbolnode_p.h"
#include "abstractsymbology.h"
 
namespace linearSymbology{
 
SymbolNode::SymbolNode() :
  d_ptr(new SymbolNodePrivate())
{ /* Empty */ }
	
SymbolNode::SymbolNode(const SymbolNode& other) :
  d_ptr(
    new SymbolNodePrivate(
      static_cast<SymbolNodePrivate &>(
	*other.d_ptr.data())))
{ /* Empty */ }

SymbolNode::SymbolNode(const AbstractSymbology* symbology) : 
  d_ptr(new SymbolNodePrivate())
{
  Q_D(SymbolNode);
  d->m_symbology = symbology;
  d->m_string = shared::EmptyString;
  d->m_defaultEncoding << shared::EmptyString; 
  d->m_index = shared::NOT_FOUND; 
  d->m_symbolSetName = shared::EmptyString;
  d->m_isVisible = false;
  d->m_defaultEncoding << shared::EmptyString;
  //d->m_encodings;
  d->m_encodingSetName = shared::EmptyString;
  //d->m_expandedNodes;
}
      
SymbolNode::SymbolNode(const AbstractSymbology* symbology, 
		      const QString& symbolString, 
		      int index, 
		      QString defaultEncoding,
		      const QString& symbolSetName, 
		      const QString& encodingSetName, 
		      bool isVisible) :
  d_ptr(new SymbolNodePrivate())
  {
    Q_D(SymbolNode);
    d->m_symbology = symbology;
    d->m_string = symbolString;
    d->m_defaultEncoding << shared::EmptyString; 
    d->m_index = index; 
    d->m_symbolSetName = symbolSetName;
    d->m_isVisible = isVisible;
    d->m_defaultEncoding << defaultEncoding;
    //d->m_encodings;
    d->m_encodingSetName = encodingSetName;
    //d->m_expandedNodes;   
 }
      
SymbolNode::SymbolNode(const AbstractSymbology* symbology, 
		       const QString& symbolString, 
		       int index, 
		       const SymbolEncoding& encodings, 
		       const QString& symbolSetName, 
		       const QString& encodingSetName, 
		       bool isVisible) : 
    d_ptr(new SymbolNodePrivate())
{  
  Q_D(SymbolNode);
  d->m_symbology = symbology;
  d->m_string = symbolString;
  d->m_defaultEncoding << shared::EmptyString; 
  d->m_index = index; 
  d->m_symbolSetName = symbolSetName;
  d->m_isVisible = isVisible;
  d->m_defaultEncoding << shared::EmptyString;
  d->m_encodings = encodings;
  d->m_encodingSetName = encodingSetName;
  //d->m_expandedNodes;
}

SymbolNode::SymbolNode(const AbstractSymbology* symbology, 
		       const QString& symbolString, 
		       //extended symbols don't have a usable index 
		       const QString& symbolSetName, 
		       const QString& encodingSetName, 
		       bool isVisible) : 
	d_ptr(new SymbolNodePrivate())
	
{ 
  Q_D(SymbolNode);
  d->m_symbology = symbology;
  d->m_string = symbolString;
  d->m_defaultEncoding << shared::EmptyString; 
  d->m_index = shared::NOT_FOUND;
  d->m_symbolSetName = symbolSetName;
  d->m_isVisible = isVisible;
  //d->m_encodings ;
  d->m_encodingSetName = encodingSetName;
  //d->m_expandedNodes;
}

SymbolNode::SymbolNode(const AbstractSymbology* symbology, 
		       const QString& symbolString, 
		       int index, 
		       const SymbolNodePtrList& expandedSymbolNodes, 
		       const QString& symbolSet, 
		       const QString& encodingSetName,
		       bool isVisible) :
	d_ptr(new SymbolNodePrivate())
{
  Q_D(SymbolNode);
  d->m_symbology = symbology;
  d->m_string = symbolString;
  d->m_defaultEncoding << shared::EmptyString; 
  d->m_index = index; 
  d->m_symbolSetName = symbolSet;
  d->m_isVisible = isVisible;
  d->m_defaultEncoding << shared::EmptyString;
  //d->m_encodings;
  d->m_encodingSetName = encodingSetName;
  d->m_expandedNodes = expandedSymbolNodes;
          
  if (d->m_expandedNodes.isEmpty()) { return; }
  
  // m_defaultEncoding << expandedSymbolNodes(encodingSetName)
  d->m_defaultEncoding.clear();
  int count = d->m_expandedNodes.count();
  for (int i = 0; i > count; ++i) {
    d->m_defaultEncoding << d->m_expandedNodes.at(i)->encoding(d->m_encodingSetName);
   }
}

      
 const AbstractSymbology* SymbolNode::getSymbology() const
{
  Q_D(const SymbolNode);
  return d->m_symbology;
}
      
QString SymbolNode::symbologyName() const
{
  Q_D(const SymbolNode);
  return d->m_symbology == 0 ? shared::EmptyString : d->m_symbology->getName();
}

QString SymbolNode::namedSet() const
{  
  Q_D(const SymbolNode);
  return d->m_symbolSetName;
}
      
int SymbolNode::index() const 
{
  Q_D(const SymbolNode);
  return d->m_index;
}

QStringList SymbolNode::encoding() const
{
  Q_D(const SymbolNode);
  QStringList result;
  if (!d->m_defaultEncoding.isEmpty()) {
    result << d->m_defaultEncoding;
  } else if (!d->m_expandedNodes.isEmpty() ) {
    int extNodes = d->m_expandedNodes.count();
    for (int i = 0; i < extNodes; ++i) {
//       SymbolNode * nodePtr = d->m_expandedNodes.at(i);
//       result << nodePtr->encoding();
      result << static_cast<SymbolNode *>(d->m_expandedNodes.at(i))->encoding();
    }
  }  
  
  return result;
}

QStringList SymbolNode::encoding(QChar encodingSetName) const
{ 
  return encoding(QString(encodingSetName));
}
      
QStringList SymbolNode::encoding(const QString& encodingSetName) const
{
  Q_D(const SymbolNode);
  QStringList result;
  
  if (encodingSetName == d->m_encodingSetName) {
    result = d->m_defaultEncoding;
  }
  // single encoding
  else if (!d->m_encodings.isEmpty()) {
    result << d->m_encodings.key(encodingSetName);
  }
  // expanded nodes
  else if (!d->m_expandedNodes.isEmpty() ) {
    int extNodes = d->m_expandedNodes.count();
    for (int i = 0; i < extNodes; ++i) {
//       SymbolNode * nodePtr = d->m_expandedNodes.at(i);
//       result << nodePtr->encoding();
      result << 
	static_cast<SymbolNode *>(d->m_expandedNodes.at(i))->encoding(encodingSetName);
    }
  }
  
  return result;
}


bool SymbolNode::isValid() const
{  
  Q_D(const SymbolNode);
  return (!d->m_symbology) ? false : true;
}

bool SymbolNode::hasValue() const
{
  Q_D(const SymbolNode);
  return (d->m_index != shared::NOT_FOUND && !d->m_symbology) ? false : true;
}

bool SymbolNode::isVisible() const
{
  Q_D(const SymbolNode);
  return d->m_isVisible;
}


QString SymbolNode::toString() const
{
  Q_D(const SymbolNode);
  return d->m_string;
}

QString SymbolNode::operator()() const
{
  Q_D(const SymbolNode);
  return d->m_string;
}


bool SymbolNode::operator<(const SymbolNode& other) const
{
  Q_D(const SymbolNode);
  return d->m_index < other.index();
}

bool SymbolNode::operator==(const SymbolNode& other) const
{
  Q_D(const SymbolNode);
  return (d->m_index == other.index()) ;
}


bool SymbolNode::operator==(const QString& string) const
{
  Q_D(const SymbolNode);
  return (d->m_string == string);
}

SymbolNode::operator QString() const
{
  Q_D(const SymbolNode);
  return d->m_string;
}
      
bool operator<=(const SymbolNode& left, const SymbolNode& right)
{
  return left.index() <= right.index();
}

bool operator>=(const SymbolNode& left, const SymbolNode& right)
{
  return left.index() >= right.index();
}

}

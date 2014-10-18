 
#include "symbolnode_p.h"
#include "abstractsymbology.h"
 
 using namespace linearSymbology;
 
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
  d_ptr(new SymbolNodePrivate(symbology, 
			      shared::EmptyString, 
			      shared::NOT_FOUND, 
			      shared::EmptyString))
{ /* Empty */ }
      
SymbolNode::SymbolNode(const AbstractSymbology* symbology, 
		      const QString& symbolString, 
		      int index, 
		      QString defaultEncoding,
		      const QString& symbolSetName, 
		      const QString& encodingSetName, 
		      bool isVisible) :
  d_ptr(new SymbolNodePrivate(symbology,
			      symbolString,
			      index,
			      defaultEncoding,
			      symbolSetName,
			      encodingSetName,
			      isVisible))
  { /* Empty */ }
      
SymbolNode::SymbolNode(const AbstractSymbology* symbology, 
		       const QString& symbolString, 
		       int index, 
		       const SymbolEncoding& encodings, 
		       const QString& symbolSetName, 
		       const QString& encodingSetName, 
		       bool isVisible) : 
    d_ptr(new SymbolNodePrivate(symbology, 
			      symbolString, 
			      index, 
			      shared::EmptyString))
{ /* Empty */ }

      
 const AbstractSymbology* SymbolNode::getSymbology() const
{
  Q_D(const SymbolNode);
  return d->m_symbology;
}
      
QString SymbolNode::symbologyName() const
{
  Q_D(const SymbolNode);
  return !d->m_symbology ? shared::EmptyString : d->m_symbology->getName();
  //return m_symbology.isNull() ? EmptyString : m_symbology->getName();
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
  return d->m_defaultEncoding;
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
  } else {
    
    // single encoding
    if (!d->m_encodings.contains(encodingSetName))  {
      result << d->m_encodings.key(encodingSetName);
    }
    // expanded nodes
    if (!d->m_expandedNodes.isEmpty() && 
      !d->m_expandedNodes.constBegin()->encoding(encodingSetName).empty()) {
      int expandNodes = d->m_expandedNodes.count();
      for (int i = 0; i < expandNodes; ++i) {
	result << d->m_expandedNodes.at(i).encoding(encodingSetName);
      }
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
   
   
// SymbolNode::SymbolNode(const AbstractSymbology& symbology,
// 		       int index, 
// 		       const QString& visibleString, 
// 		       const QString& encoding, 
// 		       const QString& namedSymbolSet, 
// 		       bool isVisible) : 
//   d_ptr(symbology, 
// 	visibleString, 
// 	encoding, 
// 	namedSymbolSet, 
// 	isVisible)
// {
//   Q_D(SymbolNode);
//   d->m_symbology(symbology);
//   d->m_index(index);
//   d->m_string(shared::EmptyString); 
//   d->m_symbolSetName(namedSymbolSet);
//   d->m_isVisible(isVisible);
//   d->m_expandedNodes(0);
//   d->m_encoding(0);
//   d->m_defaultEncoding();
//   
//   if (!d->m_symbology) { return; }
//   
//   d->m_defaultEncoding << shared::EmptyString;
// // 	m_expandedNodes = m_symbology.constData()->expandedSymbols();
// // 	m_encoding = m_symbology.constData()->encodings();
//   
//   SymbolEncoding::const_iterator itDefEncoding = d->m_encoding->data()->constEnd();
//   SymbolEncoding::const_iterator itEncodingEnd = d->m_encoding->data()->constEnd();
// // 	itDefEncoding = m_encoding->data()->find(m_symbology->defaultEncoding());	
//   if (itDefEncoding != itEncodingEnd) {
//     d->m_defaultEncoding << itDefEncoding.value();
//   }	  
//}  
      
      
bool linearSymbology::operator<=(const SymbolNode& left, const SymbolNode& right)
{
  return left.index() <= right.index();
}

bool linearSymbology::operator>=(const SymbolNode& left, const SymbolNode& right)
{
  return left.index() >= right.index();
}

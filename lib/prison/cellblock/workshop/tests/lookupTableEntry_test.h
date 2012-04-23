
#ifndef LOOKUP_TABLE_ENTRY_TEST
#define LOOKUP_TABLE_ENTRY_TEST


#include <QtTest/QtTest>
#include <QtCore/QStringList>
#include "../lookuptableentry.h"
  
class Test_LookupTableEntry: public QObject
{
  Q_OBJECT
  
private:
  QStringList numbers;
  QStringList l_case;
  QStringList u_case;
  
public:
  Test_LookupTableEntry();

private slots:
  void constructorD();
  void constructorDValue();
  void constructorCopy();
};
#endif


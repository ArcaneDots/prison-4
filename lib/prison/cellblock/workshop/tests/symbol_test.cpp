 

#include "../symbol.h"
#include "symbol_test.h"
 
void Test_Symbol::constructorD()
{
  Symbol s;
  QVERIFY(!s.hasValue());
  QVERIFY(!s.isValid());
}

void Test_Symbol::constructorCopy()
{
 
}

void Test_Symbol::constructorCopy_data()
{
 
}
 QTEST_MAIN(Test_Symbol)
 #include "symbol_test.moc"
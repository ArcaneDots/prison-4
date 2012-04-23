 
#ifndef SYMBOL_TEST
#define SYMBOL_TEST 
#include <QtTest/QTest>

class Test_Symbol: public QObject
{
  Q_OBJECT

private slots:
  void constructorD();
  void constructorCopy();
  void constructorCopy_data();
};

#endif // SYMBOL_TEST
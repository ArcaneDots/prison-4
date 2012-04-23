 
#ifndef SYMBOL_DATUM_TEST
#define SYMBOL_DATUM_TEST 
 
#include <QtTest/QTest>
  class Test_SymbolDatum: public QObject
 {
     Q_OBJECT
 private:
  QStringList numbers;
  QStringList l_case;
  QStringList u_case;

 public:
   Test_SymbolDatum();
 private slots:
   void constructorD();
   void constructorCopy();
   void constructorEncoded();
   void constructorExpanded();
 };

#endif //SYMBOL_DATUM_TEST
 
#ifndef BARCODE_SYMBOL_SET_TEST
#define BARCODE_SYMBOL_SET_TEST 
 
#include <QtTest/QTest>
#include <QtCore/QStringList>

class Test_BarcodeSymbolSet: public QObject
{
 Q_OBJECT
public:
  Test_BarcodeSymbolSet();
  ~Test_BarcodeSymbolSet();
private:
 QList<QString> digits;
 QList<QString> Odd;
 QList<QString> Even;
 QList<QString> Right;
private slots:
 void constructorD();
 void constructorCopy();
};

#endif // BARCODE_SYMBOL_SET_TEST
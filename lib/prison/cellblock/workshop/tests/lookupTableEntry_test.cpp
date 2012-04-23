
#include "lookupTableEntry_test.h"

Test_LookupTableEntry::Test_LookupTableEntry() : QObject()
{
  numbers = QString("1 2 3 4 5 6 7 8 9 10 11").split(" ");
  l_case = QString("a b c d e f g h i j k l").split(" ");
  u_case = QString("A B C D E F G H I J K").split(" ");
}

void Test_LookupTableEntry::constructorD()
{
  LookupTableEntry<QString> lookup_l;
  QVERIFY(lookup_l.isEmpty());
  lookup_l.addRow(numbers);
  QVERIFY(numbers.toStdList().size() == 11);
  QCOMPARE(lookup_l.getRow().toStdList(), numbers.toStdList());
  QVERIFY(!lookup_l.isEmpty());
  qDebug() << "contents: " << lookup_l;
}

void Test_LookupTableEntry::constructorCopy()
{   
  LookupTableEntry<QString> lookup_1;
  lookup_1.addRow("numbers", numbers);
  lookup_1.addRow("u_case", u_case);
  QVERIFY(!lookup_1.isEmpty());
  LookupTableEntry<QString> lookup_2(lookup_1);  
  QVERIFY(lookup_1.size() == lookup_2.size());  
  QVERIFY(lookup_2 == lookup_1);  
}

void Test_LookupTableEntry::constructorDValue()
{
  LookupTableEntry<QString> lookup_l(l_case);
  QCOMPARE(lookup_l.getRow().toStdList(), l_case.toStdList());
  QVERIFY(!lookup_l.isEmpty());
  QCOMPARE(lookup_l.removeRow().toStdList(), l_case.toStdList());
  QVERIFY(lookup_l.isEmpty());
  lookup_l.addRow("numbers", numbers);
  lookup_l.addRow("u_case", u_case);
  QVERIFY(lookup_l.size() == 2);
}


QTEST_MAIN(Test_LookupTableEntry)
#include "lookupTableEntry_test.moc"
/****************************************************************************
** Meta object code from reading C++ file 'lookupTableEntry_test.h'
**
** Created: Sun Apr 15 18:51:13 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../lib/prison/cellblock/workshop/tests/lookupTableEntry_test.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lookupTableEntry_test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tests__Test_LookupTableEntry[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   29,   29,   29, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_tests__Test_LookupTableEntry[] = {
    "tests::Test_LookupTableEntry\0\0"
    "constructorCopy()\0"
};

void tests::Test_LookupTableEntry::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Test_LookupTableEntry *_t = static_cast<Test_LookupTableEntry *>(_o);
        switch (_id) {
        case 0: _t->constructorCopy(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData tests::Test_LookupTableEntry::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject tests::Test_LookupTableEntry::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_tests__Test_LookupTableEntry,
      qt_meta_data_tests__Test_LookupTableEntry, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &tests::Test_LookupTableEntry::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *tests::Test_LookupTableEntry::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *tests::Test_LookupTableEntry::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tests__Test_LookupTableEntry))
        return static_cast<void*>(const_cast< Test_LookupTableEntry*>(this));
    return QObject::qt_metacast(_clname);
}

int tests::Test_LookupTableEntry::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

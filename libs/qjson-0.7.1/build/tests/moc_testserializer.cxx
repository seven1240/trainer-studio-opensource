/****************************************************************************
** Meta object code from reading C++ file 'testserializer.cpp'
**
** Created: Wed Sep 15 12:16:59 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'testserializer.cpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TestSerializer[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      45,   15,   15,   15, 0x08,
      61,   15,   15,   15, 0x08,
      82,   15,   15,   15, 0x08,
      98,   15,   15,   15, 0x08,
     116,   15,   15,   15, 0x08,
     139,   15,   15,   15, 0x08,
     158,   15,   15,   15, 0x08,
     182,   15,   15,   15, 0x08,
     200,   15,   15,   15, 0x08,
     223,   15,   15,   15, 0x08,
     242,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TestSerializer[] = {
    "TestSerializer\0\0testReadWriteEmptyDocument()\0"
    "testReadWrite()\0testReadWrite_data()\0"
    "testValueNull()\0testValueString()\0"
    "testValueString_data()\0testValueInteger()\0"
    "testValueInteger_data()\0testValueDouble()\0"
    "testValueDouble_data()\0testValueBoolean()\0"
    "testValueBoolean_data()\0"
};

const QMetaObject TestSerializer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestSerializer,
      qt_meta_data_TestSerializer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TestSerializer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TestSerializer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TestSerializer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TestSerializer))
        return static_cast<void*>(const_cast< TestSerializer*>(this));
    return QObject::qt_metacast(_clname);
}

int TestSerializer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: testReadWriteEmptyDocument(); break;
        case 1: testReadWrite(); break;
        case 2: testReadWrite_data(); break;
        case 3: testValueNull(); break;
        case 4: testValueString(); break;
        case 5: testValueString_data(); break;
        case 6: testValueInteger(); break;
        case 7: testValueInteger_data(); break;
        case 8: testValueDouble(); break;
        case 9: testValueDouble_data(); break;
        case 10: testValueBoolean(); break;
        case 11: testValueBoolean_data(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'testparser.cpp'
**
** Created: Wed Sep 15 12:16:56 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'testparser.cpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TestParser[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      34,   11,   11,   11, 0x08,
      54,   11,   11,   11, 0x08,
      73,   11,   11,   11, 0x08,
      91,   11,   11,   11, 0x08,
     102,   11,   11,   11, 0x08,
     124,   11,   11,   11, 0x08,
     143,   11,   11,   11, 0x08,
     164,   11,   11,   11, 0x08,
     185,   11,   11,   11, 0x08,
     211,   11,   11,   11, 0x08,
     229,   11,   11,   11, 0x08,
     243,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TestParser[] = {
    "TestParser\0\0parseNonAsciiString()\0"
    "parseSimpleObject()\0parseEmptyObject()\0"
    "parseEmptyValue()\0parseUrl()\0"
    "parseMultipleObject()\0parseSimpleArray()\0"
    "parseInvalidObject()\0parseMultipleArray()\0"
    "testTrueFalseNullValues()\0testEscapeChars()\0"
    "testNumbers()\0testNumbers_data()\0"
};

const QMetaObject TestParser::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestParser,
      qt_meta_data_TestParser, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TestParser::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TestParser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TestParser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TestParser))
        return static_cast<void*>(const_cast< TestParser*>(this));
    return QObject::qt_metacast(_clname);
}

int TestParser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: parseNonAsciiString(); break;
        case 1: parseSimpleObject(); break;
        case 2: parseEmptyObject(); break;
        case 3: parseEmptyValue(); break;
        case 4: parseUrl(); break;
        case 5: parseMultipleObject(); break;
        case 6: parseSimpleArray(); break;
        case 7: parseInvalidObject(); break;
        case 8: parseMultipleArray(); break;
        case 9: testTrueFalseNullValues(); break;
        case 10: testEscapeChars(); break;
        case 11: testNumbers(); break;
        case 12: testNumbers_data(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

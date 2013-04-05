/****************************************************************************
** Meta object code from reading C++ file 'color_dialog.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "color_dialog.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'color_dialog.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Color_Dialog_t {
    QByteArrayData data[11];
    char stringdata[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Color_Dialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Color_Dialog_t qt_meta_stringdata_Color_Dialog = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 12),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 8),
QT_MOC_LITERAL(4, 36, 1),
QT_MOC_LITERAL(5, 38, 14),
QT_MOC_LITERAL(6, 53, 7),
QT_MOC_LITERAL(7, 61, 8),
QT_MOC_LITERAL(8, 70, 27),
QT_MOC_LITERAL(9, 98, 22),
QT_MOC_LITERAL(10, 121, 4)
    },
    "Color_Dialog\0colorChanged\0\0setColor\0"
    "c\0update_widgets\0set_hsl\0set_rgba\0"
    "on_edit_hex_editingFinished\0"
    "on_edit_hex_textEdited\0arg1\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Color_Dialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    1,   52,    2, 0x0a,
       5,    0,   55,    2, 0x09,
       6,    0,   56,    2, 0x09,
       7,    0,   57,    2, 0x09,
       8,    0,   58,    2, 0x08,
       9,    1,   59,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::QColor,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void Color_Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Color_Dialog *_t = static_cast<Color_Dialog *>(_o);
        switch (_id) {
        case 0: _t->colorChanged((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 1: _t->setColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->update_widgets(); break;
        case 3: _t->set_hsl(); break;
        case 4: _t->set_rgba(); break;
        case 5: _t->on_edit_hex_editingFinished(); break;
        case 6: _t->on_edit_hex_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Color_Dialog::*_t)(QColor );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Color_Dialog::colorChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Color_Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Color_Dialog.data,
      qt_meta_data_Color_Dialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *Color_Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Color_Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Color_Dialog.stringdata))
        return static_cast<void*>(const_cast< Color_Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Color_Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Color_Dialog::colorChanged(QColor _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'dockglyph.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../dockglyph.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dockglyph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN9DockGlyphE_t {};
} // unnamed namespace

template <> constexpr inline auto DockGlyph::qt_create_metaobjectdata<qt_meta_tag_ZN9DockGlyphE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DockGlyph",
        "glyphChanged",
        "",
        "QSharedPointer<GlyphMeta>",
        "glyph",
        "bitmapDimensionChanged",
        "newSize",
        "gridDimensionChanged",
        "newValue",
        "glyphSizeChanged",
        "fontChanged",
        "newFont",
        "newFontPath",
        "characterChanged",
        "newChar",
        "moveGlyphLeft",
        "moveGlyphRight",
        "moveGlyphTop",
        "moveGlyphDown",
        "moveGlyphCenter",
        "slotFontChanged",
        "font",
        "slotCharacterChanged",
        "slotBitmapDimensionChanged",
        "slotGridDimensionChanged",
        "slotGlyphSizeChanged",
        "slotMoveCenterClicked",
        "slotMoveLeftClicked",
        "slotMoveTopClicked",
        "slotMoveDownClicked",
        "slotMoveRightClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'glyphChanged'
        QtMocHelpers::SignalData<void(QSharedPointer<GlyphMeta>)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'bitmapDimensionChanged'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'gridDimensionChanged'
        QtMocHelpers::SignalData<void(int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Signal 'glyphSizeChanged'
        QtMocHelpers::SignalData<void(int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'fontChanged'
        QtMocHelpers::SignalData<void(const QFont &, const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QFont, 11 }, { QMetaType::QString, 12 },
        }}),
        // Signal 'characterChanged'
        QtMocHelpers::SignalData<void(const QChar &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QChar, 14 },
        }}),
        // Signal 'moveGlyphLeft'
        QtMocHelpers::SignalData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'moveGlyphRight'
        QtMocHelpers::SignalData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'moveGlyphTop'
        QtMocHelpers::SignalData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'moveGlyphDown'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'moveGlyphCenter'
        QtMocHelpers::SignalData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotFontChanged'
        QtMocHelpers::SlotData<void(const QFont &)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QFont, 21 },
        }}),
        // Slot 'slotCharacterChanged'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotBitmapDimensionChanged'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Slot 'slotGridDimensionChanged'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Slot 'slotGlyphSizeChanged'
        QtMocHelpers::SlotData<void(int)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Slot 'slotMoveCenterClicked'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotMoveLeftClicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotMoveTopClicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotMoveDownClicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotMoveRightClicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DockGlyph, qt_meta_tag_ZN9DockGlyphE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DockGlyph::staticMetaObject = { {
    QMetaObject::SuperData::link<QDockWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9DockGlyphE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9DockGlyphE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9DockGlyphE_t>.metaTypes,
    nullptr
} };

void DockGlyph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DockGlyph *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->glyphChanged((*reinterpret_cast< std::add_pointer_t<QSharedPointer<GlyphMeta>>>(_a[1]))); break;
        case 1: _t->bitmapDimensionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->gridDimensionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->glyphSizeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->fontChanged((*reinterpret_cast< std::add_pointer_t<QFont>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->characterChanged((*reinterpret_cast< std::add_pointer_t<QChar>>(_a[1]))); break;
        case 6: _t->moveGlyphLeft(); break;
        case 7: _t->moveGlyphRight(); break;
        case 8: _t->moveGlyphTop(); break;
        case 9: _t->moveGlyphDown(); break;
        case 10: _t->moveGlyphCenter(); break;
        case 11: _t->slotFontChanged((*reinterpret_cast< std::add_pointer_t<QFont>>(_a[1]))); break;
        case 12: _t->slotCharacterChanged(); break;
        case 13: _t->slotBitmapDimensionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->slotGridDimensionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->slotGlyphSizeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->slotMoveCenterClicked(); break;
        case 17: _t->slotMoveLeftClicked(); break;
        case 18: _t->slotMoveTopClicked(); break;
        case 19: _t->slotMoveDownClicked(); break;
        case 20: _t->slotMoveRightClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)(QSharedPointer<GlyphMeta> )>(_a, &DockGlyph::glyphChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)(int )>(_a, &DockGlyph::bitmapDimensionChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)(int )>(_a, &DockGlyph::gridDimensionChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)(int )>(_a, &DockGlyph::glyphSizeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)(const QFont & , const QString & )>(_a, &DockGlyph::fontChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)(const QChar & )>(_a, &DockGlyph::characterChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)()>(_a, &DockGlyph::moveGlyphLeft, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)()>(_a, &DockGlyph::moveGlyphRight, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)()>(_a, &DockGlyph::moveGlyphTop, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)()>(_a, &DockGlyph::moveGlyphDown, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (DockGlyph::*)()>(_a, &DockGlyph::moveGlyphCenter, 10))
            return;
    }
}

const QMetaObject *DockGlyph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DockGlyph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9DockGlyphE_t>.strings))
        return static_cast<void*>(this);
    return QDockWidget::qt_metacast(_clname);
}

int DockGlyph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void DockGlyph::glyphChanged(QSharedPointer<GlyphMeta> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void DockGlyph::bitmapDimensionChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void DockGlyph::gridDimensionChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void DockGlyph::glyphSizeChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void DockGlyph::fontChanged(const QFont & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void DockGlyph::characterChanged(const QChar & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void DockGlyph::moveGlyphLeft()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void DockGlyph::moveGlyphRight()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void DockGlyph::moveGlyphTop()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void DockGlyph::moveGlyphDown()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void DockGlyph::moveGlyphCenter()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP

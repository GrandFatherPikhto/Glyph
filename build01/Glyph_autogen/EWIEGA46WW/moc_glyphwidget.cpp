/****************************************************************************
** Meta object code from reading C++ file 'glyphwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../glyphwidget.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glyphwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11GlyphWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto GlyphWidget::qt_create_metaobjectdata<qt_meta_tag_ZN11GlyphWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GlyphWidget",
        "setGlyphMeta",
        "",
        "QSharedPointer<GlyphMeta>",
        "newGlyph",
        "enableTemplateLayer",
        "editable",
        "enableGrid",
        "enable",
        "enablePreviewLayer",
        "enableUserLayer",
        "enableGlyphRectLayer",
        "enableBitmapRectLayer",
        "enableBaselineLayer"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'setGlyphMeta'
        QtMocHelpers::SlotData<void(QSharedPointer<GlyphMeta>)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'enableTemplateLayer'
        QtMocHelpers::SlotData<void(bool)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 6 },
        }}),
        // Slot 'enableGrid'
        QtMocHelpers::SlotData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Slot 'enablePreviewLayer'
        QtMocHelpers::SlotData<void(bool)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Slot 'enableUserLayer'
        QtMocHelpers::SlotData<void(bool)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Slot 'enableGlyphRectLayer'
        QtMocHelpers::SlotData<void(bool)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Slot 'enableBitmapRectLayer'
        QtMocHelpers::SlotData<void(bool)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Slot 'enableBaselineLayer'
        QtMocHelpers::SlotData<void(bool)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GlyphWidget, qt_meta_tag_ZN11GlyphWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GlyphWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11GlyphWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11GlyphWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11GlyphWidgetE_t>.metaTypes,
    nullptr
} };

void GlyphWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GlyphWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->setGlyphMeta((*reinterpret_cast< std::add_pointer_t<QSharedPointer<GlyphMeta>>>(_a[1]))); break;
        case 1: _t->enableTemplateLayer((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->enableGrid((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->enablePreviewLayer((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->enableUserLayer((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->enableGlyphRectLayer((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->enableBitmapRectLayer((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->enableBaselineLayer((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *GlyphWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GlyphWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11GlyphWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GlyphWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP

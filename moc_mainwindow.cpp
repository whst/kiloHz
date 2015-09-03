/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Fri Apr 24 09:03:22 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      23,   11,   11,   11, 0x08,
      49,   31,   11,   11, 0x08,
      91,   31,   11,   11, 0x08,
     137,   11,   11,   11, 0x08,
     162,  157,   11,   11, 0x08,
     182,  175,   11,   11, 0x08,
     217,   11,   11,   11, 0x08,
     244,  233,   11,   11, 0x08,
     266,   11,   11,   11, 0x08,
     282,   11,   11,   11, 0x08,
     298,   11,   11,   11, 0x08,
     317,   11,   11,   11, 0x08,
     326,   11,   11,   11, 0x08,
     337,   11,   11,   11, 0x08,
     356,   11,   11,   11, 0x08,
     376,   11,   11,   11, 0x08,
     387,   11,   11,   11, 0x08,
     409,  405,   11,   11, 0x08,
     430,   11,   11,   11, 0x08,
     450,  443,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0addFiles()\0about()\0"
    "newState,oldState\0"
    "stateChanged(Phonon::State,Phonon::State)\0"
    "metaStateChanged(Phonon::State,Phonon::State)\0"
    "changePausedState()\0time\0tick(qint64)\0"
    "source\0sourceChanged(Phonon::MediaSource)\0"
    "aboutToFinish()\0row,column\0"
    "tableClicked(int,int)\0goToNextMusic()\0"
    "goToPrevMusic()\0goToCurrentMusic()\0"
    "turnUp()\0turnDown()\0selectionChanged()\0"
    "deleteCurrentItem()\0clearAll()\0"
    "setShowMsgState()\0str\0searchMusic(QString)\0"
    "searchNext()\0reason\0"
    "showWin(QSystemTrayIcon::ActivationReason)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->addFiles(); break;
        case 1: _t->about(); break;
        case 2: _t->stateChanged((*reinterpret_cast< Phonon::State(*)>(_a[1])),(*reinterpret_cast< Phonon::State(*)>(_a[2]))); break;
        case 3: _t->metaStateChanged((*reinterpret_cast< Phonon::State(*)>(_a[1])),(*reinterpret_cast< Phonon::State(*)>(_a[2]))); break;
        case 4: _t->changePausedState(); break;
        case 5: _t->tick((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 6: _t->sourceChanged((*reinterpret_cast< const Phonon::MediaSource(*)>(_a[1]))); break;
        case 7: _t->aboutToFinish(); break;
        case 8: _t->tableClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->goToNextMusic(); break;
        case 10: _t->goToPrevMusic(); break;
        case 11: _t->goToCurrentMusic(); break;
        case 12: _t->turnUp(); break;
        case 13: _t->turnDown(); break;
        case 14: _t->selectionChanged(); break;
        case 15: _t->deleteCurrentItem(); break;
        case 16: _t->clearAll(); break;
        case 17: _t->setShowMsgState(); break;
        case 18: _t->searchMusic((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: _t->searchNext(); break;
        case 20: _t->showWin((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

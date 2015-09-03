#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//debug
//#include <iostream>
//#include <string>
//debug
#include <QMainWindow>
#include <QtGlobal>
#include <QLineEdit>
#include <QSystemTrayIcon>
#include <QButtonGroup>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QFile>
#include <QList>
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>
////nce
//#include <phonon/effect.h>
//#include <phonon/effectparameter.h>
//#include <phonon/effectwidget.h>
//#include <phonon/videowidget.h>
//#include <QtGui/QWidget>
//#include <QtGui/QApplication>
//#include <QtCore/QTimerEvent>
//#include <QtGui/QShowEvent>
//#include <QtGui/QIcon>
//#include <QtCore/QBasicTimer>
//#include <QtGui/QAction>
////nce

#define LISTFILE "kiloMusicList.txt"
#define UNSELECT "<Î´Ñ¡Ôñ>"
#define DEFAULTTITLE "Ç§ºÕ×È(KiloHz)²¥·ÅÆ÷"

QT_BEGIN_NAMESPACE
class QAction;
class QTableWidget;
class QLCDNumber;
QT_END_NAMESPACE
class MediaPlayer;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    QSize sizeHint() const {
        return QSize(417,370);
    }

private slots:
    void addFiles();
    void about();
    void stateChanged(Phonon::State newState, Phonon::State oldState);
    void metaStateChanged(Phonon::State newState, Phonon::State oldState);
    void changePausedState(void);
    void tick(qint64 time);
    void sourceChanged(const Phonon::MediaSource &source);
    void aboutToFinish();
    void tableClicked(int row,int column);
    void goToNextMusic();
    void goToPrevMusic();
    void goToCurrentMusic();
    void turnUp();
    void turnDown();
    void selectionChanged();
    void deleteCurrentItem();
    void clearAll();
    void setShowMsgState();
    void searchMusic(QString str);
    void searchNext();
    void showWin(QSystemTrayIcon::ActivationReason reason);

private:
    void setupActions();
    void setupMenus();
    void setupUi();
    void setupTrayIcon();
    void loadList();
    void addItemToTable(QString fileName);
    void changeMusic(bool next);
    void showMusicMsg();
    void turnItemUp(bool up);
    void writeList();
    int searchTable(int begin,int end,QString str);

    Phonon::SeekSlider *seekSlider;
    Phonon::MediaObject *mediaObject;
    Phonon::MediaObject *metaInfoResolver;
    Phonon::AudioOutput *audioOutput;
    Phonon::VolumeSlider *volumeSlider;
    QList<Phonon::MediaSource> sources;

    QAction *pauseAction;
    QAction *stopAction;
    QAction *nextAction;
    QAction *prevAction;
    QAction *addFilesAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *deleteFileAction;
    QAction *clearAllAction;
    QAction *goUpAction;
    QAction *goDownAction;
    QAction *showWindowAction;
    QAction *setShowMsgAction;
    QLabel *musicInfo;
    QLCDNumber *timeLcd;
    QTableWidget *musicTable;
    QAction *goToCurrentAction;
    QComboBox *plyModeBox;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QLineEdit *searchLine;
    QPushButton *searchNextButton;
    bool showMsg;
};

#endif

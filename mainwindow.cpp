#include <QtGui>

#include "mainwindow.h"
#include "others.h"

MainWindow::MainWindow()
{
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    metaInfoResolver = new Phonon::MediaObject(this);

    mediaObject->setTickInterval(200);
    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
            this, SLOT(sourceChanged(Phonon::MediaSource)));
    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
    connect(metaInfoResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));

    Phonon::createPath(mediaObject, audioOutput);

    setupActions();
    setupMenus();
    setupUi();
    setupTrayIcon();
    loadList();
    showMsg = true;
    timeLcd->display("00:00");
    trayIcon->show();
}

void MainWindow::setShowMsgState()
{
    showMsg = !showMsg;
    if(showMsg){
        setShowMsgAction->setText(tr("关闭气泡提示(&M)"));
    }else{
        setShowMsgAction->setText(tr("显示气泡提示(&M)"));
    }
}

void MainWindow::searchMusic(QString str)
{
    if (str.isEmpty())
        return;
    int index = searchTable(0,sources.size(),str);
    if(index != -1){
        musicTable->selectRow(index);
    }
}

void MainWindow::searchNext()
{
    QString str = searchLine->text().toLower();
    if (str.isEmpty())
        return;
    int index = searchTable(musicTable->currentRow()+1,sources.size(),str);
    if (index == -1){
        index=searchTable(0,musicTable->currentRow(),str);
    }
    if( index != -1){
        musicTable->selectRow(index);
    }
}

void MainWindow::showWin(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        qApp->quit();
        break;
    default:
        /*NULL*/;
    }
}

void MainWindow::writeList()
{
    QFile list(LISTFILE);
    if (!list.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&list);
    for(int i=0; i < sources.size(); ++i)
    {
        out << sources.at(i).fileName() <<'\n';
    }

    list.close();
}

int MainWindow::searchTable(int begin,int end, QString str)
{
    for(/*Null*/; begin < end; ++begin){
        if(musicTable->item(begin, 0)->text().contains(str)){
            return begin;
        }
    }
    return -1;
}

void MainWindow::loadList()
{
    deleteFileAction->setDisabled(true);
    clearAllAction->setDisabled(true);
    goUpAction->setDisabled(true);
    goDownAction->setDisabled(true);
    QFile list(LISTFILE);
    if(!QFile::exists(LISTFILE))
        QMessageBox::information(this, tr("找不到列表文件"),
                                 tr("无法在当前文件夹下找到 ")+tr(LISTFILE)+tr("\n如果您添加文件, 则会创建一个新的列表"));
    if (!list.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&list);
     while (!in.atEnd()) {
         QString line = in.readLine();
         Phonon::MediaSource source(line);
         sources.append(source);
         addItemToTable(line);
     }
     list.close();
     if(!sources.isEmpty()){
         deleteFileAction->setEnabled(true);
         clearAllAction->setEnabled(true);
     }
}

void MainWindow::addFiles()
{

    QStringList files = QFileDialog::getOpenFileNames(this, tr("打开"),
                                                      QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    if (files.isEmpty())
        return;

    QFile list(LISTFILE);
    if (!list.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&list);
    foreach (QString string, files) {
        Phonon::MediaSource source(string);
        sources.append(source);
        addItemToTable(string);
        out << string <<'\n';
    }
    list.close();
}

void MainWindow::about()
{
    QMessageBox aboutBox;
    aboutBox.setWindowTitle(tr("关于 千赫兹-KiloHz"));
    /**********************************************************/
    aboutBox.setText(tr(ABOUTINFO));
//    QFile aboutFile("files/about.txt");
//    QString aboutText = aboutFile.readAll().constData();
//    aboutBox.setText(aboutText);
    aboutBox.setIconPixmap(QPixmap(":/files/logo.svg"));
    aboutBox.exec();
}

void MainWindow::stateChanged(Phonon::State newState, Phonon::State /*oldState*/)
{
    bool existItem = ! sources.isEmpty();
    switch (newState) {
    case Phonon::ErrorState:
        timeLcd->display("00:00");
        goToCurrentAction->setText(tr(UNSELECT));
        goToCurrentAction->setDisabled(true);
        setWindowTitle(tr(DEFAULTTITLE));
        musicInfo->setText(tr(""));
        stopAction->setDisabled(true);
        if (mediaObject->errorType() == Phonon::FatalError) {
            QMessageBox::warning(this, tr("致命错误"),
                                 mediaObject->errorString());
        } else {
            QMessageBox::warning(this, tr("错误"),
                                 mediaObject->errorString());
        }
        break;

    case Phonon::PlayingState:
        pauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        pauseAction->setText(tr("暂停(&P)"));
        pauseAction->setEnabled(true);
        nextAction->setEnabled(true);
        prevAction->setEnabled(true);
        stopAction->setEnabled(true);
        goToCurrentAction->setEnabled(true);
        metaInfoResolver->setCurrentSource(mediaObject->currentSource());//
        goToCurrentAction->setText(musicTable->item(sources.indexOf(mediaObject->currentSource()), 0)->text());
        setWindowTitle(goToCurrentAction->text()+tr(" - KiloHz Player"));//
        showMusicMsg();
        break;
    case Phonon::StoppedState:
        stopAction->setEnabled(false);
        pauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        pauseAction->setText(tr("播放(&P)"));
        pauseAction->setEnabled(existItem);
        nextAction->setEnabled(existItem);
        prevAction->setEnabled(existItem);
        timeLcd->display("00:00");
        goToCurrentAction->setText(tr(UNSELECT));
        goToCurrentAction->setDisabled(true);
        setWindowTitle(tr(DEFAULTTITLE));
        musicInfo->setText(tr(""));
        break;
    case Phonon::PausedState:
        pauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        pauseAction->setText(tr("播放(&P)"));
        pauseAction->setEnabled(true);
        nextAction->setEnabled(true);
        prevAction->setEnabled(true);
        stopAction->setEnabled(true);
        break;
    case Phonon::BufferingState:
        break;
    default:
        ;
    }
}

void MainWindow::metaStateChanged(Phonon::State /*newState*/, Phonon::State /*oldState*/)
{
    QMap<QString, QString> metaData = metaInfoResolver->metaData();
    goToCurrentAction->setText(musicTable->item(sources.indexOf(mediaObject->currentSource()), 0)->text());
    setWindowTitle(goToCurrentAction->text()+tr(" - KiloHz Player"));
    QString tmp;
    QString info=tr("");
    if(!(tmp=metaData.value("TITLE")).isEmpty())
        info = tr("标题：") + metaData.value("TITLE");
    if(!(tmp=metaData.value("ARTIST")).isEmpty())
        info = info + tr("\n艺人：") + metaData.value("ARTIST");
    if(!(tmp=metaData.value("ALBUM")).isEmpty())
        info = info + tr("\n专辑：") + metaData.value("ALBUM");
    musicInfo->setText(info);
    goToCurrentAction->setEnabled(true);
}

void MainWindow::changePausedState()
{
    Phonon::State currentState = mediaObject->state();
    if (currentState == Phonon::PlayingState ){
        mediaObject->pause();
    }else if ( currentState == Phonon::PausedState ){
        mediaObject->play();
    }else{
        mediaObject->setCurrentSource(sources.at(musicTable->currentRow()));
        mediaObject->play();
    }
}

void MainWindow::addItemToTable(QString fileName)
{
    QTableWidgetItem *titleItem = new QTableWidgetItem(friendlyFileName(fileName));
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);

    int currentRow = musicTable->rowCount();
    musicTable->insertRow(currentRow);
    musicTable->setItem(currentRow, 0, titleItem);
}

void MainWindow::changeMusic(bool next)
{
    int currentMusic = sources.indexOf(mediaObject->currentSource());
    int plyMode = plyModeBox->currentIndex();
    int newMusic=0;
    switch(plyMode){
    case 1:
        newMusic = qrand() % sources.size();
        break;
    case 0:
    case 2:
        if(next){
            (currentMusic >= sources.size()-1)? newMusic = 0 : newMusic=currentMusic+1;
        }else{
            (currentMusic <= 0)? newMusic = sources.size()-1 : newMusic=currentMusic-1;
        }
        break;
    default:
        ;
    }
    mediaObject->stop();
    mediaObject->clearQueue();
    mediaObject->setCurrentSource(sources.at(newMusic));
    mediaObject->play();
}

void MainWindow::showMusicMsg()
{
    if(!showMsg)
        return;
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
    QString name = musicTable->item(sources.indexOf(mediaObject->currentSource()),0)->text();
    trayIcon->showMessage(tr("正在播放:"), name, icon,3000);
}

void MainWindow::turnItemUp(bool up)
{
    int newRow,currentRow = musicTable->currentRow();
    if(up){
        newRow = currentRow - 1;
        if(newRow < 0)
            newRow = musicTable->rowCount()-1;
    }else{
        newRow = currentRow + 1;
        if(newRow >= musicTable->rowCount())
            newRow = 0;
    }
    QString tmpStr = musicTable->item(currentRow, 0)->text();
    musicTable->item(currentRow, 0)->setText( musicTable->item(newRow, 0)->text());
    musicTable->item(newRow, 0)->setText(tmpStr);
    Phonon::MediaSource tmpSource= sources.at(currentRow);
    sources.replace(currentRow, sources.at(newRow));
    sources.replace(newRow, tmpSource);
    musicTable->selectRow(newRow);

    writeList();
}

void MainWindow::goToCurrentMusic()
{
    musicTable->selectRow(sources.indexOf(mediaObject->currentSource()));
}

void MainWindow::goToNextMusic()
{
    changeMusic(true);
}

void MainWindow::goToPrevMusic()
{
    changeMusic(false);
}

void MainWindow::turnUp()
{
    turnItemUp(true);
}

void MainWindow::turnDown()
{
    turnItemUp(false);
}

void MainWindow::selectionChanged()
{
    pauseAction->setEnabled(true);
    prevAction->setEnabled(true);
    nextAction->setEnabled(true);

    deleteFileAction->setEnabled(true);
    clearAllAction->setEnabled(true);
    goUpAction->setEnabled(true);
    goDownAction->setEnabled(true);
}

void MainWindow::deleteCurrentItem()
{
    if(sources.isEmpty())
        return;
    int selectItem = musicTable->currentRow();
    if ( sources.at(selectItem) == mediaObject->currentSource() )
        mediaObject->stop();
    sources.removeAt(selectItem);
    musicTable->removeRow(selectItem);
    if(sources.isEmpty()){
        deleteFileAction->setDisabled(true);
        clearAllAction->setDisabled(true);
        goUpAction->setDisabled(true);
        goDownAction->setDisabled(true);
        pauseAction->setDisabled(true);
        prevAction->setDisabled(true);
        nextAction->setDisabled(true);
    }
    writeList();
}

void MainWindow::clearAll()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("您确定要清除列表中的所有项目吗？"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if(msgBox.exec()==QMessageBox::Cancel)
        return;

    mediaObject->stop();
    mediaObject->clearQueue();

    for(int i=0; i<sources.size(); ++i)
        musicTable->removeRow(0);
    sources.clear();
    QFile list(LISTFILE);
    if (!list.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    deleteFileAction->setDisabled(true);
    clearAllAction->setDisabled(true);
    goUpAction->setDisabled(true);
    goDownAction->setDisabled(true);
    pauseAction->setDisabled(true);
    prevAction->setDisabled(true);
    nextAction->setDisabled(true);

    list.close();
}

void MainWindow::tick(qint64 time)
{
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);

    timeLcd->display(displayTime.toString("mm:ss"));
}

void MainWindow::sourceChanged(const Phonon::MediaSource &source)
{
    musicTable->selectRow(sources.indexOf(source));
    timeLcd->display("00:00");
}

void MainWindow::aboutToFinish()
{
    int currentMusic = sources.indexOf(mediaObject->currentSource());
    int plyMode = plyModeBox->currentIndex();
    int newMusic=0;
    switch(plyMode){
    case 1://RANDOM
        newMusic = qrand() % sources.size();
        break;
    case 0://LoopAll
        (currentMusic >= sources.size()-1)? newMusic = 0 : newMusic=currentMusic+1;
        break;
    case 2://single
        newMusic = currentMusic;
        break;
    default:
        ;
    }
    mediaObject->enqueue(sources.at(newMusic));
}

void MainWindow::tableClicked(int row, int /* column */)
{
    mediaObject->stop();
    mediaObject->clearQueue();

    if (row >= sources.size())
        return;

    mediaObject->setCurrentSource(sources[row]);
    mediaObject->play();
}

void MainWindow::setupActions()
{
    showWindowAction = new QAction(tr("显示窗口(&W)"), this);
    showWindowAction->setShortcut(tr("Ctrl+W"));
    pauseAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("播放(&P)"), this);
    pauseAction->setShortcut(tr("Ctrl+P"));
    pauseAction->setDisabled(true);
    stopAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop), tr("停止(&S)"), this);
    stopAction->setShortcut(tr("Ctrl+S"));
    stopAction->setDisabled(true);
    nextAction = new QAction(style()->standardIcon(QStyle::SP_MediaSkipForward), tr("下一曲(&N)"), this);
    nextAction->setShortcut(tr("Ctrl+N"));
    nextAction->setDisabled(true);
    prevAction = new QAction(style()->standardIcon(QStyle::SP_MediaSkipBackward), tr("上一曲(&R)"), this);
    prevAction->setShortcut(tr("Ctrl+R"));
    prevAction->setDisabled(true);
    addFilesAction = new QAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder),tr("添加(&O)"), this);
    addFilesAction->setShortcut(tr("Ctrl+O"));
    exitAction = new QAction(tr("退出(&X)"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    aboutAction = new QAction(tr("关于"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    aboutQtAction = new QAction(tr("关于Qt"), this);
    aboutQtAction->setShortcut(tr("Ctrl+Q"));
    searchLine = new QLineEdit;
    searchNextButton = new QPushButton;
    searchNextButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    searchNextButton->setShortcut(tr("Ctrl+F"));
    setShowMsgAction = new QAction(tr("关闭气泡提示(&M)"), this);
    setShowMsgAction->setShortcut(tr("Ctrl+M"));
    goToCurrentAction = new QAction(tr(UNSELECT), this);
    QFont bigFont;
    bigFont.setPointSize(12);
    goToCurrentAction->setFont(bigFont);
    goToCurrentAction->setDisabled(true);
    deleteFileAction = new QAction(style()->standardIcon(QStyle::SP_DialogCloseButton),tr("删除(&D)"),this);
    deleteFileAction->setShortcut(tr("Ctrl+D"));
    clearAllAction = new QAction(style()->standardIcon(QStyle::SP_DialogResetButton),tr("清空(&C)"),this);
    clearAllAction->setShortcut(tr("Ctrl+C"));
    goUpAction = new QAction(style()->standardIcon(QStyle::SP_ArrowUp),tr("上移文件(&U)"),this);
    goUpAction->setShortcut(tr("Ctrl+U"));
    goDownAction = new QAction(style()->standardIcon(QStyle::SP_ArrowDown),tr("下移文件(&W)"),this);
    goDownAction->setShortcut(tr("Ctrl+W"));
    musicInfo = new QLabel;
    plyModeBox = new QComboBox;
    plyModeBox->insertItems(0, QStringList()<<tr("列表循环")<<tr("随机播放")<<tr("单曲循环"));

    connect(showWindowAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(changePausedState()) );
    connect(stopAction, SIGNAL(triggered()), mediaObject, SLOT(stop()));
    connect(nextAction, SIGNAL(triggered()), this, SLOT(goToNextMusic()));
    connect(prevAction, SIGNAL(triggered()), this, SLOT(goToPrevMusic()));
    connect(addFilesAction, SIGNAL(triggered()), this, SLOT(addFiles()));
    connect(exitAction, SIGNAL(triggered()),  qApp, SLOT(quit()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(setShowMsgAction,SIGNAL(triggered()),this,SLOT(setShowMsgState()));
    connect(deleteFileAction, SIGNAL(triggered()), this, SLOT(deleteCurrentItem()));
    connect(clearAllAction, SIGNAL(triggered()), this, SLOT(clearAll()));
    connect(goUpAction, SIGNAL(triggered()), this, SLOT(turnUp()));
    connect(goDownAction, SIGNAL(triggered()), this, SLOT(turnDown()));
    connect(goToCurrentAction,SIGNAL(triggered()), this, SLOT(goToCurrentMusic()));
    connect(searchLine,SIGNAL(textChanged(QString)), this, SLOT(searchMusic(QString)));
    connect(searchNextButton,SIGNAL(clicked()),this,SLOT(searchNext()));
}

void MainWindow::setupMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(addFilesAction);
    fileMenu->addAction(deleteFileAction);
    fileMenu->addAction(clearAllAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu *aboutMenu = menuBar()->addMenu(tr("帮助(&H)"));
    aboutMenu->addAction(setShowMsgAction);
    aboutMenu->addSeparator();
    aboutMenu->addAction(aboutAction);
    aboutMenu->addAction(aboutQtAction);
}

void MainWindow::setupUi()
{
    QToolBar *musicControlBar = new QToolBar;
    musicControlBar->addAction(prevAction);
    musicControlBar->addAction(pauseAction);
    musicControlBar->addAction(nextAction);
    musicControlBar->addAction(stopAction);

    QToolBar *fileOprBar = new QToolBar;
    fileOprBar->addAction(addFilesAction);
    fileOprBar->addAction(deleteFileAction);
    fileOprBar->addAction(clearAllAction);
    fileOprBar->addSeparator();
    fileOprBar->addAction(goUpAction);
    fileOprBar->addAction(goDownAction);


    QToolBar *showFileBar = new QToolBar;
    showFileBar->addAction(goToCurrentAction);

    seekSlider = new Phonon::SeekSlider(this);
    seekSlider->setMediaObject(mediaObject);

    volumeSlider = new Phonon::VolumeSlider(this);
    volumeSlider->setAudioOutput(audioOutput);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QLabel *volumeLabel = new QLabel;
    volumeLabel->setPixmap(QPixmap("images/volume.png"));

    QPalette palette;
    palette.setBrush(QPalette::Light, Qt::black);

    timeLcd = new QLCDNumber;
    timeLcd->setPalette(palette);

    musicTable = new QTableWidget(0, 1);
    musicTable->horizontalHeader()->setStretchLastSection(true);
    musicTable->horizontalHeader()->setVisible(false);
    musicTable->resizeColumnsToContents();
    musicTable->setSelectionMode(QAbstractItemView::SingleSelection);
  //  musicTable->verticalHeader()-
    musicTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(musicTable, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(tableClicked(int,int)));
    connect(musicTable,SIGNAL(itemSelectionChanged()),this,SLOT(selectionChanged()));

    QHBoxLayout *infoLayout = new QHBoxLayout;
    infoLayout->addWidget(showFileBar);
    infoLayout->addStretch();
    infoLayout->addWidget(musicInfo);

    QHBoxLayout *seekerLayout = new QHBoxLayout;
    seekerLayout->addWidget(seekSlider);
    seekerLayout->addWidget(timeLcd);

    QHBoxLayout *playbackLayout = new QHBoxLayout;
    playbackLayout->addWidget(musicControlBar);
    playbackLayout->addStretch();
    playbackLayout->addWidget(plyModeBox);
    playbackLayout->addWidget(volumeLabel);
    playbackLayout->addWidget(volumeSlider);

    QHBoxLayout *fileOprLayout = new QHBoxLayout;
    fileOprLayout->addWidget(fileOprBar);
    fileOprLayout->addStretch();
    fileOprLayout->addWidget(searchLine);
    fileOprLayout->addWidget(searchNextButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(playbackLayout);
    mainLayout->addLayout(seekerLayout);
    mainLayout->addLayout(fileOprLayout);
    mainLayout->addWidget(musicTable);

    QWidget *widget = new QWidget;
    widget->setLayout(mainLayout);

    setCentralWidget(widget);
    setWindowTitle(tr(DEFAULTTITLE));
}

void MainWindow::setupTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/files/logo.svg"));
    setWindowIcon(QIcon(":/files/logo.svg"));
    trayIconMenu->addAction(prevAction);
    trayIconMenu->addAction(pauseAction);
    trayIconMenu->addAction(nextAction);
    trayIconMenu->addAction(stopAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(setShowMsgAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(showWindowAction);
    trayIconMenu->addAction(exitAction);
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(showWin(QSystemTrayIcon::ActivationReason)));
}

/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainwindow.h"

#include <QLayout>
#include <QLabel>
#include <QGraphicsView>

#include <QAction>
#include <KActionCollection>
#include <QDebug>
#include <KgDifficulty>
#include <KScoreDialog>
#include <KStandardGameAction>
#include <QStatusBar>
#include <KToggleAction>

#include "mainarea.h"
#include "kollisionconfig.h"

MainWindow::MainWindow()
{
    m_main = new MainArea();
    QGraphicsView* view = new QGraphicsView(m_main, this);
    view->setOptimizationFlags( QGraphicsView::DontClipPainter |
                                QGraphicsView::DontSavePainterState |
                                QGraphicsView::DontAdjustForAntialiasing );
//    view->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    view->setCacheMode( QGraphicsView::CacheBackground );
    view->setFrameStyle(QFrame::NoFrame);

    setCentralWidget(view);

    Kg::difficulty()->addStandardLevelRange(
        KgDifficultyLevel::Easy, KgDifficultyLevel::Hard,
        KgDifficultyLevel::Hard //default
    );
    KgDifficultyGUI::init(this);
    connect(Kg::difficulty(), SIGNAL(currentLevelChanged(const KgDifficultyLevel*)), m_main, SLOT(abort()));

    setupActions();

    QLayout* l = layout();
    Q_ASSERT(l);
    l->setSizeConstraint(QLayout::SetFixedSize);

    // setup status bar
    QStatusBar* bar = statusBar();
    Q_ASSERT(bar);
    m_time_label = new QLabel("");
    bar->addPermanentWidget(m_time_label);

    m_balls_label = new QLabel("");
    bar->addWidget(m_balls_label);
//     bar->setItemAlignment(STATUSBAR_BALLS, Qt::AlignLeft);

    connect(m_main, &MainArea::changeGameTime, this, &MainWindow::setGameTime);
    connect(m_main, &MainArea::changeBallNumber, this, &MainWindow::setBallNumber);
    connect(m_main, &MainArea::changeState, this, &MainWindow::changeState);
    connect(m_main, &MainArea::pause, this, &MainWindow::pause);

    stateChanged("playing", KXMLGUIClient::StateReverse);
    connect(m_main, &MainArea::starting, this, &MainWindow::newGame);
    connect(m_main, &MainArea::gameOver, this, &MainWindow::gameOver);
}

MainWindow::~MainWindow()
{
    delete m_main;
}

void MainWindow::setupActions()
{
    // Game
    QAction * abort = actionCollection()->addAction( QLatin1String( "game_abort" ));
    abort->setText(i18n("End game"));
    connect(abort, &QAction::triggered, m_main, &MainArea::abort);

    QAction * pause = KStandardGameAction::pause(m_main, SLOT(togglePause()), actionCollection());
    m_main->setPauseAction(pause);
    KStandardGameAction::highscores(this, SLOT(highscores()), actionCollection());
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());

    QAction * action;
    action = new KToggleAction(i18n("&Play Sounds"), this);
    action->setChecked(KollisionConfig::enableSounds());
    actionCollection()->addAction( QLatin1String( "options_sounds" ), action);
    connect(action, &QAction::triggered, m_main, &MainArea::enableSounds);

    setupGUI(Create | Save | Keys | StatusBar);
}

void MainWindow::newGame()
{
    stateChanged("playing");
    m_lastUsedDifficulty = Kg::difficulty()->currentLevel();
}

void MainWindow::gameOver(int time)
{
    stateChanged("playing", KXMLGUIClient::StateReverse);

    KScoreDialog ksdialog(KScoreDialog::Name, this);
    ksdialog.initFromDifficulty(Kg::difficulty(), /*setConfigGroup=*/ false);
    ksdialog.setConfigGroup(qMakePair(
        m_lastUsedDifficulty->key(),
        m_lastUsedDifficulty->title()
    ));
    KScoreDialog::FieldInfo scoreInfo;
    scoreInfo[KScoreDialog::Score].setNum(time);
    if (ksdialog.addScore(scoreInfo, KScoreDialog::AskName)) {
        ksdialog.exec();
    }
}

void MainWindow::highscores()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Time, this);
    ksdialog.initFromDifficulty(Kg::difficulty());
    ksdialog.exec();
}

void MainWindow::setBallNumber(int number)
{
    m_balls_label->setText(i18n("Balls: %1", number));
}

void MainWindow::setGameTime(int time)
{
    m_time_label->setText(i18np("Time: %1 second", "Time: %1 seconds", time));
}

void MainWindow::changeState(bool running) {
    showCursor(!running);
    Kg::difficulty()->setGameRunning(running);
}

void MainWindow::pause(bool p)
{
    showCursor(p);
}

void MainWindow::showCursor(bool show)
{
    if (show) {
        centralWidget()->setCursor(QCursor());
    }
    else {
        centralWidget()->setCursor(Qt::BlankCursor);
    }
}



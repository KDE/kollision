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

#include <KAction>
#include <KActionCollection>
#include <KConfigDialog>
#include <KDebug>
#include <KGameDifficulty>
#include <KLocalizedString>
#include <KScoreDialog>
#include <KStandardAction>
#include <KStandardGameAction>
#include <KStatusBar>
#include <KToggleAction>

#include "mainarea.h"
#include "kollisionconfig.h"

QString difficulty(int value);

MainWindow::MainWindow()
{
    m_main = new MainArea();
    QGraphicsView* view = new QGraphicsView(m_main, this);
    view->setOptimizationFlags( QGraphicsView::DontClipPainter |
                                QGraphicsView::DontSavePainterState |
                                QGraphicsView::DontAdjustForAntialiasing );
//    view->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    view->setCacheMode( QGraphicsView::CacheBackground );

    setCentralWidget(view);

    setupActions();

    QLayout* l = layout();
    Q_ASSERT(l);
    l->setSizeConstraint(QLayout::SetFixedSize);

    // setup status bar
    KStatusBar* bar = statusBar();
    Q_ASSERT(bar);
    m_time_label = new QLabel("");
    bar->addPermanentWidget(m_time_label);

    m_balls_label = new QLabel("");
    bar->addWidget(m_balls_label);
//     bar->setItemAlignment(STATUSBAR_BALLS, Qt::AlignLeft);

    connect(m_main, SIGNAL(changeGameTime(int)), this, SLOT(setGameTime(int)));
    connect(m_main, SIGNAL(changeBallNumber(int)), this, SLOT(setBallNumber(int)));
    connect(m_main, SIGNAL(showCursor(bool)), this, SLOT(showCursor(bool)));

    stateChanged("playing", KXMLGUIClient::StateReverse);
    connect(m_main, SIGNAL(starting()), this, SLOT(newGame()));
    connect(m_main, SIGNAL(gameOver(int)), this, SLOT(gameOver(int)));
    
    KGameDifficulty::init(this, m_main, SLOT(difficultyChanged(KGameDifficulty::standardLevel)));
    KGameDifficulty::setRestartOnChange(KGameDifficulty::RestartOnChange);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Easy);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Medium);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Hard);
    KGameDifficulty::setLevel(KGameDifficulty::standardLevel(KollisionConfig::gameDifficulty()));
}

void MainWindow::setupActions()
{
    // Game
    KAction* abort = actionCollection()->addAction("game_abort");
    abort->setText(i18n("Abort game"));
    connect(abort, SIGNAL(triggered()), m_main, SLOT(abort()));
//
    KStandardGameAction::highscores(this, SLOT(highscores()), actionCollection());
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());

    KAction* action;
    action = new KToggleAction(i18n("&Play Sounds"), this);
    actionCollection()->addAction("options_sounds", action);
    connect(action, SIGNAL(triggered(bool)), m_main, SLOT(enableSounds(bool)));

    setupGUI();
}

void MainWindow::newGame()
{
    stateChanged("playing");
}

void MainWindow::gameOver(int time)
{
    stateChanged("playing", KXMLGUIClient::StateReverse);

    KScoreDialog ksdialog(KScoreDialog::Name, this);
    ksdialog.setConfigGroup(difficulty(KollisionConfig::gameDifficulty()));
    KScoreDialog::FieldInfo scoreInfo;
    kDebug() << "time = " << time << endl;
    scoreInfo[KScoreDialog::Score].setNum(time);
    if (ksdialog.addScore(scoreInfo, KScoreDialog::AskName)) {
        ksdialog.exec();
    }
    else {
        kDebug() << "ksdialog returned false" << endl;
    }
}

void MainWindow::highscores()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Time, this);
    ksdialog.setConfigGroup(difficulty(KollisionConfig::gameDifficulty()));
    ksdialog.exec();
}

void MainWindow::setBallNumber(int number)
{
    m_balls_label->setText(i18n("Balls: %1", number));
}

void MainWindow::setGameTime(int time)
{
    m_time_label->setText(
        time == 0 ? "" : i18np("Time: %1 second", "Time: %1 seconds", time));
}

void MainWindow::showCursor(bool visible) {
    if (visible) {
        centralWidget()->setCursor(QCursor());
    }
    else {
        centralWidget()->setCursor(Qt::BlankCursor);
    }
}

QString difficulty(int value)
{
    switch (value) {
    case KGameDifficulty::Easy:
        return i18nc("Difficulty level", "Easy");
        break;
    case KGameDifficulty::Medium:
        return i18nc("Difficulty level", "Medium");
        break;
    case KGameDifficulty::Hard:
    default:
        return i18nc("Difficulty level", "Hard");
        break;
    };
}

#include "mainwindow.moc"


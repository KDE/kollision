/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwindow.h"

#include <QLayout>
#include <QLabel>
#include <QGraphicsView>
#include <QPointer>
#include <QStatusBar>
#include <QAction>

#include <KActionCollection>
#include <KToggleAction>

#include <KGameDifficulty>
#include <KGameHighScoreDialog>
#include <KGameStandardAction>

#include "mainarea.h"
#include "kollisionconfig.h"

MainWindow::MainWindow()
{
    m_main = new MainArea();
    QGraphicsView* view = new QGraphicsView(m_main, this);
    view->setOptimizationFlags( 
                                QGraphicsView::DontSavePainterState |
                                QGraphicsView::DontAdjustForAntialiasing );
//    view->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    view->setCacheMode( QGraphicsView::CacheBackground );
    view->setFrameStyle(QFrame::NoFrame);

    setCentralWidget(view);

    KGameDifficulty::global()->addStandardLevelRange(
        KGameDifficultyLevel::Easy, KGameDifficultyLevel::Hard,
        KGameDifficultyLevel::Hard //default
    );
    KGameDifficultyGUI::init(this);
    connect(KGameDifficulty::global(), &KGameDifficulty::currentLevelChanged, m_main, &MainArea::abort);

    setupActions();

    QLayout* l = layout();
    Q_ASSERT(l);
    l->setSizeConstraint(QLayout::SetFixedSize);

    // setup status bar
    QStatusBar* bar = statusBar();
    Q_ASSERT(bar);
    m_time_label = new QLabel;
    bar->addPermanentWidget(m_time_label);

    m_balls_label = new QLabel;
    bar->addWidget(m_balls_label);
//     bar->setItemAlignment(STATUSBAR_BALLS, Qt::AlignLeft);

    connect(m_main, &MainArea::changeGameTime, this, &MainWindow::setGameTime);
    connect(m_main, &MainArea::changeBallNumber, this, &MainWindow::setBallNumber);
    connect(m_main, &MainArea::changeState, this, &MainWindow::changeState);
    connect(m_main, &MainArea::pause, this, &MainWindow::pause);

    stateChanged(QStringLiteral("playing"), KXMLGUIClient::StateReverse);
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
    QAction * abort = actionCollection()->addAction( QStringLiteral( "game_abort" ));
    abort->setText(i18nc("@action", "End Game"));
    connect(abort, &QAction::triggered, m_main, &MainArea::abort);

    QAction * pause = KGameStandardAction::pause(m_main, &MainArea::togglePause, actionCollection());
    m_main->setPauseAction(pause);
    KGameStandardAction::highscores(this, &MainWindow::highscores, actionCollection());
    KGameStandardAction::quit(this, &QWidget::close, actionCollection());

    QAction * ballSize = new KToggleAction(i18nc("@option:check", "&Increase Ball Size"), this);
    ballSize->setChecked(KollisionConfig::increaseBallSize());
    actionCollection()->addAction( QStringLiteral( "increase_ball_size" ), ballSize);
    connect(ballSize, &QAction::triggered, m_main, &MainArea::increaseBallSize);

    QAction * action;
    action = new KToggleAction(QIcon::fromTheme(QStringLiteral("speaker")),
                               i18nc("@option:check", "Play Sounds"), this);
    action->setChecked(KollisionConfig::enableSounds());
    actionCollection()->addAction( QStringLiteral( "options_sounds" ), action);
    connect(action, &QAction::triggered, m_main, &MainArea::enableSounds);

    setupGUI(Create | Save | Keys | StatusBar);
}

void MainWindow::newGame()
{
    stateChanged(QStringLiteral("playing"));
    m_lastUsedDifficulty = KGameDifficulty::global()->currentLevel();
}

void MainWindow::gameOver(int time)
{
    stateChanged(QStringLiteral("playing"), KXMLGUIClient::StateReverse);

    QPointer<KGameHighScoreDialog> ksdialog = new KGameHighScoreDialog(KGameHighScoreDialog::Name, this);
    ksdialog->initFromDifficulty(KGameDifficulty::global(), /*setConfigGroup=*/ false);
    ksdialog->setConfigGroup(qMakePair(
        m_lastUsedDifficulty->key(),
        m_lastUsedDifficulty->title()
    ));
    KGameHighScoreDialog::FieldInfo scoreInfo;
    scoreInfo[KGameHighScoreDialog::Score].setNum(time);
    if (ksdialog->addScore(scoreInfo, KGameHighScoreDialog::AskName)) {
        ksdialog->exec();
    }
    delete ksdialog;
}

void MainWindow::highscores()
{
    KGameHighScoreDialog ksdialog(KGameHighScoreDialog::Name | KGameHighScoreDialog::Time, this);
    ksdialog.initFromDifficulty(KGameDifficulty::global());
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
    KGameDifficulty::global()->setGameRunning(running);
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

#include "moc_mainwindow.cpp"

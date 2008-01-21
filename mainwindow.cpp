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
#include <KDebug>
#include <KLocalizedString>
#include <KStandardGameAction>
#include <KStandardAction>
#include <KActionCollection>
#include <KConfigDialog>
#include <KScoreDialog>
#include <KStatusBar>

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
}

void MainWindow::setupActions()
{
    // Game
    KAction* abort = actionCollection()->addAction("game_abort");
    abort->setText(i18n("Abort game"));
    connect(abort, SIGNAL(triggered()), m_main, SLOT(abort()));
//     KStandardGameAction::demo(m_main, SLOT(newSimulation()), actionCollection());
//     KStandardGameAction::restart(m_main, SLOT(restart()), actionCollection());
//
    KStandardGameAction::highscores(this, SLOT(highscores()), actionCollection());
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());

    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

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

void MainWindow::optionsPreferences()
{
    if (KConfigDialog::showDialog("preferences")) {
        return;
    }

    KConfigDialog* dialog = new KConfigDialog(this, "preferences", KollisionConfig::self());
    QWidget* mainPage = new QWidget(dialog);
    m_pref_ui.setupUi(mainPage);
    dialog->addPage(mainPage, i18n("Main page"), "main_page");
    connect(dialog, SIGNAL(settingsChanged(const QString&)),
            m_main, SLOT(enableSounds()));

    dialog->show();
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
        setCursor(QCursor());
    }
    else {
        setCursor(Qt::BlankCursor);
    }
}

QString difficulty(int value)
{
    switch (value) {
    case 0:
        return i18nc("Difficulty level", "Easy");
        break;
    case 1:
        return i18n("Difficulty level", "Medium");
        break;
    case 2:
    default:
        return i18n("Difficulty level", "Hard");
        break;
    };
}

#include "mainwindow.moc"


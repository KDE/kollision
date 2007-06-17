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

#include <kdebug.h>
#include <klocalizedstring.h>
#include <kstatusbar.h>
#include "mainarea.h"

MainWindow::MainWindow()
{
    MainArea* main = new MainArea(this);
    setCentralWidget(main);

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
    
    connect(main, SIGNAL(changeGameTime(int)), this, SLOT(setGameTime(int)));
    connect(main, SIGNAL(changeBallNumber(int)), this, SLOT(setBallNumber(int)));
}

void MainWindow::setupActions()
{
    // Game
//     KStandardGameAction::gameNew(m_main, SLOT(newGame()), actionCollection());
//     KStandardGameAction::demo(m_main, SLOT(newSimulation()), actionCollection());
//     KStandardGameAction::restart(m_main, SLOT(restart()), actionCollection());
//     
//     KStandardGameAction::highscores(m_main, SLOT(highscores()), actionCollection());
//     KStandardGameAction::quit(this, SLOT(close()), actionCollection());

    // Move

    // Settings
//     KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());



    setupGUI();
}

void MainWindow::optionsPreferences()
{
    kDebug() << "preferences" << endl;
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

#include "mainwindow.moc"


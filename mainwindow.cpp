/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainwindow.h"

#include <QLayout>
#include <kdebug.h>
#include "mainarea.h"

MainWindow::MainWindow()
{
    setCentralWidget(new MainArea(this));

    setupActions();
    
    QLayout* l = layout();
    Q_ASSERT(l);
    l->setSizeConstraint(QLayout::SetFixedSize);
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


#include "mainwindow.moc"


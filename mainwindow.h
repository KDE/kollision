/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KGameDifficulty>
#include <KXmlGuiWindow>
#include "ui_preferences.h"

class QLabel;
class MainArea;

class MainWindow : public KXmlGuiWindow
{
Q_OBJECT
    QLabel* m_balls_label;
    QLabel* m_time_label;
    Ui_Preferences m_pref_ui;
    MainArea* m_main;
    QPair<QByteArray, QString> m_lastUsedDifficulty;
    
    void showCursor(bool show);
public:
    MainWindow();
    ~MainWindow();
protected:
    void setupActions();
protected slots:
    void setGameTime(int time);
    void setBallNumber(int number);
    
    void newGame();
    void gameOver(int);
    void highscores();
    void changeState(bool);
    void difficultyChanged(KGameDifficulty::standardLevel level);
    void pause(bool);
};


#endif // MAINWINDOW_H


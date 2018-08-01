/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class KgDifficultyLevel;
#include <KXmlGuiWindow>
#include "ui_preferences.h"

class QLabel;
class MainArea;

class MainWindow : public KXmlGuiWindow
{
Q_OBJECT
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
    void pause(bool);
private:
    QLabel* m_balls_label = nullptr;
    QLabel* m_time_label = nullptr;
    Ui_Preferences m_pref_ui;
    MainArea* m_main = nullptr;
    const KgDifficultyLevel* m_lastUsedDifficulty = nullptr;

    void showCursor(bool show);

};


#endif // MAINWINDOW_H


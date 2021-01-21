/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    ~MainWindow() override;
protected:
    void setupActions();
protected Q_SLOTS:
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


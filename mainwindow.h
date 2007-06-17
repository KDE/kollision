/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <kxmlguiwindow.h>

class MainWindow : public KXmlGuiWindow
{
Q_OBJECT
public:
    MainWindow();
protected:
    void setupActions();
protected slots:
    void optionsPreferences();
};


#endif // MAINWINDOW_H


/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef BUTTON_H
#define BUTTON_H

#include <sprite.h>
#include <kgamecanvas.h>
#include <QIcon>
#include <QFont>
#include <QSize>

class Button : public SpriteMixin<KGameCanvasPixmap>
{
    QIcon m_icon;
    QFont m_font;
    QString m_text;
    QSize m_size;
    
    virtual void repaint();
public:
    Button(KGameCanvasAbstract* parent, const QIcon& icon, 
           const QFont& font, const QString& text);
};

#endif // BUTTON_H

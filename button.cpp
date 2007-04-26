/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "button.h"

#include <QImage>
#include <kdebug.h>

Button::Button(KGameCanvasAbstract* parent, const QIcon& icon, 
               const QFont& font, const QString& text)
: SpriteMixin<KGameCanvasPixmap>(parent)
, m_icon(icon)
, m_font(font)
, m_text(text)
{
    QFontMetrics fm(m_font);
    int h = fm.height();
    if (h < 32) {
        h = 32;
    }
    m_size = QSize(fm.width(m_text), h);
    m_size.rwidth() += 10 + 32 + 10 + 10;
    m_size.rheight() += 10 + 10;
    
    repaint();
}

void Button::repaint()
{
    QImage tmp(m_size, QImage::Format_ARGB32_Premultiplied);
    tmp.fill(0);
    {
        QPainter p(&tmp);
        p.setRenderHint(QPainter::Antialiasing);
        QPen pen(QColor(200, 200, 220, 255));
        pen.setWidth(2);
        p.setPen(pen);
        p.setBrush(QBrush(QColor(0, 0, 0, 100)));
        p.drawRoundRect(0, 0, m_size.width() - 2, m_size.height() -2, 8, 40);
        p.drawPixmap(10, 
                    m_size.height() / 2 - 16, 
                    32, 
                    32, 
                    m_icon.pixmap(32, 32));
    
        p.setFont(m_font);
        p.drawText(10+32+10, 
                m_size.height() / 2 + 6, 
                m_text);
    }
    
    setPixmap(QPixmap::fromImage(tmp));
}




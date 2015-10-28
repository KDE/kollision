/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "message.h"
#include <QFontMetrics>
#include <QPainter>
#include <QTextDocument>
#include <QTextOption>

Message::Message(const QString& text, const QFont& font, int maxwidth)
: QGraphicsTextItem(text)
, m_velocity(0.0, 0.0)
{
    setFont(font);
    setDefaultTextColor(Qt::black);
    setAcceptHoverEvents(false);
    document()->setTextWidth(maxwidth);
    QTextOption opts = document()->defaultTextOption();
    opts.setAlignment(Qt::AlignHCenter);
    document()->setDefaultTextOption(opts);
    
    // translate so that the origin is the center
    setTransform(QTransform::fromTranslate(-document()->size().width() / 2, -document()->size().height() / 2), true);
}

void Message::setOpacityF(qreal opacity)
{
    QGraphicsItem::setOpacity(opacity);
}

qreal Message::opacityF() const
{
    return QGraphicsItem::opacity();
}

void Message::setVelocity(const QPointF& vel)
{
    m_velocity = vel;
}

QPointF Message::velocity() const
{
    return m_velocity;
}

void Message::setPosition(const QPointF& pos)
{
    QGraphicsTextItem::setPos(pos);
}

QPointF Message::position() const
{
    return QGraphicsTextItem::pos();
}

int Message::height() const {
    return document()->size().height();
}

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

Message::Message(const QString& text, const QFont& font)
: QGraphicsTextItem(text)
, m_opacity(1.0)
, m_velocity(0.0, 0.0)
{
    setFont(font);
    setDefaultTextColor(Qt::black);
    setAcceptsHoverEvents(false);
    
    // translate so that the origin is the center of the
    // bounding rect
    QFontMetrics metrics(font);
    QSize offset = metrics.boundingRect(text).size();
    translate(-offset.width() / 2, -offset.height() / 2);
}

void Message::paint(QPainter *painter, 
                    const QStyleOptionGraphicsItem* option, 
                    QWidget* widget)
{
    qreal oldOpacity = painter->opacity();
    painter->setOpacity(m_opacity);
    QGraphicsTextItem::paint(painter, option, widget);
    painter->setOpacity(oldOpacity);
}

void Message::setOpacityF(qreal opacity)
{
    m_opacity = opacity;
    update();
}

qreal Message::opacityF() const
{
    return m_opacity;
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


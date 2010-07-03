/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "ball.h"
#include <QPainter>
#include "renderer.h"

Ball::Ball(Renderer* renderer, const QString& id)
: m_velocity(0.0, 0.0)
{
    setPixmap(renderer->render(id));
    setShapeMode(BoundingRectShape);
    QSize size = pixmap().size();
    translate(-size.width() / 2, -size.height() / 2);
    setAcceptsHoverEvents(false);
}

void Ball::setOpacityF(qreal opacity)
{
    QGraphicsItem::setOpacity(opacity);
}

qreal Ball::opacityF() const
{
    return QGraphicsItem::opacity();
}

void Ball::setVelocity(const QPointF& vel)
{ 
    m_velocity = vel;
}

QPointF Ball::velocity() const
{
    return m_velocity;
}

void Ball::setPosition(const QPointF& pos)
{ 
    QGraphicsPixmapItem::setPos(pos);
}

QPointF Ball::position() const
{
    return QGraphicsPixmapItem::pos();
}

qreal Ball::radius() const
{
    return pixmap().size().width() / 2.0;
}

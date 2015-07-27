/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
  Copyright (c) 2010 Brian Croom <brian.s.croom@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "ball.h"
#include <QPainter>
#include <KGameRenderer>

Ball::Ball(KGameRenderer* renderer, const QString& id, int size)
: KGameRenderedItem(renderer, id), m_velocity(0.0, 0.0)
{
    setRenderSize(QSize(size, size));
    setShapeMode(BoundingRectShape);
    setTransform(QTransform::fromTranslate(-size / 2, -size / 2), true);
    setAcceptHoverEvents(false);
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
    return renderSize().width() / 2.0;
}

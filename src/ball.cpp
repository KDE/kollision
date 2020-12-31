/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>
    SPDX-FileCopyrightText: 2010 Brian Croom <brian.s.croom@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ball.h"
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

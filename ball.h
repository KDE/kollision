/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef BALL_H
#define BALL_H

#include <QGraphicsPixmapItem>
#include <QPointF>

class Renderer;

class Ball : public QGraphicsPixmapItem
{
    QPointF m_velocity;
    qreal m_opacity;
public:
    Ball();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);

    void setOpacity( qreal opacity ) { m_opacity = opacity; update(); }
    qreal opacity() const { return m_opacity; }

    void setVelocity(const QPointF& vel) { m_velocity = vel; }
    QPointF velocity() const { return m_velocity; }
};

#endif // BALL_H


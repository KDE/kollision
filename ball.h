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
#include "sprite.h"

class Renderer;

class Ball : public QGraphicsPixmapItem, public Sprite
{
    qreal m_opacity;
    QPointF m_velocity;
public:
    Ball(Renderer* renderer, const QString& id);

    virtual void paint(QPainter *painter, 
                       const QStyleOptionGraphicsItem* option, 
                       QWidget* widget = 0);

    virtual void setOpacityF(qreal opacity);
    virtual qreal opacityF() const;

    virtual void setVelocity(const QPointF& vel);
    virtual QPointF velocity() const;
    
    virtual void setPosition(const QPointF& pos);
    virtual QPointF position() const;
    
    qreal radius() const;
};

#endif // BALL_H


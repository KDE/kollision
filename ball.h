/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
  Copyright (c) 2010 Brian Croom <brian.s.croom@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef BALL_H
#define BALL_H

#include <KGameRenderedItem>
#include <QPointF>
#include "sprite.h"

class KGameRenderer;

class Ball : public KGameRenderedItem, public Sprite
{
    QPointF m_velocity;
public:
    Ball(KGameRenderer* renderer, const QString& id, int size);

    virtual void setOpacityF(qreal opacity);
    virtual qreal opacityF() const;

    virtual void setVelocity(const QPointF& vel);
    virtual QPointF velocity() const;
    
    virtual void setPosition(const QPointF& pos);
    virtual QPointF position() const;
    
    qreal radius() const;
};

#endif // BALL_H


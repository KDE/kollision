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
public:
    Ball(KGameRenderer* renderer, const QString& id, int size);

    void setOpacityF(qreal opacity) Q_DECL_OVERRIDE;
    qreal opacityF() const Q_DECL_OVERRIDE;

    void setVelocity(const QPointF& vel) Q_DECL_OVERRIDE;
    QPointF velocity() const Q_DECL_OVERRIDE;
    
    void setPosition(const QPointF& pos) Q_DECL_OVERRIDE;
    QPointF position() const Q_DECL_OVERRIDE;
    
    qreal radius() const;
private:
    QPointF m_velocity;
};

#endif // BALL_H


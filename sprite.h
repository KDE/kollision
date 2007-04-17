/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <QPointF>
#include "spritefwd.h"

class KGameCanvasAbstract;

class Sprite : public KShared
{
public:
    virtual ~Sprite() { }
    virtual double opacityF() const = 0;
    virtual void setOpacityF(double val) = 0;
    virtual const QPointF& position() const = 0;
    virtual void setPosition(const QPointF& pos) = 0;
    virtual const QPointF& velocity() const = 0;
    virtual void setVelocity(const QPointF& vel) = 0;
};

/**
  * @brief A simple mixin to reduce keystrokes.
  */
template <typename Item>
class SpriteMixin : public Item, public Sprite
{
protected:
    double m_opacityF;
    QPointF m_position;
    QPointF m_velocity;
public:
    SpriteMixin(KGameCanvasAbstract* parent)
    : Item(parent)
    {
    }
    
    virtual double opacityF() const { return m_opacityF; }
    virtual void setOpacityF(double val) {
        m_opacityF = val;
        Item::setOpacity((int) (255 * m_opacityF));
    }
    virtual const QPointF& position() const { return m_position; }
    virtual void setPosition(const QPointF& pos) {
        m_position = pos;
        Item::moveTo(m_position.toPoint());
    }
    virtual const QPointF& velocity() const { return m_velocity; }
    virtual void setVelocity(const QPointF& vel) {
        m_velocity = vel;
    }
};
    
#endif // SPRITE_H

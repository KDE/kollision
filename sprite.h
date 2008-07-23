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


class Sprite : public KShared
{
public:
    virtual ~Sprite() { }
    virtual qreal opacityF() const = 0;
    virtual void setOpacityF(qreal val) = 0;
    virtual QPointF position() const = 0;
    virtual void setPosition(const QPointF& pos) = 0;
    virtual QPointF velocity() const = 0;
    virtual void setVelocity(const QPointF& vel) = 0;
};

    
#endif // SPRITE_H

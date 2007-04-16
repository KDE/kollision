/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef BALL_H
#define BALL_H

#include <kgamecanvas.h>
#include <QPointF>
#include "sprite.h"

class Renderer;

class Ball : public SpriteMixin<KGameCanvasPixmap>
{
    QString m_id;
    double m_radius;
public:
    Ball(KGameCanvasAbstract* parent, Renderer* renderer, const QString& id);
    
    void update(Renderer* renderer);
    virtual void setPosition(const QPointF& p);
    
    double radius() const { return m_radius; }
};

#endif // BALL_H


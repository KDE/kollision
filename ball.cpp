/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "ball.h"
#include "renderer.h"

Ball::Ball(KGameCanvasAbstract* parent, Renderer* renderer, const QString& id)
: SpriteMixin<KGameCanvasPixmap>(parent)
, m_id(id)
, m_radius(renderer->size().width() / 2.0)
{
    update(renderer);
}

void Ball::update(Renderer* renderer)
{
    setPixmap(renderer->render(m_id));
}

void Ball::setPosition(const QPointF& p) { 
    m_position = p; 
    moveTo((p - QPointF(radius(), radius())).toPoint());
}


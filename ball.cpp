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
: KGameCanvasPixmap(parent)
, m_id(id)
{
    update(renderer);
}

void Ball::update(Renderer* renderer)
{
    setPixmap(renderer->render(m_id));
}



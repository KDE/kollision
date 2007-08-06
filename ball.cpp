/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <QPainter>
#include "ball.h"
#include "renderer.h"

Ball::Ball()
    : m_opacity(1.0)
{
    setShapeMode( BoundingRectShape );
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    painter->save();
    painter->setOpacity( m_opacity );
    QGraphicsPixmapItem::paint(painter, option, widget);
    painter->restore();
}

/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <QPointF>
#include "spritefwd.h"

class Sprite : public QSharedData
{
public:
    virtual ~Sprite() { }
    virtual qreal opacityF() const = 0;
    virtual void setOpacityF(qreal opacity) = 0;
    virtual QPointF position() const = 0;
    virtual void setPosition(const QPointF& pos) = 0;
    virtual QPointF velocity() const = 0;
    virtual void setVelocity(const QPointF& vel) = 0;
};

    
#endif // SPRITE_H

/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>
    SPDX-FileCopyrightText: 2010 Brian Croom <brian.s.croom@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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

    void setOpacityF(qreal opacity) override;
    qreal opacityF() const override;

    void setVelocity(const QPointF& vel) override;
    QPointF velocity() const override;
    
    void setPosition(const QPointF& pos) override;
    QPointF position() const override;
    
    qreal radius() const;
private:
    QPointF m_velocity;
};

#endif // BALL_H


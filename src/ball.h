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


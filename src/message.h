/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QGraphicsTextItem>
#include <QExplicitlySharedDataPointer>
#include "sprite.h"

class Message : public QGraphicsTextItem, public Sprite
{
public:
    Message(const QString& text, const QFont& font, int maxwidth);

    void setOpacityF(qreal opacity) override;
    qreal opacityF() const override;
    
    void setVelocity(const QPointF& vel) override;
    QPointF velocity() const override;
    
    void setPosition(const QPointF& pos) override;
    QPointF position() const override;
    
    int height() const;
private:
    QPointF m_velocity;
};

typedef QExplicitlySharedDataPointer<Message> MessagePtr;

#endif // MESSAGE_H

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

    void setOpacityF(qreal opacity) Q_DECL_OVERRIDE;
    qreal opacityF() const Q_DECL_OVERRIDE;
    
    void setVelocity(const QPointF& vel) Q_DECL_OVERRIDE;
    QPointF velocity() const Q_DECL_OVERRIDE;
    
    void setPosition(const QPointF& pos) Q_DECL_OVERRIDE;
    QPointF position() const Q_DECL_OVERRIDE;
    
    int height() const;
private:
    QPointF m_velocity;
};

typedef QExplicitlySharedDataPointer<Message> MessagePtr;

#endif // MESSAGE_H

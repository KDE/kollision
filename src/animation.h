/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <QPointF>
#include <QList>
#include <QQueue>
#include <QObject>
#include "spritefwd.h"

class Animation : public QObject
{
Q_OBJECT
public:
    ~Animation() override;
    
    virtual void start(int t) = 0;
    virtual bool step(int t) = 0;
public Q_SLOTS:
    virtual void stop() = 0;
Q_SIGNALS:
    void over();
};

class PauseAnimation : public Animation
{
Q_OBJECT
    int m_time;
    int m_start;
public:
    explicit PauseAnimation(int time);
    
    void start(int t) override;
    bool step(int t) override;
    void stop() override { }
};

class FadeAnimation : public Animation
{
Q_OBJECT
    SpritePtr m_sprite;
    double m_from;
    double m_to;
    int m_time;
    int m_start;
    
    bool m_stopped;
public:
    FadeAnimation(const SpritePtr& sprite, double from, double to, int time);
    
    void start(int t) override;
    bool step(int t) override;
    void stop() override;
};

class MovementAnimation : public Animation
{
Q_OBJECT
    SpritePtr m_sprite;
    QPointF m_from;
    QPointF m_velocity;
    int m_time;
    int m_last;
public:
    MovementAnimation(const SpritePtr& sprite, const QPointF& from, 
                      const QPointF& velocity, int time);
    
    void start(int t) override;
    bool step(int t) override;
    void stop() override;
};


class AnimationGroup : public Animation
{
Q_OBJECT
    typedef QList<Animation*> List;
    List m_animations;
    int m_last;
public:
    AnimationGroup();
    
    void add(Animation* animation);
    
    void start(int t) override;
    bool step(int t) override;
    void stop() override;
};

class AnimationSequence : public Animation
{
Q_OBJECT
    QQueue<Animation*> m_animations;
    int m_last;
public:
    AnimationSequence();
    
    void add(Animation* animation);
    
    void start(int t) override;
    bool step(int t) override;
    void stop() override;
};

#endif // ANIMATION_H


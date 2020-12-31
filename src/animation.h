/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
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
    virtual ~Animation();
    
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
    
    void start(int t) Q_DECL_OVERRIDE;
    bool step(int t) Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE { }
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
    
    void start(int t) Q_DECL_OVERRIDE;
    bool step(int t) Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;
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
    
    void start(int t) Q_DECL_OVERRIDE;
    bool step(int t) Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;
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
    
    void start(int t) Q_DECL_OVERRIDE;
    bool step(int t) Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;
};

class AnimationSequence : public Animation
{
Q_OBJECT
    QQueue<Animation*> m_animations;
    int m_last;
public:
    AnimationSequence();
    
    void add(Animation* animation);
    
    void start(int t) Q_DECL_OVERRIDE;
    bool step(int t) Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;
};

#endif // ANIMATION_H


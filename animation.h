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
#include "spritefwd.h"

class Animation
{
public:
    virtual ~Animation();
    
    virtual void start(int t) = 0;
    virtual bool step(int t) = 0;
    virtual void stop() = 0;
};

class PauseAnimation : public Animation
{    
    int m_time;
    int m_start;
public:
    PauseAnimation(int time);
    
    virtual void start(int t);
    virtual bool step(int t);
    virtual void stop() { }
};

class FadeAnimation : public Animation
{
    SpritePtr m_sprite;
    double m_from;
    double m_to;
    int m_time;
    int m_start;
public:
    FadeAnimation(const SpritePtr& sprite, double from, double to, int time);
    
    virtual void start(int t);
    virtual bool step(int t);
    virtual void stop();
};

class MovementAnimation : public Animation
{
    SpritePtr m_sprite;
    QPointF m_from;
    QPointF m_velocity;
    int m_time;
    int m_last;
public:
    MovementAnimation(const SpritePtr& sprite, const QPointF& from, 
                      const QPointF& velocity, int time);
    
    virtual void start(int t);
    virtual bool step(int t);
    virtual void stop();
};

class AnimationGroup : public Animation
{
    typedef QList<Animation*> List;
    List m_animations;
    int m_last;
public:
    AnimationGroup();
    
    void add(Animation* animation);
    
    virtual void start(int t);
    virtual bool step(int t);
    virtual void stop();
};

class AnimationSequence : public Animation
{
    QQueue<Animation*> m_animations;
    int m_last;
public:
    AnimationSequence();
    
    void add(Animation* animation);
    
    virtual void start(int t);
    virtual bool step(int t);
    virtual void stop();
};

#endif // ANIMATION_H


/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "animation.h"

#include "sprite.h"

#include <math.h>

Animation::~Animation()
{
    Q_EMIT over();
}

PauseAnimation::PauseAnimation(int time)
: m_time(time)
{
}

void PauseAnimation::start(int t)
{
    m_start = t;
}

bool PauseAnimation::step(int t)
{
    return t - m_start >= m_time;
}

FadeAnimation::FadeAnimation(const SpritePtr& sprite, double from, double to, int time)
: m_sprite(sprite)
, m_from(from)
, m_to(to)
, m_time(time)
, m_stopped(false)
{
}

void FadeAnimation::start(int t)
{
    m_start = t;
    m_sprite->setOpacityF(m_from);
}

bool FadeAnimation::step(int t)
{
    if (m_stopped) {
        return true;
    }
    else {
        double val = m_from + (m_to - m_from) * (t - m_start) / m_time;
        m_sprite->setOpacityF(val);
        return t - m_start >= m_time;
    }
}

void FadeAnimation::stop()
{
    m_sprite->setOpacityF(m_to);
    m_stopped = true;
}


MovementAnimation::MovementAnimation(const SpritePtr& sprite, const QPointF& from, 
                                     const QPointF& velocity, int time)
: m_sprite(sprite)
, m_from(from)
, m_velocity(velocity)
, m_time(time)
{
}

void MovementAnimation::start(int t)
{
    m_last = t;
    m_sprite->setPosition(m_from);
}

bool MovementAnimation::step(int t)
{
    int delta = t - m_last;
    m_last = t;
    m_sprite->setPosition(m_sprite->position() + delta * m_velocity);
    m_time -= delta;
    return m_time <= 0;
}

void MovementAnimation::stop()
{
    m_sprite->setPosition(m_from + m_velocity * m_time);
}


AnimationGroup::AnimationGroup()
: m_last(-1)
{
}

void AnimationGroup::add(Animation* a)
{
    m_animations.append(a);
    if (m_last != -1) {
        a->start(m_last);
    }
}

void AnimationGroup::start(int t)
{
    m_last = t;
    for (Animation* a : qAsConst(m_animations)) {
        a->start(t);
    }
}

bool AnimationGroup::step(int t)
{
    m_last = t;
    for (List::iterator it = m_animations.begin();
         it != m_animations.end(); ) {
        if ((*it)->step(t)) {
            delete *it;
            it = m_animations.erase(it);
        }
        else {
            ++it;
        }
    }
    
    return m_animations.isEmpty();
}

void AnimationGroup::stop()
{
    qDeleteAll(m_animations);
    m_animations.clear();
}


AnimationSequence::AnimationSequence()
: m_last(-1)
{
}


void AnimationSequence::add(Animation* a)
{
    m_animations.enqueue(a);
    if (m_last != -1) {
        a->start(m_last);
    }
}

void AnimationSequence::start(int t)
{
    m_last = t;
    if (!m_animations.isEmpty()) {
        m_animations.head()->start(t);
    }
}

bool AnimationSequence::step(int t)
{
    m_last = t;
    while (!m_animations.isEmpty()) {
        if (m_animations.head()->step(t)) {
            delete m_animations.dequeue();
            if (!m_animations.isEmpty()) {
                m_animations.head()->start(t);
            }
        }
        else {
            return false;
        }
    }
    return true;
}

void AnimationSequence::stop()
{
    while (!m_animations.isEmpty()) {
        m_animations.head()->stop();
        delete m_animations.dequeue();
    }
}




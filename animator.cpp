/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "animator.h"
#include <kdebug.h>

Animator::Animator()
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

void Animator::add(Animation* a)
{
    if (!m_timer.isActive()) {
        startTimer();
    }
    AnimationGroup::add(a);
}

void Animator::startTimer()
{
    m_time.restart();
    m_timer.start(0);
    start(0);
}

void Animator::stopTimer()
{
    m_timer.stop();
    stop();
}

void Animator::tick()
{
    if (AnimationGroup::step(m_time.elapsed())) {
        stopTimer();
    }
}


#include "animator.moc"



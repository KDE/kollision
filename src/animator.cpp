/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "animator.h"

Animator::Animator()
{
    connect(&m_timer, &QTimer::timeout, this, &Animator::tick);
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






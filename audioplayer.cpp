/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "audioplayer.h"

#include <KStandardDirs>
#include <Phonon/MediaObject>

AudioPlayer::AudioPlayer()
{
    m_media = 0;
    m_sounds.insert(HIT_WALL, KStandardDirs::locate("appdata", "sounds/hit_wall.ogg"));
    m_sounds.insert(YOU_LOSE, KStandardDirs::locate("appdata", "sounds/you_lose.ogg"));
    m_sounds.insert(BALL_LEAVING, KStandardDirs::locate("appdata", "sounds/ball_leaving.ogg"));
    m_sounds.insert(START, KStandardDirs::locate("appdata", "sounds/start.ogg"));
}

AudioPlayer::~AudioPlayer()
{
    delete m_media;
}

void AudioPlayer::setActive(bool active)
{
    if (active) {
        if (!m_media) {
            m_media = Phonon::createPlayer(Phonon::GameCategory);
        }
    }
    else {
        delete m_media;
        m_media = 0;
    }
}

void AudioPlayer::play(Sound sound)
{
    if (m_media) {
        QString path = m_sounds.value(sound);
        if (!path.isEmpty()) {
            m_media->setCurrentSource(path);
            m_media->play();
        }
    }
}

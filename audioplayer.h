/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMap>

namespace Phonon { class MediaObject; }

class AudioPlayer
{
    Phonon::MediaObject* m_media;
    QMap<int, QString> m_sounds;
public:
    enum Sound {
        HIT_WALL,
        YOU_LOSE,
        BALL_LEAVING,
        START
    };
    
    AudioPlayer();
    ~AudioPlayer();
    
    void play(Sound sound);
    void setActive(bool active);
};

#endif // AUDIOPLAYER_H

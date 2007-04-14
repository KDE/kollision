/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include <KApplication>
#include <KAboutData>
#include <KLocale>
#include <KCmdLineArgs>

#include "mainarea.h"

static KCmdLineOptions options[] =
    {
        { "s", 0, 0 },
        { "set-arbitrary-names", I18N_NOOP( "Fill in the fildes to speed up testing. Just for development purpose." ), 0 },
        KCmdLineLastOption
    };


int main(int argc, char *argv[])
{
    KAboutData aboutData("kollision", I18N_NOOP("Kollision"),
                         "0.1", I18N_NOOP("KDE collision game"), KAboutData::License_GPL,
                         "(c) 2007 Paolo Capriotti", 0, "", "p.capriotti@gmail.com");
    aboutData.addAuthor("Paolo Capriotti", 0, "p.capriotti@gmail.com");
    aboutData.addCredit("Matteo Guarnieri", I18N_NOOP("Original idea"), 0, 0);

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineArgs::addCmdLineOptions(options);
    KCmdLineArgs::parsedArgs();
    KApplication app;

    MainArea* area = new MainArea;
    area->resize(500, 500);
    area->show();
    area->start();
    
    return app.exec();
}

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

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    KAboutData aboutData("kollision", 0, ki18n("Kollision"),
                         "0.1", ki18n("KDE collision game"), KAboutData::License_GPL,
                         ki18n("(c) 2007 Paolo Capriotti"), KLocalizedString(), "http://games.kde.org/kollision");
    aboutData.addAuthor(ki18n("Paolo Capriotti"), KLocalizedString(), "p.capriotti@gmail.com");
    aboutData.addAuthor(ki18n("Dmitry Suzdalev"), KLocalizedString(), "dimsuz@gmail.com");
    aboutData.addCredit(ki18n("Matteo Guarnieri"), ki18n("Original idea"));

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;
    KGlobal::locale()->insertCatalog("libkdegames");

    MainWindow* window = new MainWindow;
    window->show();
    
    return app.exec();
}

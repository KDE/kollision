/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/


#include <KAboutData>

#include <QApplication>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <kdelibs4configmigrator.h>
#include <KDBusService>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    Kdelibs4ConfigMigrator migrate(QStringLiteral("kollision"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kollisionrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kollisionui.rc"));
    migrate.migrate();

    QApplication app(argc, argv);

    Kdelibs4ConfigMigrator migrate(QStringLiteral("kollision"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kollisionrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kollisionui.rc"));
    migrate.migrate();


    KAboutData aboutData("kollision", i18n("Kollision"),
                         "0.2", i18n("KDE collision game"), KAboutLicense::GPL,
                         i18n("(c) 2007 Paolo Capriotti"), "http://games.kde.org/kollision");
    aboutData.addAuthor(i18n("Paolo Capriotti"), QString(), "p.capriotti@gmail.com");
    aboutData.addAuthor(i18n("Dmitry Suzdalev"), QString(), "dimsuz@gmail.com");
    aboutData.addCredit(i18n("Matteo Guarnieri"), i18n("Original idea"));
    aboutData.addCredit(i18n("Brian Croom"), i18n("Port to use KGameRenderer"));


    app.setWindowIcon(QIcon::fromTheme(QLatin1String("kollision")));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    KDBusService service;

    MainWindow* window = new MainWindow;
    window->show();
    
    return app.exec();
}

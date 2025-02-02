/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwindow.h"
#include "kollision_version.h"

#include <KAboutData>
#include <KCrash>
#include <KDBusService>
#include <KLocalizedString>
#define HAVE_KICONTHEME __has_include(<KIconTheme>)
#if HAVE_KICONTHEME
#include <KIconTheme>
#endif

#define HAVE_STYLE_MANAGER __has_include(<KStyleManager>)
#if HAVE_STYLE_MANAGER
#include <KStyleManager>
#endif
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
#if HAVE_KICONTHEME
    KIconTheme::initTheme();
#endif
    QApplication app(argc, argv);
#if HAVE_STYLE_MANAGER
    KStyleManager::initStyle();
#else // !HAVE_STYLE_MANAGER
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif // defined(Q_OS_MACOS) || defined(Q_OS_WIN)
#endif // HAVE_STYLE_MANAGER
    KLocalizedString::setApplicationDomain(QByteArrayLiteral("kollision"));

    KAboutData aboutData(QStringLiteral("kollision"), i18n("Kollision"),
                         QStringLiteral(KOLLISION_VERSION_STRING),
                         i18n("Casual ball game"),
                         KAboutLicense::GPL,
                         i18n("(c) 2007 Paolo Capriotti"),
                         QString(),
                         QStringLiteral("https://apps.kde.org/kollision"));
    aboutData.addAuthor(i18n("Paolo Capriotti"), QString(), QStringLiteral("p.capriotti@gmail.com"));
    aboutData.addAuthor(i18n("Dmitry Suzdalev"), QString(), QStringLiteral("dimsuz@gmail.com"));
    aboutData.addCredit(i18n("Matteo Guarnieri"), i18n("Original idea"));
    aboutData.addCredit(i18n("Brian Croom"), i18n("Port to use KGameRenderer"));

    KAboutData::setApplicationData(aboutData);
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kollision")));

    KCrash::initialize();

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KDBusService service;

    MainWindow* window = new MainWindow;
    window->show();
    
    return app.exec();
}

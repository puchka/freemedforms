/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2010 by Eric MAEKER, MD (France) <eric.maeker@free.fr>        *
 *  All rights reserved.                                                   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program (COPYING.FREEMEDFORMS file).                   *
 *  If not, see <http://www.gnu.org/licenses/>.                            *
 ***************************************************************************/
/***************************************************************************
 *   Main Developper : Eric MAEKER, <eric.maeker@free.fr>                  *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#include "coreimpl.h"

#include <coreplugin/settings_p.h>
#include <coreplugin/isettings.h>
#include <coreplugin/imainwindow.h>
#include <coreplugin/theme.h>
#include <coreplugin/translators.h>
#include <coreplugin/actionmanager/actionmanager_p.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/contextmanager/contextmanager.h>
#include <coreplugin/contextmanager/contextmanager_p.h>
#include <coreplugin/uniqueidmanager.h>
#include <coreplugin/filemanager.h>
#include <coreplugin/constants_icons.h>

#include <faccountcoreplugin/commandlineparser.h>

#include <translationutils/constanttranslations.h>
#include <utils/log.h>
#include <utils/global.h>
#include <utils/updatechecker.h>

#include <QDir>
#include <QCoreApplication>
#include <QTime>
#include <QSplashScreen>
#include <QFont>
#include <QWidget>
#include <QSplashScreen>

namespace Core {
namespace Internal {
    static CoreImpl *m_instance = 0;
} // namespace Internal
} // namespace Core


using namespace Core;
using namespace Core::Internal;
using namespace Trans::ConstantTranslations;

ICore* ICore::instance()
{
    return m_instance;
}

// instance is created by Core::CorePlugin()
CoreImpl::CoreImpl(QObject *parent) :
        ICore(parent),
        m_Splash(0),
        m_MainWindow(0),
        m_ActionManager(0),
        m_ContextManager(0),
        m_UID(new UniqueIDManager),
        m_User(0)
{
    m_Settings = new SettingsPrivate(this);
    m_Settings->setPath(ISettings::UpdateUrl, Utils::Constants::FREEACCOUNT_UPDATE_URL);

    m_Theme = new ThemePrivate(this);
    m_Theme->setThemeRootPath(m_Settings->path(ISettings::ThemeRootPath));

    m_CommandLine = new CommandLine();
//    m_CommandLine->feedPatientDatas(m_Patient);

    QTime chrono;
    chrono.start();
    bool logChrono = m_CommandLine->value(CommandLine::CL_Chrono).toBool();
    if (logChrono)
        Utils::Log::logTimeElapsed(chrono, "Core", "command line parsing");

    createSplashScreen(m_Theme->splashScreen(Constants::FREEACCOUNT_SPLASHSCREEN));

    // add translators
    messageSplashScreen(tkTr(Trans::Constants::INITIALIZING_TRANSLATIONS));
    m_Translators = new Translators(this);
    m_Translators->setPathToTranslations(m_Settings->path(ISettings::TranslationsPath));
    // Qt
    m_Translators->addNewTranslator("qt");
    // Core Needed Libs
    m_Translators->addNewTranslator(Trans::Constants::CONSTANTS_TRANSLATOR_NAME);
    m_Translators->addNewTranslator("utils");
    m_Translators->addNewTranslator("faccountcoreplugin");

    if (logChrono)
        Utils::Log::logTimeElapsed(chrono, "Core", "translators");

    messageSplashScreen(tkTr(Trans::Constants::STARTING_APPLICATION_AT_1).arg(QDateTime::currentDateTime().toString()));

    m_FileManager = new FileManager(this);
    m_UpdateChecker = new Utils::UpdateChecker(this);

    Utils::Log::addMessage("Core" , tkTr(Trans::Constants::STARTING_APPLICATION_AT_1).arg(QDateTime::currentDateTime().toString()));

    // initialize the settings
    messageSplashScreen(tkTr(Trans::Constants::LOADING_SETTINGS));

    // WINE compatibility (only for testing under ubuntu when crosscompiling)
#ifdef Q_OS_WIN
    // For WINE testings
    if (m_CommandLine->value(Core::CommandLine::CL_RunningUnderWine).toBool()) {
        Utils::Log::addMessage("Core", "Running under Wine environnement.");
        QFont::insertSubstitution("MS Shell Dlg", "Tahoma");
        QFont::insertSubstitution("MS Shell Dlg 2", "Tahoma");
    }
#endif

    foreach(const QString &l, QCoreApplication::libraryPaths()) {
        Utils::Log::addMessage("Core" , tkTr(Trans::Constants::USING_LIBRARY_1).arg(l));
    }

//    m_FormManager = new FormManager(this);

    if (logChrono)
        Utils::Log::logTimeElapsed(chrono, "Core", "managers");

    if (Utils::isRunningOnMac())
        QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);

    // ready
    messageSplashScreen(QCoreApplication::translate("Core", "Core intialization finished..."));

    Utils::Log::addMessage("Core" , QCoreApplication::translate("Core", "Core intialization finished..."));
    if (logChrono)
        Utils::Log::logTimeElapsed(chrono, "Core", "end of core intialization");

    m_instance = this;
}

CoreImpl::~CoreImpl()
{
    delete m_UID;
    delete m_CommandLine;
}

void CoreImpl::createSplashScreen(const QPixmap &pix)
{
    if (!m_Splash) {
        m_Splash = new QSplashScreen(pix);
        QFont ft(m_Splash->font());
        ft.setPointSize(ft.pointSize() - 2);
        ft.setBold(true);
        m_Splash->setFont(ft);
        m_Splash->show();
    }
}

void CoreImpl::finishSplashScreen(QWidget *w)
{
    Q_ASSERT(m_Splash);
    if (m_Splash) {
        m_Splash->finish(w);
        delete m_Splash;
        m_Splash = 0;
    }
}

void CoreImpl::messageSplashScreen(const QString &msg)
{
    Q_ASSERT(m_Splash);
    if (m_Splash)
        m_Splash->showMessage(msg, Qt::AlignLeft | Qt::AlignBottom, Qt::black);
}

QSplashScreen *CoreImpl::splashScreen()  { return m_Splash;}
ActionManager *CoreImpl::actionManager() const { return m_ActionManager; }
ContextManager *CoreImpl::contextManager() const { return m_ContextManager; }
UniqueIDManager *CoreImpl::uniqueIDManager() const { return m_UID; }
ITheme *CoreImpl::theme() const { return m_Theme; }
Translators *CoreImpl::translators() const { return m_Translators; }
ISettings *CoreImpl::settings() const { return m_Settings; }
IMainWindow *CoreImpl::mainWindow() const { return m_MainWindow; }

void CoreImpl::setMainWindow(IMainWindow *win)
{
    // can be called only once
    Q_ASSERT(m_MainWindow==0);
    Q_ASSERT(m_ActionManager==0);
    Q_ASSERT(m_ContextManager==0);
    m_MainWindow = win;
    m_ActionManager = new ActionManagerPrivate(m_MainWindow);
    m_ContextManager = new ContextManagerPrivate(m_MainWindow);
}

//FormManager *CoreImpl::formManager() const { return m_FormManager; }
FileManager *CoreImpl::fileManager() const { return m_FileManager; }

CommandLine *CoreImpl::commandLine() const { return m_CommandLine; }
Utils::UpdateChecker *CoreImpl::updateChecker() const { return m_UpdateChecker; }


bool CoreImpl::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);
    // first time runnning ?
    if (m_Settings->firstTimeRunning()) {
        // show the license agreement dialog
#ifndef LINUX_INTEGRATED
        if (!Utils::defaultLicenceAgreementDialog("", Utils::LicenseTerms::BSD))
            return false;
#endif
        m_Settings->noMoreFirstTimeRunning();
        m_Settings->setLicenseApprovedApplicationNumber(qApp->applicationVersion());
    } else if (m_Settings->licenseApprovedApplicationNumber() != qApp->applicationVersion()) {
        // show the license agreement dialog
#ifndef LINUX_INTEGRATED
        if (!Utils::defaultLicenceAgreementDialog(
                QCoreApplication::translate("Core", "You are running a new version of %1, you need to renew the licence agreement.")
                .arg(qApp->applicationName()),
                Utils::LicenseTerms::BSD))
            return false;
#endif
        m_Settings->setLicenseApprovedApplicationNumber(qApp->applicationVersion());
    }

    return true;
}

void CoreImpl::extensionsInitialized()
{
}


/***************************************************************************
 *   FreeMedicalForms                                                      *
 *   (C) 2008-2010 by Eric MAEKER, MD                                      *
 *   eric.maeker@free.fr                                                   *
 *   All rights reserved.                                                  *
 *                                                                         *
 *   This program is a free and open source software.                      *
 *   It is released under the terms of the new BSD License.                *
 *                                                                         *
 *   Redistribution and use in source and binary forms, with or without    *
 *   modification, are permitted provided that the following conditions    *
 *   are met:                                                              *
 *   - Redistributions of source code must retain the above copyright      *
 *   notice, this list of conditions and the following disclaimer.         *
 *   - Redistributions in binary form must reproduce the above copyright   *
 *   notice, this list of conditions and the following disclaimer in the   *
 *   documentation and/or other materials provided with the distribution.  *
 *   - Neither the name of the FreeMedForms' organization nor the names of *
 *   its contributors may be used to endorse or promote products derived   *
 *   from this software without specific prior written permission.         *
 *                                                                         *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   *
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     *
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS     *
 *   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE        *
 *   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,  *
 *   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  *
 *   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      *
 *   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
 *   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT    *
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN     *
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
 *   POSSIBILITY OF SUCH DAMAGE.                                           *
 ***************************************************************************/
/***************************************************************************
 *   Main Developper : Eric MAEKER, <eric.maeker@free.fr>                  *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <utils/global_exporter.h>

#include <QObject>
#include <QUrl>
#include <QString>
#include <QProgressBar>
QT_BEGIN_NAMESPACE
class QWidget;
class QSettings;
QT_END_NAMESPACE

namespace Utils {
namespace Constants {
    const char* const  FREEMEDFORMS_UPDATE_URL  = "http://www.ericmaeker.fr/FreeMedForms/update-fmf.txt";
    const char* const  FREEDIAMS_UPDATE_URL     = "http://www.ericmaeker.fr/FreeMedForms/update-di.txt";
    const char* const  FREEACCOUNT_UPDATE_URL   = "http://www.ericmaeker.fr/FreeMedForms/update-freeaccount.txt";

    const char* const  S_CHECKUPDATE       = "CheckUpdate";
    const char* const  S_LAST_CHECKUPDATE  = "LastCheckUpdate";
}
}

/**
 * \file updatechecker.h
 * \author Eric MAEKER <eric.maeker@free.fr>
 * \version 0.4.0
 * \date 18 Mar 2010
*/

namespace Utils {
namespace Internal {
class UpdateCheckerPrivate;
}

class UTILS_EXPORT UpdateChecker : public QObject
{
    Q_OBJECT
    friend class Internal::UpdateCheckerPrivate;

public:
    enum {  // Don't change the ordre of this enum
        Check_AtStartup = 0,
        Check_EachWeeks,
        Check_EachMonth,
        Check_EachQuarters,
        Check_Never
    };

     UpdateChecker(QObject *parent = 0);
    ~UpdateChecker();

    bool needsUpdateChecking(QSettings *settings) const;

    bool isChecking() const;
    void check(const QString &url);
    void check(const QUrl &url);
    void cancel();

    bool fileRetreived();
    bool hasUpdate();
    QString lastVersion();
    QString updateText();

    QProgressBar *progressBar(QWidget *parent);

public Q_SLOTS:
    void showUpdateInformations();

Q_SIGNALS:
    void updateFound(const QString &updateText);
    void updateFound();
    void done(bool);

protected:
    void emitSignals();

private:
    Internal::UpdateCheckerPrivate *d;
};
}

#endif  // UPDATECHECKER_H

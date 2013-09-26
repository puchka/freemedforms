/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2012 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
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
 *   Main developers: Eric MAEKER, <eric.maeker@gmail.com>                 *
 *   Contributors:                                                         *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
/*!
 * \class eDRC::Internal::CrEditorDialog
 * \brief short description of class
 *
 * Long description of class
 * \sa eDRC::
 */

#include "creditordialog.h"
#include <edrcplugin/consultresult.h>

#include <translationutils/constants.h>

#include <QDebug>

#include "ui_creditordialog.h"

using namespace eDRC;
using namespace Internal;
using namespace Trans::ConstantTranslations;

namespace eDRC {
namespace Internal {
class CrEditorDialogPrivate
{
public:
    CrEditorDialogPrivate(CrEditorDialog *parent) :
        ui(new Ui::CrEditorDialog),
        q(parent)
    {
        Q_UNUSED(q);
    }
    
    ~CrEditorDialogPrivate()
    {
        delete ui;
    }
    
public:
    Ui::CrEditorDialog *ui;
    ConsultResult _cr;
    
private:
    CrEditorDialog *q;
};
} // namespace Internal
} // end namespace eDRC


/*! Constructor of the eDRC::Internal::CrEditorDialog class */
CrEditorDialog::CrEditorDialog(QWidget *parent) :
    QDialog(parent),
    d(new CrEditorDialogPrivate(this))
{
    d->ui->setupUi(this);
}

/*! Destructor of the eDRC::Internal::CrEditorDialog class */
CrEditorDialog::~CrEditorDialog()
{
    if (d)
        delete d;
    d = 0;
}

/*! Initializes the object with the default values. Return true if initialization was completed. */
bool CrEditorDialog::initialize(const ConsultResult &cr)
{
    d->ui->widget->setConsultResult(cr);
    return true;
}

ConsultResult CrEditorDialog::submit() const
{
    return d->ui->widget->submit();
}

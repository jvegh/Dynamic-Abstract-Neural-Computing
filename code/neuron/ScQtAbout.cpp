/*
** This file is part of gSysC.
**
** gSysC is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** gSysC is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with gSysC with the file ``LICENSE''; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "ScQtAbout.moc"
#include "ScQtAbout.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>


/* 
 *  Constructs a scQAbout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ScQtAbout::ScQtAbout( QWidget* parent/*, const char* name, bool modal, WFlags fl*/ )
    : QDialog( parent/*, name, modal, fl*/ )
{
//    if ( !name )
//	setName( "gsysAbout" );
//    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 500, 330 ) );

    languageChange();
    resize( QSize(500, 400).expandedTo(minimumSizeHint()) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ScQtAbout::~ScQtAbout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ScQtAbout::languageChange()
{
//    setCaption( tr( "scQ" ) );
    textLabel1->setText( tr( "<h3><u>scQ</u></h3><font size='+1'>is a graphical addon for the C++ library SystemC (www.systemc.org).<br>gSysC was created as student project at the<br><br><b>Institute of Computer Engineering</b> of the <br><b>University of L&uuml;beck</b> from<br><b>Christian J. Eibl</b>.<br><br>gSysC provides the possibility to visualize SystemC projects and the improved control of the simulation over graphical frontends. Additionally gSysC provides control windows to watch the used ports and signals. With these windows it is possible to shorten debugger sessons by finding errors much earlier.</font>" ) );
}

#include "moc_ScQtAbout.cpp"

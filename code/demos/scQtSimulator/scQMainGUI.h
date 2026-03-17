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

/*****************************************************************************
    Author: Christian J. Eibl
    last modified: 2005-01-31
 *****************************************************************************
    Content:
      This class provides the main window of the graphical user interface.
      It is parent class to all other windows.
 *****************************************************************************/



#ifndef GSYS_MAINGUI_H
#define GSYS_MAINGUI_H

#include <qvariant.h>
#include <qdialog.h>

#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

class scQSimulator;
//class gsysHierarchyTree;
//class gsysPortViewer;
class scQAbout;
//class gsysBuffer;

class scQMainGUI : public QDialog
{
    friend class scQMain;
    friend void* startThread(void *stepsVoid);

    Q_OBJECT

public:
    /*
     *  Constructs a gsysMain as a child of 'parent', with the
     *  name 'name' and widget flags set to 'f'.
     *
     *  The dialog will by default be modeless, unless you set 'modal' to
     *  TRUE to construct a modal dialog.
     */
    scQMainGUI( QWidget* parent = 0/*, const char* name = 0, bool modal = false, WFlags fl = 0*/);

private:

    scQSimulator * simulator;
//    gsysHierarchyTree * hierTree;
//    gsysPortViewer * portViewer;
//    gsysBuffer* bufferWdw;
    scQAbout* aboutWdw;
    void openAboutWdw();
//    void openBufferWdw();

public:

    void openMainWdw();
    void startSimulator(int steps=1000);
    /*
    void startHierarchyTree();
    void startPortViewer();
    gsysPortViewer* getPortViewer();
*/
    scQSimulator* createSimulator();
 //   gsysHierarchyTree* createHierTree();
 //   gsysBuffer* createBufferWdw();

    /*
     *  Destroys the object and frees any allocated resources
     */
    ~scQMainGUI();
      

    QPushButton* pushButton4;
    QPushButton* pushButton5;
    QPushButton* pushButton8;
    QPushButton* pushButton9;
    QPushButton* pushButton6;

public slots:
/*    void pushButton4_clicked();    // start simulator
    void pushButton5_clicked();    // Hierarchy-Viewer
    void pushButton8_clicked();    // Port observation
    void pushButton9_clicked();    // Buffer variables
    void pushButton6_clicked();    // About dialog
*/
protected:
    QVBoxLayout* scQMainGUILayout;

protected slots:
    /*
     *  Sets the strings of the subwidgets using the current
     *  language.
     */
    void languageChange();
  
};


#endif

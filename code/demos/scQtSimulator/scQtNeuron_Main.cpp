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

#include <QApplication>
#include "scQtNeuron_MainWindow.h"
//#include "scQMainGUI.h"
// /#include "mainwindow.h"


//#include "gsysConnection.h"
//#include "gsysHierarchy.h"
#include "scQtSimulator.h"
//#include "gsysHierarchyTree.h"
#include "scQMainGUI.h"
//#include "gsysRegister.h"
//#include "gsysBuffer.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 *   thread function to simulate without getting all ressources 
 *   from the simulation controller frontend
 *   In this way it's possible to simulate and still have the possibility to use the frontends,
 *   e.g. for stopping some simulation process.
 */
pthread_mutex_t run_mutex;
void* startThread(void *stepsVoid)
{
/*  int steps = *((int*) stepsVoid);
  scQSimulator* simWdw = (new scQtNeuron_MainWindow())->getSimulator();
  for (int i=1; i<=steps; i++)
  {
    pthread_mutex_lock(&run_mutex);
    simWdw->start(1);
    if (simWdw->autoStep->isChecked())
    {
      usleep( 100000 * 2);//;simWdw->intervalSpin->value() );
    }
    pthread_mutex_unlock(&run_mutex);
    if (i==steps-1) simWdw->stop();
  }
  pthread_exit(NULL);
  simWdw = 0;
*/
    return NULL; //!!
}
		      

  // global variables of the class scQMain
  //   pointers to the main window and to the registration class
//  Ui::scQtNeuron_MainWindow * scQtNeuron_MainWindow::mainWindow = 0;
//  scQRegister * scQMain::regObj = 0;

/* 
 *   constructor
 */
/*  scQMain::scQMain()
  {
  }
*/
  int guiMode(QApplication &app) {
      scQtNeuron_MainWindow m;

#ifdef Q_OS_WASM
          // In the WASM build, we'll just want a full-screen application that can't be
      // dragged or resized by the user.
      m.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
#endif

      // The following sequence of events manages to successfully start the
      // application as maximized, with the processor at a reasonable size. This has
      // been found to be specially a problem on windows.
      m.resize(60, 100);
      m.show();
      int Steps = 10;
      startThread(&Steps);
// *      m.setWindowState(Qt::WindowMaximized);
 //     QTimer::singleShot(100, &m, [&m] { m.fitToView(); });

      return app.exec();
  }


  #if 0
/*
 *   get registration object (gsysRegister object)
 */
  gsysRegister* gsysMain::getRegModule()
  {
    if (regObj == 0)
      regObj = new gsysRegister();
    return regObj;
  }
  
/*
 *   get pointer to the window with user-defined functionalities
 */
  gsysBuffer* gsysMain::getBufferWdw()
  {
    return getMainWindow()->createBufferWdw();
  }
  
/*
 *   get hierarchyTree window
 */
  gsysHierarchyTree* gsysMain::getHierarchyTree()
  {
    if (mainWindow == 0)
      return getMainWindow()->createHierTree();
    else
      return mainWindow->createHierTree();
  }
#endif

/*
 *   create and open main window, means start gSysC frontends
 */
  /*
  void scQMain::createGUI()
  {
    if (mainWindow==0)
    {
      mainWindow = new scQMainGUI();
//      getRegModule()->regFinished();
    }
  }
*/
  

  int sc_main(int argc, char* argv[])
  {
#if 0
  Q_INIT_RESOURCE(icons);
  Q_INIT_RESOURCE(examples);
  Q_INIT_RESOURCE(layouts);
  Q_INIT_RESOURCE(fonts);
#endif
      QApplication app(argc, argv);
      QCoreApplication::setApplicationName("scQtNeuron");

#if 0
  QCommandLineParser parser;
  Ripes::CLIModeOptions options;
  initParser(parser, options);
  QString err;
  switch (parseCommandLine(parser, err)) {
  case CommandLineError:
    std::cerr << "ERROR: " << err.toStdString() << std::endl;
    parser.showHelp();
    return 0;
  case CommandLineHelpRequested:
    parser.showHelp();
    return 0;
  case CommandLineGUI:
    return guiMode(app);
  case CommandLineCLI:
    return CLIMode(parser, options);
  }
#endif
      return guiMode(app);
//      return 0;
  }

/*
 *   method to refresh the progressBar visualization
 *   'id' is the identification number of the variable to visualize
 *   'value' is the new value
 *   'maxValue' gives the new maximum of the progressbar
 */
/*  void gsysMain::refreshBuffer(void* id, int value, int maxValue)
  { 
    getBufferWdw()->refreshBuffer(id,value,maxValue);
  }
*/

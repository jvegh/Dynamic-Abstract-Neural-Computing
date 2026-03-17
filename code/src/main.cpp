#include <QApplication>
#include <QResource>
//#include <systemc.h>

#include "MyWindow.h"
#include "ScQtNeuron_MainWindow.h"

int guiMode(QApplication &app) {
    ScQtNeuron_MainWindow m;
//    MyWindow    m;

#ifdef Q_OS_WASM
    // In the WASM build, we'll just want a full-screen application that can't be
    // dragged or resized by the user.
    m.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
#endif

    // The following sequence of events manages to successfully start the
    // application as maximized, with the processor at a reasonable size. This has
    // been found to be specially a problem on windows.
    m.resize(800, 600);
    m.show();
    //  m.showMaximized();
    //  m.setWindowState(Qt::WindowMaximized);
    //QTimer::singleShot(100, &m, [&m] { m.fitToView(); });
   return app.exec();
}

int sc_main(int argc, char *argv[])
{
    sc_set_time_resolution(1,SC_US);
 /*   Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(examples);
    Q_INIT_RESOURCE(layouts);
    Q_INIT_RESOURCE(fonts);
*/
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("ScQtNeuron");
    MyWindow m;
    return guiMode(app);
    m.resize(800, 600);
    m.show();
    /*
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
*/
    /*  case CommandLineCLI:
    return CLIMode(parser, options);
  }
*/

 //   w.show();

    return app.exec();
}


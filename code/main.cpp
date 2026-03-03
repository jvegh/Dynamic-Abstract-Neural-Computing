#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QMessageBox>
#include <QResource>
#include <QTimer>
#include <iostream>
#include <systemc>

//#include "src/cli/clioptions.h"
//#include "src/cli/clirunner.h"
#include "ScQtNeuron_MainWindow.h"
//#include "src/mainwindow.h"
//#include "src/ripes_lib_autogen/include/ui_mainwindow.h"

using namespace std;
using namespace sc_core;

/*void initParser(QCommandLineParser &parser, Ripes::CLIModeOptions &options) {
  QString helpText =
      "The command line interface allows for assembling/compiling/executing a "
      "\n"
      "program on an arbitrary processor model and subsequent reporting of \n"
      "execution telemetry.\nCommand line mode is enabled when the '--mode "
      "sh' argument is provided.";

  helpText.prepend("Ripes command line interface.\n");
  parser.setApplicationDescription(helpText);
  QCommandLineOption modeOption("mode", "Ripes mode [gui, cli]", "mode", "gui");
  parser.addOption(modeOption);
  Ripes::addCLIOptions(parser, options);
}

enum CommandLineParseResult {
  CommandLineError,
  CommandLineHelpRequested,
  CommandLineGUI,
  CommandLineCLI
};

CommandLineParseResult parseCommandLine(QCommandLineParser &parser,
                                        QString &errorMessage) {
  const QCommandLineOption helpOption = parser.addHelpOption();
  if (!parser.parse(QCoreApplication::arguments())) {
    errorMessage = parser.errorText();
    return CommandLineError;
  }

  if (parser.isSet(helpOption))
    return CommandLineHelpRequested;

  if (parser.value("mode") == "gui")
    return CommandLineGUI;
  else if (parser.value("mode") == "cli")
    return CommandLineCLI;
  else {
    errorMessage = "Invalid mode: " + parser.value("mode");
    return CommandLineError;
  }
}
*/
int guiMode(QApplication &app) {

    ScQtNeuron_MainWindow m;

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

/*
int CLIMode(QCommandLineParser &parser, Ripes::CLIModeOptions &options) {
  QString err;
  if (!Ripes::parseCLIOptions(parser, err, options)) {
    std::cerr << "ERROR: " << err.toStdString() << std::endl;
    parser.showHelp();
    return 0;
  }
  return Ripes::CLIRunner(options).run();
}
*/
int sc_main(int argc, char **argv) {
    sc_set_time_resolution(1,SC_US);
  Q_INIT_RESOURCE(icons);
//  Q_INIT_RESOURCE(examples);
//  Q_INIT_RESOURCE(layouts);
  Q_INIT_RESOURCE(fonts);

  QApplication app(argc, argv);
  QCoreApplication::setApplicationName("NeuronScQt");
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
*/    return guiMode(app);
/*  case CommandLineCLI:
    return CLIMode(parser, options);
  }
*/
  return 0;
}

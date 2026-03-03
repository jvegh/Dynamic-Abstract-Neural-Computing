#include "ScQtNeuron_MainWindow.h"
#include "ui_ScQtNeuron_MainWindow.h"
//#include "ScQtNeuron_ParameterWindow.h"
#include <QMdiSubWindow>
#include <QtCore>
#include <QFileSystemModel>
#include <QCloseEvent>
#include <QStatusBar>
#include <QSystemTrayIcon>
//#include "QStuff.h"
//#include "Stuff.h"
//#include "edittab.h"
#include "../version/version.h"

extern struct SystemDirectories Directories;

ScQtNeuron_MainWindow::ScQtNeuron_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScQtNeuron_MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: light grey;");
//??    menubar->setStyleSheet("menu-color: grey;");
    //??Neuron::NeuronHandler::get();
    QMainWindow::setWindowIcon(QIcon(":/icons/neurer.png"));

    // Initialize fonts
    QFontDatabase::addApplicationFont(
        ":/fonts/Inconsolata/Inconsolata-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Inconsolata/Inconsolata-Bold.ttf");

    // Create tabs
    m_stackedTabs = new QStackedWidget(this);
    ui->centrallayout->addWidget(m_stackedTabs);
    // Setup status bar
    //    setupStatusBar();
        (void)statusBar();

    auto *controlToolbar = addToolBar("Simulator control");
    controlToolbar->setVisible(true); // Always visible


    auto *neuronToolbar = addToolBar("Neuron");
    neuronToolbar->setVisible(false);
    auto *neuronTab = new NeuronTab(controlToolbar, neuronToolbar, this);
    m_stackedTabs->insertWidget(NeuronTabID, neuronTab);

/*
     auto *editToolbar = addToolBar("Edit");
    editToolbar->setVisible(false);
    auto *editTab = new EditTab(editToolbar, this);
    m_stackedTabs->insertWidget(EditTabID, editTab);
    m_tabWidgets[EditTabID] = {editTab, editToolbar};
*/
    // Setup tab bar
    setIconSize(QSize(24,24));
    ui->tabbar->addFancyTab(QIcon(":/icons/neurer.png"), "Neuron");


//    setStyleSheet("border: 1px solid green");
    setWindowTitle( "NeuronScQt main window" );
    setIconSize(QSize(24,24));
    QSystemTrayIcon(this).setIcon( QIcon( ":/images/ESlogo.png" ) );
    setMinimumSize( QSize( 600, 400 ) );
    setupMenus();
//     SetFileMenu();
    /*
    SetProcessMenu();
//    initFileFilterList();
    SetupSystemDirectories(this); // Establish system and user directories
    readSettings(); // Read window-related settings
*/    setupToolBoxes();   // Set up the tool box contents
//    MyNeuron = new NeuronPhysicalTEST("NeuronPhysical");
    m_Simulator_ControlWindow = new SimulatorControlWindow(this);
    m_Simulator_ControlWindow->show();
    m_Simulator_ControlWindow->move(QPoint(600,000)),
    m_AP_Window = new ScQtNeuron_AP_Window(this);
    m_AP_Window->show();
// Not sure if needed
//    connect(m_simulator, &ScQtSimulator::eventHappened, this, &ScQtNeuron_MainWindow::event_happened);
#if 0
//     m_AP_Window->move(QPoint(800,600)),
     m_dVdt_Window = new ScQtNeuron_ParameterWindow(this);
     m_dVdt_Window->show();
#endif
//     m_dVdt_Window->move(QPoint(1000,800)),
    statusBar()->showMessage("Simulator is ready to work",2000);
}

ScQtNeuron_MainWindow::~ScQtNeuron_MainWindow()
{
    delete ui;
}

void ScQtNeuron_MainWindow::fitToView() {
//    statusBar()->showMessage("Timer", 800);
//    QTimer::singleShot(1000, this, &fitToView(); });
/*    static_cast<ProcessorTab *>(m_tabWidgets.at(ProcessorTabID).tab)
    ->fitToScreen();
*/
}

void ScQtNeuron_MainWindow::setupStatusBar() {
    statusBar()->showMessage("");
/*
    // Setup selected processor & ISA info status widget (right-aligned =>
    // permanent)
    setupPermanentStatusWidget(ProcessorInfo);
    auto updateProcessorInfo = [=] {
        const auto &desc =
            ProcessorRegistry::getDescription(ProcessorHandler::getID());
        QString status = "Processor: " + desc.name + "    ISA: " +
                         ProcessorHandler::getProcessor()->implementsISA()->name();
        ProcessorInfoStatusManager::get().setStatusPermanent(status);
    };
    connect(ProcessorHandler::get(), &ProcessorHandler::processorChanged,
            updateProcessorInfo);
    updateProcessorInfo();

    // Setup processorhandler status widget
    setupStatusWidget(Processor);

    // Setup syscall status widget
    setupStatusWidget(Syscall);

    // Setup systemIO status widget
    setupStatusWidget(SystemIO);

    // Setup general info status widget
    setupStatusWidget(General);
*/
}

void ScQtNeuron_MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeClose()) {
//        writeSettings();
//        writeSettings();    // Qrite out window-related settings

        qInfo() << qAppName() << " normally terminated by user";
        event->accept();

/*   CloseSystemFiles(GetAppName() + " simulator GUI version has terminated");
*/    } else {
        event->ignore();
    }
}


bool ScQtNeuron_MainWindow::maybeClose()
{
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("Closing this window will exit the simulator tool.\n"
                        "Are you sure you want exit?"),
                     QMessageBox::Yes  | QMessageBox::Cancel);
        if (ret == QMessageBox::Yes)
    {
      return true;
    }
        else if (ret == QMessageBox::Cancel)
            return false;
    return false;

}

void ScQtNeuron_MainWindow::addTreeRoot(QString name, QString description)
{
/*    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);
    addTreeChild(treeItem, name + "A", "Child_first");
    addTreeChild(treeItem, name + "B", "Child_second");
*/
}

void ScQtNeuron_MainWindow::addTreeChild(QTreeWidgetItem *parent,
                  QString name, QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
}



void ScQtNeuron_MainWindow::setupToolBoxes(void)
{
 /*  QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(model->index(QString('~')));
    ui->dirTreeView->setModel(model);
    ui->dirTreeView->setRootIndex(QString("~/REPO/"));
 //   ui->dirTreeView->setRootIndex(QString(Directories.Home.c_str()));
*/
    QFileSystemModel *dirModel = new QFileSystemModel(this);
//    dirModel->setRootPath(QString(Directories.Home.c_str()));
    dirModel->setRootPath(QString("~/REPO/"));

//    ui->dirTreeView->setModel(dirModel);
//    ui->dirTreeView->setRootIndex(dirModel->setRootPath(Directories.UserData.c_str()+'/'));
//    ui->dirTreeView->setRootIndex(dirModel->setRootPath("~/REPO/"));

}

void ScQtNeuron_MainWindow::setupMenus() {
    // Edit actions
/*
    const QIcon loadIcon = QIcon(":/icons/loadfile.svg");
    auto *loadAction = new QAction(loadIcon, "Load Program", this);
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, &QAction::triggered, this,
            [=] { this->loadFileTriggered(); });
    m_ui->menuFile->addAction(loadAction);
    m_ui->menuFile->addSeparator();

    auto *examplesMenu = ui->menuFile->addMenu("Load Example...");
    setupExamplesMenu(examplesMenu);

    m_ui->menuFile->addSeparator();

    const QIcon saveIcon = QIcon(":/icons/save.svg");
    auto *saveAction = new QAction(saveIcon, "Save File", this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this,
            &ScQtNeuron_MainWindow::saveFilesTriggered);
    connect(static_cast<EditTab *>(m_tabWidgets.at(EditTabID).tab),
            &EditTab::editorStateChanged, saveAction,
            [saveAction](bool enabled) { saveAction->setEnabled(enabled); });
    m_ui->menuFile->addAction(saveAction);

    const QIcon saveAsIcon = QIcon(":/icons/saveas.svg");
    auto *saveAsAction = new QAction(saveAsIcon, "Save File As...", this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this,
            &ScQtNeuron_MainWindow::saveFilesAsTriggered);
    connect(static_cast<EditTab *>(m_tabWidgets.at(EditTabID).tab),
            &EditTab::editorStateChanged, saveAction,
            [saveAsAction](bool enabled) { saveAsAction->setEnabled(enabled); });
    m_ui->menuFile->addAction(saveAsAction);
    m_ui->menuFile->addSeparator();

    const QIcon exitIcon = QIcon(":/icons/cancel.svg");
    auto *exitAction = new QAction(exitIcon, "Exit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &ScQtNeuron_MainWindow::close);
    m_ui->menuFile->addAction(exitAction);
*/

//    auto *wikiAction = new QAction(QIcon(":/icons/info.svg"), "Wiki", this);
/*    saveAction->setShortcut(QKeySequence::Save);
    connect(wikiAction, &QAction::triggered, this,
            &MainWindow::saveFilesTriggered);
*/
    connect(ui->actionOpen_wiki, &QAction::triggered, this, &ScQtNeuron_MainWindow::wiki);
    connect(ui->actionVersion, &QAction::triggered, this, &ScQtNeuron_MainWindow::version);

}

void ScQtNeuron_MainWindow::setupExamplesMenu(QMenu *parent) {
/*    const auto assemblyExamples =
        QDir(":/examples/assembly/").entryList(QDir::Files);
    auto *assemblyMenu = parent->addMenu("Assembly");
    if (!assemblyExamples.isEmpty()) {
        for (const auto &fileName : assemblyExamples) {
            assemblyMenu->addAction(fileName, this, [=] {
                LoadFileParams parms;
                parms.filepath = QString(":/examples/assembly/") + fileName;
                parms.type = SourceType::Assembly;
                static_cast<EditTab *>(m_tabWidgets.at(EditTabID).tab)
                    ->loadExternalFile(parms);
                clearSaveFile();
            });
        }
    }

    const auto cExamples = QDir(":/examples/C/").entryList(QDir::Files);
    auto *cMenu = parent->addMenu("C");
    if (!cExamples.isEmpty()) {
        for (const auto &fileName : cExamples) {
            cMenu->addAction(fileName, this, [=] {
                LoadFileParams parms;
                parms.filepath = QString(":/examples/C/") + fileName;
                parms.type = SourceType::C;
                static_cast<EditTab *>(m_tabWidgets.at(EditTabID).tab)
                    ->loadExternalFile(parms);
                clearSaveFile();
            });
        }
    }

    const auto ELFExamples = QDir(":/examples/ELF/").entryList(QDir::Files);
    auto *elfMenu = parent->addMenu("ELF (precompiled C)");
    if (!ELFExamples.isEmpty()) {
        for (const auto &fileName : ELFExamples) {
            elfMenu->addAction(fileName, this, [=] {
                // ELFIO Cannot read directly from the bundled resource file, so copy
                // the ELF file to a temporary file before loading the program.
                QTemporaryFile *tmpELFFile =
                    QTemporaryFile::createNativeFile(":/examples/ELF/" + fileName);
                if (!tmpELFFile->open()) {
                    QMessageBox::warning(this, "Error",
                                         "Could not create temporary ELF file");
                    return;
                }

                LoadFileParams parms;
                parms.filepath = tmpELFFile->fileName();
                parms.type = SourceType::ExternalELF;
                static_cast<EditTab *>(m_tabWidgets.at(EditTabID).tab)
                    ->loadExternalFile(parms);
                clearSaveFile();
                tmpELFFile->remove();
            });
        }
    }
*/
}

void ScQtNeuron_MainWindow::wiki() {
    QDesktopServices::openUrl(QUrl(QString(
        "https://jvegh.github.io/DynamicAbstractNeuralComputing")));
}

void ScQtNeuron_MainWindow::version() {
    QMessageBox aboutDialog(this);
    aboutDialog.setWindowIcon(QIcon(":/icons/neurer.png"));

    aboutDialog.setText("NeuRipes :" + getRipesVersion());
    aboutDialog.exec();
}

#if 0
// Not sure if needed
void ScQtNeuron_MainWindow::event_happened()
{
    QMessageBox aboutDialog(this);
    aboutDialog.setWindowIcon(QIcon(":/icons/neurer.png"));

    aboutDialog.setText("Event happened in Main");
    aboutDialog.exec();
}
#endif


#include "moc_ScQtNeuron_MainWindow.cpp"

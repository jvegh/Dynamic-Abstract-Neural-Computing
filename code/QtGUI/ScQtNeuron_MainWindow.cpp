/** @file ScQtNeuron_MainWindow.cpp
 *  @brief The main window for the SystemC-based neuron simulator, using Qt's stuff
 *  Ideas taken from  http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
 *  https://www.researchgate.net/publication/228972213_gSysC_A_graphical_front_end_for_SystemC
 *  and https://github.com/mortbopet/Ripes
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/
#include "ScQtNeuron_MainWindow.h"
#include "ui_ScQtNeuron_MainWindow.h"
#include <QMdiSubWindow>
#include <QtCore>
#include <QFileSystemModel>
#include <QCloseEvent>
#include <QStatusBar>
#include <QSystemTrayIcon>
//#include "QStuff.h"
//#include "Stuff.h"
#include "neurontab.h"
#include "gitversion.h"
#include "ui_neurontab.h"

extern struct SystemDirectories Directories;

ScQtNeuron_MainWindow::ScQtNeuron_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScQtNeuron_MainWindow)
{
    ui->setupUi(this);
     this->setStyleSheet("color: Navy;"
                            "border-color:  LightGray;"
                            "background-color:  LightGray;"
       "border: 1px solid white;"
    "border-width: 1px;"
    "border-style: solid;"
    "border-radius: 4px;");
   //??Neuron::NeuronHandler::get();
    QMainWindow::setWindowIcon(QIcon(":/icons/neurer.png"));

    // Initialize fonts
    QFontDatabase::addApplicationFont(
        ":/fonts/Inconsolata/Inconsolata-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Inconsolata/Inconsolata-Bold.ttf");

    setMinimumSize(550, 450);
    setMaximumSize(800, 600);
    resize(641, 481);

//    MyNeuron = new NeuronPhysicalTEST("NeuronPhysical");
    createExamples();   // Must create a nneuron for the windows

    // Be sure this event remains all the time in the stack
 //   MyNeuron->EVENT_GenComp.Failed.notify(10000,sc_core::SC_SEC);

    // Create tabs
    m_stackedTabs = new QStackedWidget(this);
    ui->centrallayout->addWidget(m_stackedTabs);
    // Setup status bar
        setupStatusBar();
    //    (void)statusBar();

    auto *controlToolbar = addToolBar("Simulator control");
    controlToolbar->setVisible(true); // Always visible


    auto *neuronToolbar = addToolBar("Neuron");
    neuronToolbar->setVisible(false);
    m_neuronTab = new NeuronTab(controlToolbar,
        neuronToolbar,
                                this);
    m_stackedTabs->insertWidget(NeuronTabID, m_neuronTab);

    connect(m_neuronTab->ui->startButton, &QPushButton::clicked, this,
            &ScQtNeuron_MainWindow::on_startButton_clicked);

    connect(m_neuronTab->ui->breakButton, &QPushButton::clicked, this,
            &ScQtNeuron_MainWindow::on_breakButton_clicked);

    connect(m_neuronTab->ui->resetButton, &QPushButton::clicked, this,
            &ScQtNeuron_MainWindow::on_resetButton_clicked);
    connect(m_neuronTab->ui->restartButton, &QPushButton::clicked, this,
            &ScQtNeuron_MainWindow::on_restartButton_clicked);

    connect(m_neuronTab->ui->DisplayReversedBox, &QCheckBox::clicked, this,
            &ScQtNeuron_MainWindow::on_ReversedDisplayModeClicked);
     //
    // The thread and the simulator are created in the constructor so it is always safe to delete them.
    //
    m_thread = new QThread();
    m_Simulator = new ScQtSimulator();
    m_Simulator->moveToThread(m_thread);
    connect(m_thread, SIGNAL(started()), m_Simulator, SLOT(mainLoop()));
    connect(m_Simulator, SIGNAL(finished()), m_thread, SLOT(quit()), Qt::DirectConnection);

    // Receive SC event from the simulator
    qDebug()<<"Starting thread in Thread "<<this->QObject::thread()->currentThreadId();
    m_thread->start();
    connect(m_Simulator,SIGNAL(eventHappened()), this, SLOT(on_eventHappened()));

    // Setup tab bar
    setIconSize(QSize(32,32));
    ui->tabbar->addFancyTab(QIcon(":/icons/neurer.png"), "Neuron");


    setWindowTitle( "NeuronScQt main window" );
//    QSystemTrayIcon(this).setIcon( QIcon( ":/images/ESlogo.png" ) );
    setupMenus();
//     SetFileMenu();
    /*
    SetProcessMenu();
    SetupSystemDirectories(this); // Establish system and user directories
    readSettings(); // Read window-related settings
    setupToolBoxes();   // Set up the tool box contents
*/
    m_VoltageWindow = new VoltageWindow(m_Simulator, MyNeuron);
    m_VoltageWindow->show();
#if 0
    m_CurrentWindow = new CurrentWindow(m_Simulator, MyNeuron);
    m_CurrentWindow->show();
#endif
    m_GradientWindow = new GradientWindow(m_Simulator, MyNeuron);
    m_GradientWindow->show();
    m_PhasePlotWindow = new PhasePlotWindow(m_Simulator, MyNeuron);
    m_PhasePlotWindow->show();
    m_PhasePlotWindow->DisplayMode_Set(false);
    m_neuronTab->ui->DisplayReversedBox->setCheckState( Qt::CheckState(m_PhasePlotWindow->DisplayMode_Get()));
    on_ExamplesFailed_AP(); // Must be after creating the windows
}
/*
     auto *editToolbar = addToolBar("Edit");
    editToolbar->setVisible(false);
    auto *editTab = new EditTab(editToolbar, this);
    m_stackedTabs->insertWidget(EditTabID, editTab);
    m_tabWidgets[EditTabID] = {editTab, editToolbar};
*/

ScQtNeuron_MainWindow::~ScQtNeuron_MainWindow()
{
    delete ui;
}

void ScQtNeuron_MainWindow::replot()
{
    m_PhasePlotWindow->replot();
    m_VoltageWindow->replot();
 //   m_CurrentWindow->replot();
    m_GradientWindow->replot();
}


void ScQtNeuron_MainWindow::setupStatusBar()
{
    statusBar()->showMessage("");
}

void ScQtNeuron_MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeClose()) {
//        writeSettings();    // Write out window-related settings

        qInfo() << qAppName() << " normally terminated by user";
        event->accept();

/*   CloseSystemFiles(GetAppName() + " simulator GUI version has terminated");
*/    } else {
        event->ignore();
    }
}

void ScQtNeuron_MainWindow::on_startButton_clicked()
{
    m_neuronTab->ui->DisplayReversedBox->setEnabled(false);
    m_StepNumber = m_neuronTab->ui->StepNumberBox->value();
    m_FinalTime = sc_core::sc_time_stamp() + sc_core::sc_time(m_neuronTab->ui->StepTimeBox->value(),sc_core::SC_US);
    MyNeuron->RushinParameters_Set(0,m_neuronTab->ui->RushinAmplitudeSlider->value()*1000);
    MyNeuron->RushinParameters_Set(1,m_neuronTab->ui->RushinASlider->value()/1000.);
    MyNeuron->RushinParameters_Set(2,m_neuronTab->ui->RushinBSlider->value()/1000.);
    MyNeuron->MembraneParameters_Set(0, m_neuronTab->ui->MembraneTauSlider->value()/1000.);
    MyNeuron->MembraneParameters_Set(1, m_neuronTab->ui->MembraneRSlider->value()/1000.);

    m_Simulator->requestMethod(ScQtSimulator::Method_SingleSteps);
    m_terminated = false;
}

void ScQtNeuron_MainWindow::on_MakeSimulationStep()
{
    m_Simulator->requestMethod(ScQtSimulator::Method_SingleSteps);
}

void ScQtNeuron_MainWindow::on_breakButton_clicked()
{
    m_Simulator->abort(); m_terminated = true;
}

void ScQtNeuron_MainWindow::on_restartButton_clicked()
{
    m_Simulator->abort();
    m_terminated = true;
}

void ScQtNeuron_MainWindow::on_resetButton_clicked()
{
//    m_Simulator->abort();
    m_Simulator->reset();
    displayTime_Reset();
    MyNeuron->Initialize_Do();

    m_neuronTab->ui->DisplayReversedBox->setEnabled(true);

    delete m_PhasePlotWindow;
    m_PhasePlotWindow = new PhasePlotWindow(m_Simulator, MyNeuron);
    m_PhasePlotWindow->show();
    m_PhasePlotWindow->DisplayMode_Set(false);

//    m_PhasePlotWindow->Reset();
//    m_VoltageWindow->Reset();
    delete m_VoltageWindow;
    m_VoltageWindow = new VoltageWindow(m_Simulator, MyNeuron);
    m_VoltageWindow->show();


//    m_CurrentWindow->Reset();
//    m_GradientWindow->Reset();
    delete m_GradientWindow;
    m_GradientWindow = new GradientWindow(m_Simulator, MyNeuron);
    m_GradientWindow->show();

//    on_eventHappened();
}


void ScQtNeuron_MainWindow::on_ReversedDisplayModeClicked()
{
    m_PhasePlotWindow->DisplayMode_Set(m_neuronTab->ui->DisplayReversedBox->isChecked());
    m_StepNumber = m_neuronTab->ui->DisplayReversedBox->isChecked();
    m_PhasePlotWindow->setupDataPlot();
}
// This routine is executed when after executing an Sc event, the control returns to the display
void ScQtNeuron_MainWindow::on_eventHappened()
{
            BENCHMARK_TIME_BEGIN(&m_display_t,&m_display_x);    // Begin display time benchmarking here
    // The functionality moved to here to benchmark the display time
     m_PhasePlotWindow->displayDataSlot();
    m_VoltageWindow->displayDataSlot();
    m_GradientWindow->displayDataSlot();
    m_neuronTab->ui->SimulatedTimeValue->setText(QString(sc_time_String_Get(m_Simulator->scTime_Get()).c_str()));
    m_neuronTab->ui->UserTimeValue->setText(QString(time_String_Get(m_Simulator->userTime_Get(),CLOCK_TIME_UNIT_S,1,7).c_str()));
    m_neuronTab->ui->ProcessorTimeValue->setText(QString(time_String_Get(m_Simulator->systemTime_Get()/1000.,CLOCK_TIME_UNIT_S,3,7).c_str()));
    m_neuronTab->ui->DisplayTimeValue->setText(QString(time_String_Get(displayTime_Get()/1000/1000.,CLOCK_TIME_UNIT_S,2,7).c_str()));
    if ( MyNeuron->EVENT_GenComp.RelaxingEnd.triggered() ) {
        QMessageBox::warning(this, tr("ScQtSimulator"),
                                   tr("Simulation of a single AP successfully terminated\n"
                                      "Maybe you want to make screenshots"),
                                   QMessageBox::Yes );
        m_Simulator->abort();
        m_terminated = true;
    }
    if((
        (m_neuronTab->ui->timeMode->isChecked() && (m_FinalTime > sc_core::sc_time_stamp()))
        || (m_neuronTab->ui->stepMode->isChecked() && (m_StepNumber-->0))
        || (m_neuronTab->ui->continuousMode->isChecked())
        )
        && !m_terminated && (!m_Simulator->isAborted()) //&& (m_Simulator->isInterrupted())
        )
    {   // Continue execution by issuing one more request
        // Imitate pressing 'Start'
        //m_neuronTab->ui->DisplaySlider->value();
        QTimer::singleShot(0    // Zero means continuation with no delay
                           +m_neuronTab->ui->DisplaySlider->value(), this, SLOT(on_MakeSimulationStep()));
    }
//    m_Simulator->reset();
            BENCHMARK_TIME_END(&m_display_t,&m_display_x,&m_display_s);   // End display time benchmarking here
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

#if 0
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
#endif
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

    auto *wikiAction = new QAction(QIcon(":/icons/info.svg"), "Wiki", this);
/*    saveAction->setShortcut(QKeySequence::Save);
    connect(wikiAction, &QAction::triggered, this,
            &ScQtNeuron_MainWindow::saveFilesTriggered);
*/

    connect(ui->actionOpen_wiki, &QAction::triggered, this, &ScQtNeuron_MainWindow::wiki);
    connect(ui->actionSimple_AP, &QAction::triggered, this, &ScQtNeuron_MainWindow::on_ExamplesSimple_AP);
    connect(ui->actionSimple_AP, &QAction::triggered, this, &ScQtNeuron_MainWindow::on_ExamplesFailed_AP);
    connect(ui->actionVersion, &QAction::triggered, this, &ScQtNeuron_MainWindow::version);

}

void ScQtNeuron_MainWindow::createExamples()
{
    DemoNeuronSingleAP = new DemoSimpleSingleAP("Demo Single AP");
    DemoNeuronFailedAP = new DemoSimpleFailedAP("Demo Failed AP");
    MyNeuron = DemoNeuronFailedAP; // Just to test it
}

void ScQtNeuron_MainWindow::on_ExamplesSimple_AP()
{
    MyNeuron->ClearEvents();
    MyNeuron = DemoNeuronSingleAP; //("Demo Single AP");
    m_GradientWindow->setWindowTitle(QString(MyNeuron->name())+QString(" voltage gradients"));
    std::cerr << MyNeuron->name() << '\n';
    MyNeuron->EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
}

void ScQtNeuron_MainWindow::on_ExamplesFailed_AP()
{
    //    delete MyNeuron;
    MyNeuron->ClearEvents();
    MyNeuron = DemoNeuronFailedAP; //("Demo Failed AP");
    m_GradientWindow->setWindowTitle(QString(MyNeuron->name())+QString(" voltage gradients"));
    MyNeuron->EVENT_GenComp.Initialize.notify(SC_ZERO_TIME);
}


void ScQtNeuron_MainWindow::wiki() {
    QDesktopServices::openUrl(QUrl(QString(
        "https://jvegh.github.io/Dynamic-Abstract-Neural-Computing/manual/ch_Simulation/index.html")));
//        "https://jvegh.github.io/DANCES")));
}

void ScQtNeuron_MainWindow::version() {
    QMessageBox aboutDialog(this);
//    aboutDialog.setWindowIcon(QIcon(":/icons/neurer.png"));

    aboutDialog.setText("NeuronScQt V0.2.6"//+ getGitVersion()
                        );
    aboutDialog.exec();
}


#include "moc_ScQtNeuron_MainWindow.cpp"

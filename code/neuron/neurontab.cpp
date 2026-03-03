
#include <QDir>
#include <QFontMetrics>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QSpinBox>
#include <QTemporaryFile>
//#include "ScQtNeuron_Plot_AP.h"
//#include "ScQtNeuron_MainWindow.h"
//#include "consolewidget.h"

//#include "syscall/systemio.h"
//#include "neuronhandler.h"
#include "neurontab.h"
#include "ui_neurontab.h"

class ScQtNeuron_MainWindow;
NeuronTab::NeuronTab(QToolBar *controlToolbar,
                           QToolBar *additionalToolbar, QMainWindow *parent)
    : RipesTab(additionalToolbar, parent) {
    m_ui = new Ui::NeuronTab;
  m_ui->setupUi(this);

/*  connect(NeuronHandler::get(), &ProcessorHandler::procStateChangedNonRun,
          this, &ProcessorTab::updateStatistics);
  connect(ProcessorHandler::get(), &ProcessorHandler::procStateChangedNonRun,
          this, &ProcessorTab::updateInstructionLabels);

*/
  setupSimulatorActions(controlToolbar);

  // Setup statistics update timer - this timer is distinct from the
  // ProcessorHandler's update timer, given that it needs to run during
  // 'running' the processor.

  m_statUpdateTimer = new QTimer(this);
#if 0
  m_statUpdateTimer->setInterval(
      1000.0 / RipesSettings::value(RIPES_SETTING_UIUPDATEPS).toInt());
  connect(m_statUpdateTimer, &QTimer::timeout, this,
          &NeuronTab::updateStatistics);
  connect(RipesSettings::getObserver(RIPES_SETTING_UIUPDATEPS),
          &SettingObserver::modified, m_statUpdateTimer, [=] {
            m_statUpdateTimer->setInterval(
                1000.0 /
                RipesSettings::value(RIPES_SETTING_UIUPDATEPS).toInt());
          });

  // Connect changes in VSRTL reversible stack size to checking whether the
  // simulator is reversible
  connect(RipesSettings::getObserver(RIPES_SETTING_REWINDSTACKSIZE),
          &SettingObserver::modified, m_reverseAction, [=](const auto &) {
            m_reverseAction->setEnabled(m_vsrtlWidget->isReversible());
          });

  // Connect the global reset request signal to reset()
  connect(NeuronHandler::get(), &NeuronHandler::neuronReset, this,
          &NeuronTab::reset);
  connect(NeuronHandler::get(), &NeuronHandler::exit, this,
          &NeuronTab::neuronFinished);
  connect(NeuronHandler::get(), &NeuronHandler::runFinished, this,
          &NeuronTab::runFinished);
  connect(NeuronHandler::get(), &NeuronHandler::stopping, this,
          &NeuronTab::pause);
#endif
   /*
  // Make processor view stretch wrt. right side tabs
  m_ui->viewSplitter->setStretchFactor(0, 1);
  m_ui->viewSplitter->setStretchFactor(1, 0);
*/
//  m_ui->VoltagePlot = new ScQtNeuronVoltagePlot(parent);

  // Initially, no file is loaded, disable toolbuttons
  enableSimulatorControls();
}

/*
void NeuronTab::loadLayout(const Layout &layout) {
  if (layout.name.isEmpty() || layout.file.isEmpty())
    return; // Not a valid layout

  if (layout.stageLabelPositions.size() !=
      ProcessorHandler::getProcessor()->structure().numStages()) {
    Q_ASSERT(false &&
             "A stage label position must be specified for each stage");
  }

  // cereal expects the archive file to be present standalone on disk, and
  // available through an ifstream. Copy the resource layout file (bundled
  // within the binary as a Qt resource) to a temporary file, for loading the
  // layout.
  const auto &layoutResourceFilename = layout.file;
  QFile layoutResourceFile(layoutResourceFilename);
  QTemporaryFile *tmpLayoutFile =
      QTemporaryFile::createNativeFile(layoutResourceFile);
  if (!tmpLayoutFile->open()) {
    QMessageBox::warning(this, "Error",
                         "Could not create temporary layout file");
    return;
  }

  m_vsrtlWidget->getTopLevelComponent()->loadLayoutFile(
      tmpLayoutFile->fileName());
  tmpLayoutFile->remove();

  // Adjust stage label positions
  const auto &parent = m_stageInstructionLabels.at({0, 0})->parentItem();
  for (auto sid : ProcessorHandler::getProcessor()->structure().stageIt()) {
    auto &label = m_stageInstructionLabels.at(sid);
    QFontMetrics metrics(label->font());
    label->setPos(parent->boundingRect().width() *
                      layout.stageLabelPositions.at(sid).x(),
                  metrics.height() * layout.stageLabelPositions.at(sid).y());
  }
}
*/

void NeuronTab::setupSimulatorActions(QToolBar *controlToolbar) {
  const QIcon neuronIcon = QIcon(":/icons/cpu.svg");
  m_selectNeuronAction =
      new QAction(neuronIcon, "Select neuron", this);
  connect(m_selectNeuronAction, &QAction::triggered, this,
          &NeuronTab::neuronSelection);
  controlToolbar->addAction(m_selectNeuronAction);
  controlToolbar->addSeparator();
#if 0
  const QIcon resetIcon = QIcon(":/icons/reset.svg");
  m_resetAction = new QAction(resetIcon, "Reset (F3)", this);
  connect(m_resetAction, &QAction::triggered, this, [=] {
    RipesSettings::getObserver(RIPES_GLOBALSIGNAL_REQRESET)->trigger();
  });
  m_resetAction->setShortcut(QKeySequence("F3"));
  m_resetAction->setToolTip("Reset the simulator (F3)");
  controlToolbar->addAction(m_resetAction);
#endif

  const QIcon clockIcon = QIcon(":/icons/step.svg");
  m_clockAction = new QAction(clockIcon, "Clock (F5)", this);
/*  connect(m_clockAction, &QAction::triggered, this,
          [=] { NeuronHandler::clock(); });
*/
  m_clockAction->setShortcut(QKeySequence("F5"));
  m_clockAction->setToolTip("Clock the circuit (F5)");
  controlToolbar->addAction(m_clockAction);
#if 0
  m_autoClockTimer = new QTimer(this);
  connect(m_autoClockTimer, &QTimer::timeout, this,
          [=] { autoClockTimeout(); });

  const QIcon startAutoClockIcon = QIcon(":/icons/step-clock.svg");
  m_autoClockAction = new QAction(startAutoClockIcon, "Auto clock (F6)", this);
  m_autoClockAction->setShortcut(QKeySequence("F6"));
  m_autoClockAction->setToolTip(
      "Clock the circuit with the selected frequency (F6)");
  m_autoClockAction->setCheckable(true);
  m_autoClockAction->setChecked(false);
  connect(m_autoClockAction, &QAction::toggled, this, &NeuronTab::autoClock);
  controlToolbar->addAction(m_autoClockAction);

  m_autoClockInterval = new QSpinBox(this);
  m_autoClockInterval->setRange(1, 10000);
  m_autoClockInterval->setSuffix(" ms");
  m_autoClockInterval->setToolTip("Auto clock interval");
  connect(m_autoClockInterval, qOverload<int>(&QSpinBox::valueChanged), this,
          [this](int msec) {
            RipesSettings::setValue(RIPES_SETTING_AUTOCLOCK_INTERVAL, msec);
            m_autoClockTimer->setInterval(msec);
          });
  m_autoClockInterval->setValue(
      RipesSettings::value(RIPES_SETTING_AUTOCLOCK_INTERVAL).toInt());
  controlToolbar->addWidget(m_autoClockInterval);
#endif
  const QIcon runIcon = QIcon(":/icons/run.svg");
  m_runAction = new QAction(runIcon, "Run (F8)", this);
  m_runAction->setShortcut(QKeySequence("F8"));
  m_runAction->setCheckable(true);
  m_runAction->setChecked(false);
  m_runAction->setToolTip(
      "Execute simulator without updating UI (fast execution) (F8).\n Running "
      "will stop once the program exits or a "
      "breakpoint is hit.");
  connect(m_runAction, &QAction::toggled, this, &NeuronTab::run);
  controlToolbar->addAction(m_runAction);
#if 0
  // Setup neuron-tab only actions
  m_displayValuesAction = new QAction("Show neuron signal values", this);
  m_displayValuesAction->setCheckable(true);
  connect(m_displayValuesAction, &QAction::toggled, m_vsrtlWidget,
          [=](bool checked) {
            RipesSettings::setValue(RIPES_SETTING_SHOWSIGNALS,
                                    QVariant::fromValue(checked));
            m_vsrtlWidget->setOutputPortValuesVisible(checked);
          });
  m_displayValuesAction->setChecked(
      RipesSettings::value(RIPES_SETTING_SHOWSIGNALS).toBool());
#endif

/*
  m_darkmodeAction = new QAction("Processor darkmode", this);
  m_darkmodeAction->setCheckable(true);
  connect(m_darkmodeAction, &QAction::toggled, m_vsrtlWidget,
          [=](bool checked) {
            RipesSettings::setValue(RIPES_SETTING_DARKMODE,
                                    QVariant::fromValue(checked));
            m_vsrtlWidget->setDarkmode(checked);
          });
  m_darkmodeAction->setChecked(
      RipesSettings::value(RIPES_SETTING_DARKMODE).toBool());
*/
}

void NeuronTab::updateStatistics() {
  static auto lastUpdateTime = std::chrono::system_clock::now();
 /* static long long lastCycleCount =
      NeuronHandler::getNeuron()->getCycleCount();

  const auto timeNow = std::chrono::system_clock::now();
  const auto cycleCount = NeuronHandler::getNeuron()->getCycleCount();
  const auto instrsRetired =
      NeuronHandler::getProcessor()->getInstructionsRetired();
  const auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(
                            timeNow - lastUpdateTime)
                            .count() /
                        1000.0; // in seconds
  const auto cycleDiff = cycleCount - lastCycleCount;

  // Cycle count
  m_ui->cycleCount->setText(QString::number(cycleCount));
  // Instructions retired
  m_ui->instructionsRetired->setText(QString::number(instrsRetired));
  QString cpiText, ipcText;
  if (cycleCount != 0 && instrsRetired != 0) {
    const double cpi =
        static_cast<double>(cycleCount) / static_cast<double>(instrsRetired);
    const double ipc = 1 / cpi;
    cpiText = QString::number(cpi, 'g', 3);
    ipcText = QString::number(ipc, 'g', 3);
  }
  // CPI & IPC
  m_ui->cpi->setText(cpiText);
  m_ui->ipc->setText(ipcText);

  // Clock rate
  const double clockRate = static_cast<double>(cycleDiff) / timeDiff;
  m_ui->clockRate->setText(convertToSIUnits(clockRate) + "Hz");

  // Record timestamp values
  lastUpdateTime = timeNow;
  lastCycleCount = cycleCount;
  */
}

void NeuronTab::pause() {
#if 0
  m_autoClockAction->setChecked(false);
  m_runAction->setChecked(false);
  m_reverseAction->setEnabled(m_vsrtlWidget->isReversible());
#endif
}

//void NeuronTab::fitToScreen() { m_vsrtlWidget->zoomToFit(); }

void NeuronTab::loadNeuronToWidget(const Layout *layout) {
/*
  const bool doPlaceAndRoute = layout != nullptr;
  ProcessorHandler::loadProcessorToWidget(m_vsrtlWidget, doPlaceAndRoute);

  // Construct stage instruction labels
  auto *topLevelComponent = m_vsrtlWidget->getTopLevelComponent();

  m_stageInstructionLabels.clear();
  for (auto laneIt : ProcessorHandler::getProcessor()->structure()) {
    for (unsigned stageIdx = 0; stageIdx < laneIt.second; stageIdx++) {
      StageIndex sid = {laneIt.first, stageIdx};
      auto *stagelabel = new vsrtl::Label(topLevelComponent, "-");
      stagelabel->setPointSize(14);
      m_stageInstructionLabels[sid] = stagelabel;
    }
  }
  if (layout != nullptr) {
    loadLayout(*layout);
  }
  updateInstructionLabels();
  fitToScreen();
  */
}

void NeuronTab::neuronSelection() {
 /*  m_autoClockAction->setChecked(false);

  ProcessorSelectionDialog diag;
  if (diag.exec()) {
    // New processor model was selected
    m_vsrtlWidget->clearDesign();
    m_stageInstructionLabels.clear();
    ProcessorHandler::selectProcessor(diag.getSelectedId(),
                                      diag.getEnabledExtensions(),
                                      diag.getRegisterInitialization());

    // Store selected layout index
    const auto &layouts =
        ProcessorRegistry::getDescription(diag.getSelectedId()).layouts;
    if (auto *layout = diag.getSelectedLayout()) {
      auto layoutIter = std::find(layouts.begin(), layouts.end(), *layout);
      Q_ASSERT(layoutIter != layouts.end());
      const long layoutIndex = std::distance(layouts.begin(), layoutIter);
      RipesSettings::setValue(RIPES_SETTING_PROCESSOR_LAYOUT_ID,
                              static_cast<int>(layoutIndex));
    }

    if (ProcessorHandler::isVSRTLProcessor()) {
      loadProcessorToWidget(diag.getSelectedLayout());
    }
    updateInstructionModel();

    // Retrigger value display action if enabled
    if (m_displayValuesAction->isChecked()) {
      m_vsrtlWidget->setOutputPortValuesVisible(true);
    }
  }
  */
}
 /*
void NeuronTab::updateInstructionModel() {
  auto *oldModel = m_instrModel;
  m_instrModel = new InstructionModel(this);

  // Update the instruction view according to the newly created model
  m_ui->instructionView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_ui->instructionView->setModel(m_instrModel);

  // Only the instruction column should stretch
  m_ui->instructionView->horizontalHeader()->setMinimumSectionSize(1);
  m_ui->instructionView->horizontalHeader()->setSectionResizeMode(
      InstructionModel::Breakpoint, QHeaderView::ResizeToContents);
  m_ui->instructionView->horizontalHeader()->setSectionResizeMode(
      InstructionModel::PC, QHeaderView::ResizeToContents);
  // The "stage" section is _NOT_ resized to contents. Resize to contents is
  // very slow if # of items in the model is large and the contents of the rows
  // change frequently.
  m_ui->instructionView->horizontalHeader()->setSectionResizeMode(
      InstructionModel::Stage, QHeaderView::Interactive);
  auto ivfm = QFontMetrics(m_ui->instructionView->font());
  m_ui->instructionView->horizontalHeader()->resizeSection(
      InstructionModel::Stage,
      ivfm.horizontalAdvance(m_instrModel
                                 ->headerData(InstructionModel::Stage,
                                              Qt::Horizontal, Qt::DisplayRole)
                                 .toString()) *
          1.25);
  m_ui->instructionView->horizontalHeader()->setSectionResizeMode(
      InstructionModel::Instruction, QHeaderView::Stretch);
  // Make the instruction view follow the instruction which is currently present
  // in the first stage of the
  connect(m_instrModel, &InstructionModel::firstStageInstrChanged, this,
          &ProcessorTab::setInstructionViewCenterRow);

  if (oldModel) {
    delete oldModel;
  }
}
 */

void NeuronTab::restart() {
  // Invoked when changes to binary simulation file has been made
  enableSimulatorControls();
}

NeuronTab::~NeuronTab() { delete m_ui; }

void NeuronTab::neuronFinished() {
  // Disallow further clocking of the circuit
  m_clockAction->setEnabled(false);
/*  m_autoClockAction->setChecked(false);
  m_autoClockAction->setEnabled(false);
*/
  m_runAction->setEnabled(false);
  m_runAction->setChecked(false);
}

void NeuronTab::enableSimulatorControls() {
  m_clockAction->setEnabled(true);
/*  m_autoClockAction->setEnabled(true);
  m_runAction->setEnabled(true);
  m_reverseAction->setEnabled(m_vsrtlWidget->isReversible());
  m_resetAction->setEnabled(true);
  m_pipelineDiagramAction->setEnabled(true);
*/
}

#if 0
void NeuronTab::updateInstructionLabels() {
  const auto &proc = ProcessorHandler::getProcessor();
  for (auto sid : ProcessorHandler::getProcessor()->structure().stageIt()) {
    if (!m_stageInstructionLabels.count(sid))
      continue;
    const auto stageInfo = proc->stageInfo(sid);
    auto &instrLabel = m_stageInstructionLabels.at(sid);
    QString instrString;
    if (stageInfo.state != StageInfo::State::None) {
      /* clang-format off */
            switch (stageInfo.state) {
                case StageInfo::State::Flushed: instrString = "nop (flush)"; break;
                case StageInfo::State::Stalled: instrString = "nop (stall)"; break;
                case StageInfo::State::WayHazard: if(stageInfo.stage_valid) {instrString = "nop (way hazard)";} break;
                case StageInfo::State::Unused: instrString = "nop (unused)"; break;
                case StageInfo::State::None: Q_UNREACHABLE();
            }
      /* clang-format on */
      instrLabel->forceDefaultTextColor(Qt::red);
    } else if (stageInfo.stage_valid) {
      instrString = ProcessorHandler::disassembleInstr(stageInfo.pc);
      instrLabel->clearForcedDefaultTextColor();
    }
    instrLabel->setText(instrString);
  }
}
#endif

void NeuronTab::reset() {
//  m_autoClockAction->setChecked(false);
  enableSimulatorControls();
//  SystemIO::printString("\n");
}

#if 0
void ProcessorTab::setInstructionViewCenterRow(int row) {
  const auto view = m_ui->instructionView;
  const auto rect = view->rect();
  int rowTop = view->indexAt(rect.topLeft()).row();
  int rowBot = view->indexAt(rect.bottomLeft()).row();
  rowBot = rowBot < 0 ? m_instrModel->rowCount() : rowBot;

  const int nItemsVisible = rowBot - rowTop;

  // move scrollbar if if is not visible
  if (row <= rowTop || row >= rowBot) {
    auto scrollbar = view->verticalScrollBar();
    scrollbar->setValue(row - nItemsVisible / 2);
  }
}
#endif

void NeuronTab::runFinished() {
  pause();
//  NeuronHandler::checkNeuronFinished();
//  m_vsrtlWidget->sync();
  m_statUpdateTimer->stop();
}

#if 0
void NeuronTab::autoClockTimeout() {
  if (NeuronHandler::checkBreakpoint())
    return;
  NeuronHandler::clock();

}
#endif

#if 0
void NeuronTab::autoClock(bool state) {
  const QIcon startAutoClockIcon = QIcon(":/icons/step-clock.svg");
  const QIcon stopAutoTimerIcon = QIcon(":/icons/stop-clock.svg");
  if (!state) {
    m_autoClockTimer->stop();
    m_autoClockAction->setIcon(startAutoClockIcon);
  } else {
    // Always clock the processor to start with. Afterwards, run
    // autoClockTimeout() which will check if the processor is at a breakpoint.
    // This is to circumvent some annoying cross-thread, eventloop,
    // race-condition-y state setting wrt. when exactly a breakpoint is hit.
    ProcessorHandler::clock();
    m_autoClockTimer->start();
    m_autoClockAction->setIcon(stopAutoTimerIcon);
  }

  // Enable/disable all other actions
  m_selectProcessorAction->setEnabled(!state);
  m_clockAction->setEnabled(!state);
  m_reverseAction->setEnabled(!state);
  m_resetAction->setEnabled(!state);
  m_displayValuesAction->setEnabled(!state);
  m_pipelineDiagramAction->setEnabled(!state);
  m_runAction->setEnabled(!state);
}
#endif

void NeuronTab::run(bool state) {
  // Stop any currently exeuting auto-clocking
#if 0
  if (m_autoClockAction->isChecked()) {
    m_autoClockAction->setChecked(false);
  }
  if (state) {
    NeuronHandler::run();
    m_statUpdateTimer->start();
  } else {
    NeuronHandler::stopRun();
    m_statUpdateTimer->stop();
#endif
  }

#if 0
  // Enable/Disable all actions based on whether the processor is running.
  m_selectNeuronAction->setEnabled(!state);
  m_clockAction->setEnabled(!state);
//  m_autoClockAction->setEnabled(!state);
//  m_reverseAction->setEnabled(!state);
  m_resetAction->setEnabled(!state);
  m_displayValuesAction->setEnabled(!state);
//  m_pipelineDiagramAction->setEnabled(!state);

  // Disable widgets which are not updated when running the processor
//  m_vsrtlWidget->setEnabled(!state);
//  m_ui->registerContainerWidget->setEnabled(!state);
//  m_ui->instructionView->setEnabled(!state);
}


/*
void NeuronTab::showPipelineDiagram() {
  auto w = PipelineDiagramWidget(m_stageModel);
  w.exec();
}
*/
#endif
  void NeuronTab::reverse() {
      //  m_vsrtlWidget->reverse();
      enableSimulatorControls();
  }
//} // namespace Ripes


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
    ui = new Ui::NeuronTab;
    ui->setupUi(this);

    // Set up Slider 1
    ui->Slider1->setMinimum(3);
    ui->Slider1->setMaximum(17);
    ui->Slider1->setValue(5);
    ui->Slider1Value->setText(QString::number(ui->Slider1->value()));
    QObject::connect(ui->Slider1, &QSlider::valueChanged, this, [=] () {
        (ui->Slider1Value->setText(QString::number(ui->Slider1->value())));
    });
    // Set up Slider 2
    ui->Slider2->setMinimum(15);
    ui->Slider2->setMaximum(50);
    ui->Slider2->setValue(23);
    ui->Slider2Value->setText(QString::number(ui->Slider2->value()));
    QObject::connect(ui->Slider2, &QSlider::valueChanged, this, [=] () {
        (ui->Slider2Value->setText(QString::number(ui->Slider2->value(),'f',2)));
    });
    // Set up Slider 3
    ui->Slider3->setMinimum(31);
    ui->Slider3->setMaximum(63);
    ui->Slider3->setValue(35);
    ui->Slider3Value->setText(QString::number(ui->Slider3->value()));
    QObject::connect(ui->Slider3, &QSlider::valueChanged, this, [=] () {
        (ui->Slider3Value->setText(QString::number(ui->Slider3->value(),'f',2)));
    });
    // Set up display slowdown Slider
    ui->DisplaySlider->setMinimum(0);
    ui->DisplaySlider->setMaximum(100);
    ui->DisplaySlider->setValue(5);
    ui->DisplayValue->setText(QString::number(ui->DisplaySlider->value()));
    QObject::connect(ui->DisplaySlider, &QSlider::valueChanged, this, [=] () {
        (ui->DisplayValue->setText(QString::number(ui->DisplaySlider->value())));
    });
    ui->StepNumberBox->setRange(1,100);
    ui->StepTimeBox->setRange(10,1000);

  setupSimulatorActions(controlToolbar);

  // Initially, no file is loaded, disable toolbuttons
  enableSimulatorControls();
}


void NeuronTab::setupSimulatorActions(QToolBar *controlToolbar) {
  const QIcon neuronIcon = QIcon(":/icons/cpu.svg");
  m_selectNeuronAction =
      new QAction(neuronIcon, "Select neuron", this);
  connect(m_selectNeuronAction, &QAction::triggered, this,
          &NeuronTab::neuronSelection);
  controlToolbar->addAction(m_selectNeuronAction);
  controlToolbar->addSeparator();
//  const QIcon clockIcon = QIcon(":/icons/step.svg");


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

void NeuronTab::restart() {
  // Invoked when changes to binary simulation file has been made
  enableSimulatorControls();
}

NeuronTab::~NeuronTab() { delete ui; }

void NeuronTab::neuronFinished() {
  // Disallow further clocking of the circuit
/*  m_autoClockAction->setChecked(false);
  m_autoClockAction->setEnabled(false);
*/
  m_runAction->setEnabled(false);
  m_runAction->setChecked(false);
}

void NeuronTab::enableSimulatorControls() {
/*  m_autoClockAction->setEnabled(true);
  m_runAction->setEnabled(true);
  m_reverseAction->setEnabled(m_vsrtlWidget->isReversible());
  m_resetAction->setEnabled(true);
  m_pipelineDiagramAction->setEnabled(true);
*/
}

void NeuronTab::reset() {
//  m_autoClockAction->setChecked(false);
  enableSimulatorControls();
//  SystemIO::printString("\n");
}






  void NeuronTab::reverse() {
      //  m_vsrtlWidget->reverse();
      enableSimulatorControls();
  }
//} // namespace Ripes

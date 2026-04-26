
#include <QDir>
#include <QFontMetrics>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QSpinBox>
#include <QTemporaryFile>
//#include "consolewidget.h"

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
/*  m_selectNeuronAction =
      new QAction(neuronIcon, "Select neuron", this);
  connect(m_selectNeuronAction, &QAction::triggered, this,
          &NeuronTab::neuronSelection);
  controlToolbar->addAction(m_selectNeuronAction);
  controlToolbar->addSeparator();
//  const QIcon clockIcon = QIcon(":/icons/step.svg");
*/


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



void NeuronTab::restart() {
  // Invoked when changes to binary simulation file has been made
  enableSimulatorControls();
}

NeuronTab::~NeuronTab() { delete ui; }


void NeuronTab::enableSimulatorControls() {
/*  m_autoClockAction->setEnabled(true);
  m_runAction->setEnabled(true);
  m_reverseAction->setEnabled(m_vsrtlWidget->isReversible());
  m_resetAction->setEnabled(true);
  m_pipelineDiagramAction->setEnabled(true);
*/
}


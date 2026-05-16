
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
NeuronTab::NeuronTab(//QToolBar *controlToolbar,
                           //QToolBar *additionalToolbar,
                     ScQtNeuron_MainWindow *parent)
    : //Ripes::RipesTab
    QWidget(//additionalToolbar,
            parent) {
    ui = new Ui::NeuronTab;
    ui->setupUi(this);

    // Set up Slider 3: Amplitude
    ui->RushinAmplitudeSlider->setMinimum(10);
    ui->RushinAmplitudeSlider->setMaximum(3000);
    ui->RushinAmplitudeSlider->setValue(120);
    ui->RushinAmplitudeSlider->setPageStep(20);
    ui->RushinAmplitudeSliderValue->setText(QString::number(ui->RushinAmplitudeSlider->value()));
    QObject::connect(ui->RushinAmplitudeSlider, &QSlider::valueChanged, this, [=] () {
        (ui->RushinAmplitudeSliderValue->setText(QString::number(ui->RushinAmplitudeSlider->value(),'f',2)));
    });
    // Set up Slider 2 : tau, us
    ui->MembraneTauSlider->setMinimum(500);
    ui->MembraneTauSlider->setMaximum(20000);
    ui->MembraneTauSlider->setValue(1560);
    ui->MembraneTauSlider->setPageStep(500);
    ui->MembraneTauSliderValue->setText(QString::number(ui->MembraneTauSlider->value()));
    QObject::connect(ui->MembraneTauSlider, &QSlider::valueChanged, this, [=] () {
        (ui->MembraneTauSliderValue->setText(QString::number(ui->MembraneTauSlider->value(),'f',2)));
    });
    // Set up Slider 1 : Resistance, MOhm
    ui->MembraneRSlider->setMinimum(100);
    ui->MembraneRSlider->setMaximum(9000);
    ui->MembraneRSlider->setValue(2500);
    ui->MembraneRSlider->setPageStep(100);
    ui->MembraneRSliderValue->setText(QString::number(ui->MembraneRSlider->value()));
    QObject::connect(ui->MembraneRSlider, &QSlider::valueChanged, this, [=] () {
        (ui->MembraneRSliderValue->setText(QString::number(ui->MembraneRSlider->value())));
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

//  setupSimulatorActions(controlToolbar);

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

#include "moc_neurontab.cpp"

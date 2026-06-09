
#include <QDir>
#include <QFontMetrics>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QSpinBox>
#include <QTemporaryFile>
#include "ScQtNeuron_MainWindow.h"
//#include "consolewidget.h"

//#include "neuronhandler.h"
#include "neurontab.h"
#include "ui_neurontab.h"

class ScQtNeuron_MainWindow;

NeuronTab::NeuronTab(QToolBar *controlToolbar,
                           QToolBar *additionalToolbar,
                     ScQtNeuron_MainWindow *parent)
//    : Ripes::RipesTab(additionalToolbar,
    :QWidget( //parent
              ),
      m_parent(parent)
{
    ui = new Ui::NeuronTab;
    ui->setupUi(this);
    SetupGUI();

//  setupSimulatorActions(controlToolbar);

  // Initially, no file is loaded, disable toolbuttons
  enableSimulatorControls();
}

//extern vector<double> Default_MembraneParameters;
void NeuronTab::SetupGUI()
{
    // Set up rush-in curent: Amplitude
    ui->RushinAmplitudeSlider->setMinimum(10);
    ui->RushinAmplitudeSlider->setMaximum(300);
    ui->RushinAmplitudeSlider->setValue(m_parent->MyNeuron->RushinParameters_Get()->at(0)/1000);
    ui->RushinAmplitudeSlider->setPageStep(20);
    ui->RushinAmplitudeSliderValue->setText(QString::number(ui->RushinAmplitudeSlider->value()));
    QObject::connect(ui->RushinAmplitudeSlider, &QSlider::valueChanged, this, [=] () {
        (ui->RushinAmplitudeSliderValue->setText(QString::number(ui->RushinAmplitudeSlider->value(),'f',2)));
    });

    // Set up rush-in A the rising edge
    ui->RushinASlider->setMinimum(10);
    ui->RushinASlider->setMaximum(300);
    ui->RushinASlider->setValue(m_parent->MyNeuron->RushinParameters_Get()->at(1)*1000);
    ui->RushinASlider->setPageStep(20);
    double temp = ui->RushinASlider->value();
    ui->RushinASliderValue->setText(QString::number(ui->RushinASlider->value()));
    QObject::connect(ui->RushinASlider, &QSlider::valueChanged, this, [=] () {
        (ui->RushinASliderValue->setText(QString::number(ui->RushinASlider->value(),'f',2)));
    });

    // Set up rush-in B the falling edge
    ui->RushinBSlider->setMinimum(10);
    ui->RushinBSlider->setMaximum(1000);
    ui->RushinBSlider->setValue(m_parent->MyNeuron->RushinParameters_Get()->at(2)*1000);
    ui->RushinBSlider->setPageStep(20);
    ui->RushinBSliderValue->setText(QString::number(ui->RushinBSlider->value()));
    QObject::connect(ui->RushinBSlider, &QSlider::valueChanged, this, [=] () {
        (ui->RushinBSliderValue->setText(QString::number(ui->RushinBSlider->value(),'f',2)));
    });

    // Set up membrane time constant : tau, us
    ui->MembraneTauSlider->setMinimum(100);
    ui->MembraneTauSlider->setMaximum(500);
//    ui->MembraneTauSlider->setValue(220);
    ui->MembraneTauSlider->setValue(m_parent->MyNeuron->MembraneParameters_Get()->at(2)*1000);
    ui->MembraneTauSlider->setPageStep(500);
    ui->MembraneTauSliderValue->setText(QString::number(ui->MembraneTauSlider->value()));
    QObject::connect(ui->MembraneTauSlider, &QSlider::valueChanged, this, [=] () {
        (ui->MembraneTauSliderValue->setText(QString::number(ui->MembraneTauSlider->value(),'f',2)));
    });

    // Set up R_Membrane Slider : Resistance, kOhm
    ui->MembraneRSlider->setMinimum(100);
    ui->MembraneRSlider->setMaximum(900);
//    ui->MembraneRSlider->setValue(m_parent->MyNeuron->MembraneParameters_Get()->at(0)*1000);
    ui->MembraneRSlider->setValue(200*1000);
    ui->MembraneRSlider->setPageStep(100);
    ui->MembraneRSliderValue->setText(QString::number(ui->MembraneRSlider->value()));
    QObject::connect(ui->MembraneRSlider, &QSlider::valueChanged, this, [=] () {
        (ui->MembraneRSliderValue->setText(QString::number(ui->MembraneRSlider->value())));
    });

    // Set up R_AIS Slider : Resistance, kOhm
    ui->MembraneAISRSlider->setMinimum(100);
    ui->MembraneAISRSlider->setMaximum(900);
    ui->MembraneAISRSlider->setValue(m_parent->MyNeuron->MembraneParameters_Get()->at(0)*1000);
    ui->MembraneAISRSlider->setPageStep(100);
    ui->MembraneAISRSliderValue->setText(QString::number(ui->MembraneAISRSlider->value()));
    QObject::connect(ui->MembraneAISRSlider, &QSlider::valueChanged, this, [=] () {
        (ui->MembraneAISRSliderValue->setText(QString::number(ui->MembraneAISRSlider->value())));
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
    ui->StepTimeBox->setValue(50);
}

void NeuronTab::setupSimulatorActions(QToolBar *controlToolbar) {
}


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

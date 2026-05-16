/** @file VoltageWindow.cpp
 *  @brief The main window for the SystemC-based neuron simulator, using Qt's stuff
 *  Ideas taken from  http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
 *  https://www.researchgate.net/publication/228972213_gSysC_A_graphical_front_end_for_SystemC
 *  and https://github.com/mortbopet/Ripes
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/
#include "VoltageWindow.h"
#include "ui_VoltageWindow.h"
#include <QDebug>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QApplication>

#include <QFile>

VoltageWindow::VoltageWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent ):
  QMainWindow(parent),
  ui(new Ui::VoltageWindow),
    m_Simulator(Simulator),
    m_neuron(Neuron)
{
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);
    this->setStyleSheet("color: Navy;"
                        "border-color:  LightGray;"
                        "background-color:  LightGray;");
    setWindowTitle(QString(m_neuron->name())+QString(" ActionPotential"));
    statusBar()->clearMessage();
    ui->actionScreenshot->setIcon(QIcon(":/icons/analytics.svg"));
    setupMenus();
    setupPlot();
    connect(ui->actionScreenshot, &QAction::triggered, this, &VoltageWindow::screenShot);
}

void VoltageWindow::setupMenus()
{
/*    const QIcon saveIcon = QIcon(":/icons/save.svg");
    auto *screenshotAction = new QAction(saveIcon, "Screenshot to File", this);
    ui->menuFile->addAction(screenshotAction);
    screenshotAction->setShortcut(QKeySequence::Save);
    connect(screenshotAction, &QAction::triggered, this,
            &VoltageWindow::screenShot);*/
}



void VoltageWindow::replot(void)
{ui->customPlot->replot();}

void VoltageWindow::Reset()
{
    dataVoltagePlot.clear(); RunningPointPosition_Set(0,0); index = 0;
    VoltagePlot->data()->set(dataVoltagePlot, true);
    m_HaveAlreadyP =  m_HaveAlreadyH = false;
    replot();
}

void VoltageWindow::setupPlot()
{
    double key2 = m_neuron->LocalTimeInMillisec_Get()*2.4;
    double Volt2 = m_neuron->MembraneRelativePotential_Get()*15;
    VoltagePlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
//    VoltagePlot->data()->set(dataVoltagePlot, true);
    VoltagePlot->setPen(QPen(Qt::blue));
    VoltagePlot->setBrush(QBrush(QColor(2, 20, 2, 20)));
    VoltagePlot->setName("Action Potential");
    VoltagePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    VoltagePlot->setLineStyle(QCPCurve::lsLine);


    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Time (ms)");
    ui->customPlot->yAxis->setLabel("Membrane voltage (mV)");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0,1);
    ui->customPlot->yAxis->setRange(-30,130);
    // set some basic customPlot config:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->rescaleAxes();
    // Add an ellipse
    RunningPoint = new QCPItemEllipse(ui->customPlot);
    RunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
    RunningPoint->setPen(QPen(Qt::red));
    RunningPointPosition_Set(key2,Volt2);

//    connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(displayDataSlot()));
    ui->customPlot->axisRect()->setupFullAxesBox();
    Reset();
}

void VoltageWindow::RunningPointPosition_Set(double xpos, double ypos)
{
    RunningPoint->topLeft->setCoords(xpos-0.01, ypos-1);    // Set coordinates
    RunningPoint->bottomRight->setCoords(xpos+0.01, ypos+1);
}

void VoltageWindow::displayDataSlot()
{

    double Volt2 = m_neuron->MembraneRelativePotential_Get();
    double key2 = m_neuron->LocalTimeInMillisec_Get();
    if(index>0)
    {
        double OldTime = dataVoltagePlot[index-1].key;
        if(OldTime>key2)
        {// We step back on the time scale; reset plot
            Reset(); index = 0;
        }
    }
    RunningPointPosition_Set(key2,Volt2);
    dataVoltagePlot.push_back(QCPCurveData(index++,key2,Volt2));
    VoltagePlot->data()->set(dataVoltagePlot, true);
    VoltagePlot->setPen(QPen(Qt::blue));
    VoltagePlot->setBrush(QBrush(QColor(2, 20, 20, 20)));


    // The rest is only for displaying demo legend
    if ( m_neuron->EVENT_GenComp.InputReceived.triggered() ) {
        DrawArrow(key2, Volt2, "X",.02,-15);
    }
    if ( m_neuron->EVENT_GenComp.DeliveringBegin.triggered() ) {
        DrawArrow(key2, Volt2, "<R",+0.068,18);
    }
    if ( m_neuron->EVENT_GenComp.RelaxingBegin.triggered() ) {
        DrawArrow(key2, Volt2, "R>",-0.05,18);
    }

    if(GenCompStageMachine_t::gcsm_Delivering == m_neuron->StageFlag_Get())
    {
        if ((m_neuron->dVdtResultingLast_Get() >=0) && (m_neuron->dVdtResulting_Get() < 0))
        {   // We are at the point of maximum polarization

            if(!m_HaveAlreadyP){DrawArrow(key2, Volt2, "P",0.03,-30); m_HaveAlreadyP = true;}
        }
    }
    if(GenCompStageMachine_t::gcsm_Relaxing == m_neuron->StageFlag_Get())
    {
        if ((m_neuron->dVdtResultingLast_Get() <0) && (m_neuron->dVdtResulting_Get() >= 0))
        {   // We are at the point of maximum polarization

            if(!m_HaveAlreadyH){ DrawArrow(key2, Volt2, "H",-0,50); m_HaveAlreadyH = true;}
        }
    }
     ui->customPlot->replot();
}

void VoltageWindow::DrawArrow(double xpos, double ypos, QString S, double xoffset, double yoffset)
{
    if ("P"==S)
        int test = 1;
    // add the text label at the top:
    QCPItemText *textLabel = new QCPItemText(ui->customPlot);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    //textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel->position->setCoords(xpos+xoffset, ypos+yoffset); // place position at center/top of axis rect
    textLabel->setText(S);
    textLabel->setFont(QFont(font().family(), 8)); // make font a bit smaller
    textLabel->setPen(QPen(Qt::red)); // show red border around text

    // add the arrow:
    QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
    arrow->start->setParentAnchor(textLabel->bottom);
    arrow->end->setCoords(xpos, ypos);
    arrow->setHead(QCPLineEnding::esSpikeArrow);
}

VoltageWindow::~VoltageWindow()
{
  delete ui;
}

void VoltageWindow::screenShot()
{
    QTime now = QTime::currentTime();
    QString fileName = //=QString("screenshots/")+
                       QString(m_neuron->name())+QString("_Voltage Plot_"+now.toString("hh:mm:ss"))+QString(".pdf");
    fileName.replace(" ", "");
    ui->customPlot->savePdf(fileName, 0, 0);
}

#include "moc_VoltageWindow.cpp"

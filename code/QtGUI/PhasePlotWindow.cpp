/** @file PhasePloteWindow.cpp
 *  @brief The main window for the SystemC-based neuron simulator, using Qt's stuff
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "PhasePlotWindow.h"
#include "ui_PhasePlotWindow.h"
#include <QDebug>
#include <iostream>
#include <QFile>

#define REVERSEDGRADIENT 0

PhasePlotWindow::PhasePlotWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent ):
  QMainWindow(parent),
  ui(new Ui::PhasePlotWindow),
    m_Simulator(Simulator)
    ,m_neuron(Neuron)
    ,m_DisplayMode(false)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
  this->setStyleSheet("color: Navy;"
                      "border-color:  LightGray;"
                      "background-color:  LightGray;");
  setWindowTitle(QString(m_neuron->name())+QString(" phase plot"));
  statusBar()->clearMessage();
  ui->actionScreenshot->setIcon(QIcon(":/icons/analytics.svg"));
  setupMenus();
//  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(realtimeDataSlot())); // Moved to MainWindow
  ui->customPlot->axisRect()->setupFullAxesBox();

  PhasePlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
  PhasePlot->setPen(QPen(Qt::blue));
  PhasePlot->setBrush(QBrush(QColor(2, 20, 20, 20)));
  PhasePlot->setName("AP phase plot");
  PhasePlot->setLineStyle(QCPCurve::lsLine);
  PhasePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
  // Add an ellipse
  RunningPoint = new QCPItemEllipse(ui->customPlot);
  RunningPointPosition_Set(0,0);
  RunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
  RunningPoint->setPen(QPen(Qt::red));

  // set some basic customPlot config:
  ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
  ui->customPlot->legend->setVisible(true); // Ensure legend is visible
  ui->customPlot->legend->setFont(QFont("Helvetica", 9));
  ui->customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // Set a semi-transparent brush for the legend:
  // Set position to upper left inside the axis rect
  ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignTop);
  setupDataPlot();
  Reset();

  connect(ui->actionScreenshot, &QAction::triggered, this, &PhasePlotWindow::screenShot);
}

PhasePlotWindow::~PhasePlotWindow()
{
    delete ui;
}

void PhasePlotWindow::setupMenus()
{
 /*   const QIcon saveIcon = QIcon(":/icons/save.svg");
    auto *screenshotAction = new QAction(saveIcon, "Screenshot to File", this);
    ui->menuFile->addAction(screenshotAction);
    screenshotAction->setShortcut(QKeySequence::Save);
    connect(screenshotAction, &QAction::triggered, this,
            &PhasePlotWindow::screenShot);*/
}
    // Edit actions



void PhasePlotWindow::setupDataPlot(    )
{
    // Optional: Add a slight margin so it doesn't touch the edge
//??    ui->customPlot->axisRect()->insetLayout()->setInsetMargins(0, QMargins(10, 10, 10, 10));


    if(m_DisplayMode)
   {
       ui->customPlot->xAxis->setLabel("Voltage (mV)");
       ui->customPlot->yAxis->setLabel("Gradient (V/s)");
        ui->customPlot->xAxis->setRange(-30, 130);
       ui->customPlot->yAxis->setRange(-8000, 3500);
    }
    else
    {
        ui->customPlot->xAxis->setLabel("Gradient (V/s)");
       ui->customPlot->yAxis->setLabel("Voltage (mV)");
       ui->customPlot->xAxis->setRange(-8000, 3500);
       ui->customPlot->yAxis->setRange(-30, 130);
    }; //REVERSEDGRADIENT
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->rescaleAxes();
}

//BENCHMARK_TIME_BEGIN(&m_display_t,&m_display_x);    // Begin display time benchmarking here
//BENCHMARK_TIME_END(&m_display_t,&m_display_x,&m_display_s);   // End display time benchmarking here



/* For speed-up
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */

void PhasePlotWindow::replot(void)
{ui->customPlot->replot();}

void PhasePlotWindow::Reset()
{
    dataPhasePlot.clear();  RunningPointPosition_Set(0,0);  index = 0;
    PhasePlot->data()->set(dataPhasePlot, true);
    replot();
}

void PhasePlotWindow::RunningPointPosition_Set(double xpos, double ypos)
{
    if(m_DisplayMode)
    {
        RunningPoint->topLeft->setCoords(-0.5+xpos, ypos-.50);    // Set coordinates
        RunningPoint->bottomRight->setCoords(0.5+xpos, ypos+.50);
    }
    else
    {
        RunningPoint->topLeft->setCoords(-50+xpos, ypos-.5);    // Set coordinates
        RunningPoint->bottomRight->setCoords(50+xpos, ypos+.5);
    }
}

void PhasePlotWindow::displayDataSlot()
{
//            BENCHMARK_TIME_BEGIN(&m_Simulator->m_display_t,&m_Simulator->m_display_x);    // Begin benchmarking here
     if(!index) setupDataPlot();
    double Volt2 = m_neuron->MembraneRelativePotential_Get();
    double DvDt = m_neuron->dVdtResulting_Get();
    if(m_DisplayMode)
    {
        RunningPointPosition_Set(Volt2,DvDt);
        dataPhasePlot.push_back(QCPCurveData(index++,Volt2, DvDt));
        // The rest is only for displaying demo legend
        if ( m_neuron->EVENT_GenComp.InputReceived.triggered() ) {
            DrawArrow(Volt2,DvDt,  "X",-10,800);
        }
        if ( m_neuron->EVENT_GenComp.DeliveringBegin.triggered() ) {
            DrawArrow(Volt2, DvDt, "<R",15,250);
        }
        if ( m_neuron->EVENT_GenComp.RelaxingBegin.triggered() ) {
            DrawArrow( Volt2, DvDt, "R>",35,800);
        }

        if(GenCompStageMachine_t::gcsm_Delivering == m_neuron->StageFlag_Get())
        {
            if ((m_neuron->dVdtResultingLast_Get() >=0) && (m_neuron->dVdtResulting_Get() < 0))
            {   // We are at the point of maximum polarization
                if(!m_HaveAlreadyP){DrawArrow( Volt2, DvDt,"P",-65,900); m_HaveAlreadyP = true;}
            }
        }
        if(GenCompStageMachine_t::gcsm_Relaxing == m_neuron->StageFlag_Get())
        {
            if ((m_neuron->dVdtResultingLast_Get() <0) && (m_neuron->dVdtResulting_Get() >= 0))
            {   // We are at the point of maximum hyperpolarization
                if(!m_HaveAlreadyH){DrawArrow(Volt2,  DvDt, "H",5,500);; m_HaveAlreadyH = true;}

            }
        }
    }
    else
    {
        RunningPointPosition_Set(DvDt,Volt2);
        dataPhasePlot.push_back(QCPCurveData(index++, DvDt,Volt2));

        // The rest is only for displaying demo legend
        if ( m_neuron->EVENT_GenComp.InputReceived.triggered() ) {
            DrawArrow(DvDt,Volt2,  "X",300,-15);
        }
        if ( m_neuron->EVENT_GenComp.DeliveringBegin.triggered() ) {
            DrawArrow( DvDt, Volt2, "<R",-300.,45);
        }
        if ( m_neuron->EVENT_GenComp.RelaxingBegin.triggered() ) {
            DrawArrow( DvDt, Volt2, "R>",1100,45);
        }

        if(GenCompStageMachine_t::gcsm_Delivering == m_neuron->StageFlag_Get())
        {
            if ((m_neuron->dVdtResultingLast_Get() >=0) && (m_neuron->dVdtResulting_Get() < 0))
            {   // We are at the point of maximum polarization
                DrawArrow( DvDt, Volt2,"P",300,-20);
            }
        }
        if(GenCompStageMachine_t::gcsm_Relaxing == m_neuron->StageFlag_Get())
        {
            if ((m_neuron->dVdtResultingLast_Get() <0) && (m_neuron->dVdtResulting_Get() >= 0))
            {   // We are at the point of maximum hyperpolarization
                DrawArrow( DvDt, Volt2, "H",-100,25);
            }
        }
    };

    PhasePlot->data()->set(dataPhasePlot, true);


    ui->customPlot->replot();
 //           BENCHMARK_TIME_END(&m_Simulator->m_display_t,m_Simulator->&m_display_x,&m_Simulator->m_display_s);   // End benchmarking here
}

void PhasePlotWindow::DrawArrow(double xpos, double ypos, QString S, double xoffset, double yoffset)
{
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
    arrow->end->setCoords(xpos, ypos); // point to (4, 1.6) in x-y-plot coordinates
    arrow->setHead(QCPLineEnding::esSpikeArrow);
}

void PhasePlotWindow::DisplayMode_Set(bool M)
{
    m_DisplayMode = M;  // Change the mode
/*    if(dataPhasePlot.size())
    {   // We have already data, swap them
        for(int32_t index = 0; index<dataPhasePlot.size(); index++)
        {
            QCPCurveData D = dataPhasePlot[index];
            double tmp = D.t;
            D.t = D.value;
            D.value = tmp;
            dataPhasePlot[index] = D;
        }
    }
    setupRealtimeDataDemo(ui->customPlot);
    PhasePlot->data()->set(dataPhasePlot, true);
    ui->customPlot->replot();
*/
    setupDataPlot();
    Reset();
}

void PhasePlotWindow::screenShot()
{
      QTime now = QTime::currentTime();
    QString fileName = //QString("screenshots/")+
      QString(m_neuron->name())+QString("_Phase Plot_"+now.toString("hh:mm:ss"))+QString(".pdf");
  fileName.replace(" ", "");
  ui->customPlot->savePdf(fileName, 0, 0);
}

#include "moc_PhasePlotWindow.cpp"

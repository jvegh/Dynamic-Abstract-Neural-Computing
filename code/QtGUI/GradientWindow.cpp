/** @file GradientWindow.cpp
 *  @brief The main window for the SystemC-based neuron simulator, using Qt's stuff
 *  Ideas taken from  http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/
 *  https://www.researchgate.net/publication/228972213_gSysC_A_graphical_front_end_for_SystemC
 *  and https://github.com/mortbopet/Ripes
 */
/*
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
*/

#include "GradientWindow.h"
#include "ui_GradientWindow.h"
#include <QDebug>

GradientWindow::GradientWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent ):
  QMainWindow(parent),
  ui(new Ui::GradientWindow),
    m_Simulator(Simulator),
    m_neuron(Neuron)
{
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);
    setWindowTitle(QString(m_neuron->name())+QString(" voltage gradients"));
    statusBar()->clearMessage();
    ui->actionScreenshot->setIcon(QIcon(":/icons/analytics.svg"));
    this->setStyleSheet("color: Navy;"
                        "border-color:  LightGray;"
                        "background-color:  LightGray;"
                        );
    setupPlot();
    connect(ui->actionScreenshot, &QAction::triggered, this, &GradientWindow::screenShot);
    Reset();
}

void GradientWindow::replot(void)
{   ui->customPlot->replot();}


void GradientWindow::Reset()
{
    dataRushinGradientPlot.clear(); RushinRunningPointPositionGradient_Set(0,0);
    dataAISGradientPlot.clear(); AISRunningPointPositionGradient_Set(0,0);
    dataGradientPlot.clear();  RunningPointPositionGradient_Set(0,0);
/*    RushinGradientPlot->data()->set(dataRushinGradientPlot, true);
    AISGradientPlot->data()->set(dataAISGradientPlot, true);
    GradientPlot->data()->set(dataGradientPlot, true);*/
    index = 0; replot();
}


// Fill area between graph 0 and graph 1
/* customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));
customPlot->graph(0)->setBrush(QBrush(QColor(20, 20, 20, 20)));
*/

void GradientWindow::setupPlot( )
{
     // Add ellipses
    RunningPoint = new QCPItemEllipse(ui->customPlot);
    RunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
    RunningPoint->setPen(QPen(Qt::red));
    AISRunningPoint  = new QCPItemEllipse(ui->customPlot);
    RushinRunningPoint  = new QCPItemEllipse(ui->customPlot);

    RushinGradientPlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    RushinGradientPlot->setName("Input gradient");
    RushinGradientPlot->setLineStyle(QCPCurve::lsLine);
    RushinGradientPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

    AISGradientPlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    AISGradientPlot->setName("AIS gradient");
    AISGradientPlot->setLineStyle(QCPCurve::lsLine);
    AISGradientPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

    GradientPlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    GradientPlot->setName("Resulting gradient");
    GradientPlot->setLineStyle(QCPCurve::lsLine);
    GradientPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

    GradientPlot->setPen(QPen(Qt::blue));
    GradientPlot->setBrush(QBrush(QColor(2, 2, 20, 20)));
    AISGradientPlot->setPen(QPen(Qt::red));
    AISGradientPlot->setBrush(QBrush(QColor(20, 2, 2, 20)));
    RushinGradientPlot->setPen(QPen(Qt::green));
    RushinGradientPlot->setBrush(QBrush(QColor(2, 20, 2, 20)));

    ui->customPlot->legend->setVisible(true); // Ensure legend is visible
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    ui->customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // Set a semi-transparent brush for the legend:
    // Set position to upper left inside the axis rect
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignTop);

    index = 0;
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Time (ms)");
    ui->customPlot->yAxis->setLabel("Membrane gradient (V/s)");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0,1);
    ui->customPlot->yAxis->setRange(-2000,4000);
    // set some basic customPlot config:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->rescaleAxes();

  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(DataSlot()));

  ui->customPlot->axisRect()->setupFullAxesBox();
  ui->customPlot->replot();
}


// Fill area between graph 0 and graph 1
/*     GradientPlot->setChannelFillGraph(customPlot->graph(1));
customPlot->graph(0)->setBrush(QBrush(QColor(20, 20, 20, 20)));
*/

#if 0
  /* For speed
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
#endif

void GradientWindow::RunningPointPositionGradient_Set(double xpos, double ypos)
{
    RunningPoint->topLeft->setCoords(xpos-0.01, ypos-2);    // Set coordinates
    RunningPoint->bottomRight->setCoords(xpos+0.01, ypos+2);
}

void GradientWindow::AISRunningPointPositionGradient_Set(double xpos, double ypos)
{
    AISRunningPoint->topLeft->setCoords(xpos-0.01, ypos-2);    // Set coordinates
    AISRunningPoint->bottomRight->setCoords(xpos+0.01, ypos+2);
}

void GradientWindow::RushinRunningPointPositionGradient_Set(double xpos, double ypos)
{
    RushinRunningPoint->topLeft->setCoords(xpos-0.01, ypos-2);    // Set coordinates
    RushinRunningPoint->bottomRight->setCoords(xpos+0.01, ypos+2);
}

void GradientWindow::DataSlot()
{
    double key2 = m_neuron->LocalTimeInMillisec_Get();
    double DvDt = m_neuron->dVdtResulting_Get();
    double Membrane_dVdt_AIS = -m_neuron->dVdtAIS_Get();
    double Membrane_dVdt_Input = m_neuron->dVdtInput_Get();
    if(index>0)
    {
        double OldTime = dataGradientPlot[index-1].key;
        if(OldTime>key2)
            Reset(); // We step back on the time scale; reset plot
    }
    // Handle resultant gradient display
    dataGradientPlot.push_back(QCPCurveData(index,key2, DvDt));
    GradientPlot->data()->set(dataGradientPlot, true);
    RunningPointPositionGradient_Set(key2,DvDt);

    // Handle AIS gradient display
    dataAISGradientPlot.push_back(QCPCurveData(index,key2, Membrane_dVdt_AIS));
    AISGradientPlot->data()->set(dataAISGradientPlot, true);
    AISRunningPointPositionGradient_Set(key2,Membrane_dVdt_AIS);

    // Handle Rush-in gradient display
    dataRushinGradientPlot.push_back(QCPCurveData(index,key2, Membrane_dVdt_Input));
    RushinGradientPlot->data()->set(dataRushinGradientPlot, true);
    RushinRunningPointPositionGradient_Set(key2,Membrane_dVdt_Input);
    index++;

    // The rest is only for displaying demo legend
    if ( m_neuron->EVENT_GenComp.InputReceived.triggered() ) {
        DrawArrow(key2, DvDt, "X",-0.04,-500);
    }
    if ( m_neuron->EVENT_GenComp.DeliveringBegin.triggered() ) {
        DrawArrow(key2, DvDt, "<R",0.03,700);
    }
    if ( m_neuron->EVENT_GenComp.RelaxingBegin.triggered() ) {
        DrawArrow(key2, DvDt, "R>",-0.05,1300);
    }

    if(GenCompStageMachine_t::gcsm_Delivering == m_neuron->StageFlag_Get())
    {
        if ((m_neuron->dVdtResultingLast_Get() >=0) && (m_neuron->dVdtResulting_Get() < 0))
        {   // We are at the point of maximum polarization
            DrawArrow(key2, DvDt, "P",+0.04,1400);
        }
    }
    if(GenCompStageMachine_t::gcsm_Relaxing == m_neuron->StageFlag_Get())
    {
        if ((m_neuron->dVdtResultingLast_Get() <0) && (m_neuron->dVdtResulting_Get() >= 0))
        {   // We are at the point of maximum hyperpolarization
            DrawArrow(key2, DvDt, "H",0,1000);
        }
    }

    ui->customPlot->replot();
}

GradientWindow::~GradientWindow()
{
  delete ui;
}

 void GradientWindow::screenShot()
{
     QTime now = QTime::currentTime();
     QString fileName = //QString("./screenshots/")+
         QString(m_neuron->name())+QString("_Gradients Plot_"+now.toString("hh:mm:ss"))+QString(".pdf");
     fileName.replace(" ", "");
     ui->customPlot->savePdf(fileName, 0, 0);
}

void GradientWindow::DrawArrow(double xpos, double ypos, QString S, double xoffset, double yoffset)
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
    arrow->end->setCoords(xpos, ypos);
    arrow->setHead(QCPLineEnding::esSpikeArrow);
}

#include "moc_GradientWindow.cpp"

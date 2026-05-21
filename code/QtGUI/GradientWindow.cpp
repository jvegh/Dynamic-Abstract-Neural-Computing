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
    connect(m_Simulator,SIGNAL(eventHappened()), this, SLOT(displayIllegalInputSlot()));
     Reset();
}

void GradientWindow::replot(void)
{   ui->customPlot->replot();}


void GradientWindow::Reset()
{
    dataRushinGradientPlot.clear(); RushinRunningPointPositionGradient_Set(0,0);
    dataAISGradientPlot.clear(); AISRunningPointPositionGradient_Set(0,0);
    dataGradientPlot.clear();  RunningPointPositionGradient_Set(0,0);
    m_HaveAlreadyP =  m_HaveAlreadyH = false;
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
    RunningPoint->setPen(QPen(Qt::blue));
    AISRunningPoint  = new QCPItemEllipse(ui->customPlot);
    AISRunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
    AISRunningPoint->setPen(QPen(Qt::red));
    RushinRunningPoint  = new QCPItemEllipse(ui->customPlot);
    RushinRunningPoint->setBrush(QBrush(QColor(0,255,  0, 50)));
    RushinRunningPoint->setPen(QPen(Qt::green));

    RushinGradientPlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    RushinGradientPlot->setName("Input gradient");
    RushinGradientPlot->setPen(QPen(Qt::green));
    RushinGradientPlot->setBrush(QBrush(QColor(2, 20, 2, 20)));
//    RushinGradientPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
//    RushinGradientPlot->setLineStyle(QCPCurve::lsLine);

    AISGradientPlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    AISGradientPlot->setName("AIS gradient");
    AISGradientPlot->setPen(QPen(Qt::red));
    AISGradientPlot->setBrush(QBrush(QColor(20, 2, 2, 20)));
//    AISGradientPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
//    AISGradientPlot->setLineStyle(QCPCurve::lsLine);
//    AISGradientPlot->setLineStyle(QCPCurve::lsNone);

    GradientPlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    GradientPlot->setName("Resulting gradient");
    GradientPlot->setPen(QPen(Qt::blue));
    GradientPlot->setBrush(QBrush(QColor(2, 2, 20, 20)));
//    GradientPlot->setLineStyle(QCPCurve::lsLine);
//    GradientPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));


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
    ui->customPlot->xAxis->setRange(0,.3);
    ui->customPlot->yAxis->setRange(-2000,4000);
    // set some basic customPlot config:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->rescaleAxes();

//  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(displayDataSlot()));

  ui->customPlot->axisRect()->setupFullAxesBox();
//  ui->customPlot->replot();
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

void GradientWindow::PlotBrackets(int32_t key, double coord1, double coord2, double y)
{
    switch(key)
    {
    case 0:
    {
        // add the bracket for the "Computing" phase:
        QCPItemBracket *computingBracket = new QCPItemBracket(ui->customPlot);
        computingBracket->left->setCoords(coord1, y+10);
        computingBracket->right->setCoords(coord2, y+10);
        computingBracket->setLength(13);

        // add the text label at the top:
        QCPItemText *computingText = new QCPItemText(ui->customPlot);
        computingText->position->setParentAnchor(computingBracket->center);
        computingText->position->setCoords(0, -10); // move 10 pixels to the top from bracket center anchor
        computingText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        computingText->setText("Computing");
        computingText->setFont(QFont(font().family(), 10));
    } break;
    case 1:
    {
        // add the bracket for the "Delivering" phase:
        QCPItemBracket *deliveringBracket = new QCPItemBracket(ui->customPlot);
        deliveringBracket->left->setCoords(coord1,y);
        deliveringBracket->right->setCoords(coord2, y);
        deliveringBracket->setLength(13);

        // add the text label at the top:
        QCPItemText *deliveringText = new QCPItemText(ui->customPlot);
        deliveringText->position->setParentAnchor(deliveringBracket->center);
        deliveringText->position->setCoords(0, -10); // move 10 pixels to the top from bracket center anchor
        deliveringText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        deliveringText->setText("Delivering");
        deliveringText->setFont(QFont(font().family(), 10));
    } break;
    case 2:
    {
        // add the bracket for the "Delivering" phase:
        QCPItemBracket *relaxingBracket = new QCPItemBracket(ui->customPlot);
        relaxingBracket->left->setCoords(coord1, y);
        relaxingBracket->right->setCoords(coord2, y);
        relaxingBracket->setLength(13);

        // add the text label at the top:
        QCPItemText *relaxingText = new QCPItemText(ui->customPlot);
        relaxingText->position->setParentAnchor(relaxingBracket->center);
        relaxingText->position->setCoords(0, -10); // move 10 pixels to the top from bracket center anchor
        relaxingText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        relaxingText->setText("Relaxing");
        relaxingText->setFont(QFont(font().family(), 10));
    }break;
    default: assert(0);    }
}

void GradientWindow::displayIllegalInputSlot()
{
    if ( m_neuron->EVENT_GenComp.InputIllegal.triggered() ) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("ScQtSimulator"),
                                   tr("Input is not legal in the present state\n"
                                      "Maybe you want to exit"),
                                   QMessageBox::Yes );

        double key2 = m_neuron->LocalTimeInMillisec_Get();
        double DvDt = m_neuron->dVdtResulting_Get();
        DrawArrow(key2, DvDt, "!",-0.04,-500);
    }
}

void GradientWindow::displayDataSlot()
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
//        bool RedBackround = (GenCompStageMachine_t::gcsm_Delivering==m_neuron->StageFlag_Get());
        DrawArrow(key2, DvDt, "X",-0.04,-500);
    }
    if ( m_neuron->EVENT_GenComp.DeliveringBegin.triggered() ) {
        DrawArrow(key2, DvDt, "<R",0.03,700);
        m_T_DeliveringBegin = key2;
        m_V_Peak = Membrane_dVdt_Input;
        PlotBrackets(0,key2,0.,-1500);
    }
    if ( m_neuron->EVENT_GenComp.RelaxingBegin.triggered() ) {
        DrawArrow(key2, DvDt, "R>",-0.05,1300);
        m_T_RelaxingBegin = key2;
//        PlotBrackets(1,m_T_DeliveringBegin,key2, m_V_Peak*1.5+500);
        PlotBrackets(1,key2,m_T_DeliveringBegin, -1500);
    }
    if ( m_neuron->EVENT_GenComp.RelaxingEnd.triggered() ) {
        DrawArrow(key2, DvDt, "E",-0.05,18);
//        PlotBrackets(2,key2,m_T_RelaxingBegin, -1500);
    }

    if(GenCompStageMachine_t::gcsm_Delivering == m_neuron->StageFlag_Get())
    {
        if ((m_neuron->dVdtResultingLast_Get() >=0) && (m_neuron->dVdtResulting_Get() < 0))
        {   // We are at the point of maximum polarization
            if(!m_HaveAlreadyP){DrawArrow(key2, DvDt, "P",+0.04,1400); m_HaveAlreadyP = true;}
        }
    }

    if(GenCompStageMachine_t::gcsm_Relaxing == m_neuron->StageFlag_Get())
    {
        if ((m_neuron->dVdtResultingLast_Get() <0) && (m_neuron->dVdtResulting_Get() > 0))
        {   // We are at the point of maximum hyperpolarization
            if(!m_HaveAlreadyH){
                DrawArrow(key2, Membrane_dVdt_Input, "H",0,1000); m_HaveAlreadyH = true;
                PlotBrackets(2,key2,m_T_RelaxingBegin, -1500);//Membrane_dVdt_Input+3000);
            }
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
    QDate today = QDate::currentDate();
    QString fileName = //QString("screenshots/")+
        QString(m_neuron->name())+QString("_Gradients Plot_"+today.toString("yy.MM.dd") + QString("_") + now.toString("hh:mm:ss"))+QString(".pdf");
     fileName.replace(" ", "");
     ui->customPlot->savePdf(fileName, 0, 0);
}

void GradientWindow::DrawArrow(double xpos, double ypos, QString S, double xoffset, double yoffset)//, bool RedBackground)
{
    // add the text label at the top:
    QCPItemText *textLabel = new QCPItemText(ui->customPlot);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    //textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel->position->setCoords(xpos+xoffset, ypos+yoffset); // place position at center/top of axis rect
    textLabel->setText(S);
    textLabel->setFont(QFont(font().family(), 8)); // make font a bit smaller
    textLabel->setPen(QPen(Qt::red)); // show red border around text
 //   if(RedBackground)textLabel->setBrush(Qt::red);

    // add the arrow:
    QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
    arrow->start->setParentAnchor(textLabel->bottom);
    arrow->end->setCoords(xpos, ypos);
    arrow->setHead(QCPLineEnding::esSpikeArrow);
}

#include "moc_GradientWindow.cpp"

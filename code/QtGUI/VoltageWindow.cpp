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

    VoltagePlot->setPen(QPen(Qt::blue));
    VoltagePlot->setBrush(QBrush(QColor(2, 20, 20, 20)));
    VoltagePlot->setName("Action Potential");
    VoltagePlot->setLineStyle(QCPCurve::lsLine);
    VoltagePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
#if 0
    VoltagePlot->setPen(QPen(Qt::blue));
    VoltagePlot->setBrush(QBrush(QColor(2, 20, 2, 20)));
    VoltagePlot->setName("Action Potential");
    VoltagePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
//    VoltagePlot->setLineStyle(QCPCurve::lsLine);
#endif

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
    m_FirstRelax = true;

    ui->customPlot->axisRect()->setupFullAxesBox();
    // set some basic customPlot config:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->legend->setVisible(true); // Ensure legend is visible
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    ui->customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // Set a semi-transparent brush for the legend:
    // Set position to upper left inside the axis rect
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignTop);
    Reset();
}

void VoltageWindow::PlotBrackets(int32_t key, double coord1, double coord2, double y)
{
    switch(key)
    {
    case 0:
    {
    // add the bracket for the "Computing" phase:
    QCPItemBracket *computingBracket = new QCPItemBracket(ui->customPlot);
    computingBracket->left->setCoords(coord1, y);
    computingBracket->right->setCoords(coord2, y);
    computingBracket->setLength(13);

    // add the text label at the top:
    QCPItemText *computingText = new QCPItemText(ui->customPlot);
    computingText->position->setParentAnchor(computingBracket->center);
    computingText->position->setCoords(0, +20); // move 10 pixels to the top from bracket center anchor
    computingText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    computingText->setText("Computing");
    computingText->setRotation(-90);
    computingText->setFont(QFont(font().family(), 10));
    } break;
    case 1:
    {
    // add the bracket for the "Delivering" phase:
    QCPItemBracket *deliveringBracket = new QCPItemBracket(ui->customPlot);
    deliveringBracket->left->setCoords(coord2,y+10);
    deliveringBracket->right->setCoords(coord1, y+10);
    deliveringBracket->setLength(13);

    // add the text label at the top:
    QCPItemText *deliveringText = new QCPItemText(ui->customPlot);
    deliveringText->position->setParentAnchor(deliveringBracket->center);
    deliveringText->position->setCoords(0, +20); // move 10 pixels to the top from bracket center anchor
    deliveringText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    deliveringText->setText("Delivering");
    deliveringText->setRotation(-90);
    deliveringText->setFont(QFont(font().family(), 10));

    m_FirstRelax = false; //?? temp
    } break;
    case 2:
    {
    // add the bracket for the "Relaxing" phase:
        if(m_FirstRelax){   m_FirstRelax = false; break;}
    QCPItemBracket *relaxingBracket = new QCPItemBracket(ui->customPlot);
    relaxingBracket->left->setCoords(coord1, y+10);
    relaxingBracket->right->setCoords(coord2, y+10);
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
        if(GenCompStageMachine_t::gcsm_Delivering == m_neuron->StageFlag_Get())
            DrawItemText(key2, Volt2, "X", Qt::red);
        else
            DrawItemText(key2, Volt2, "X", Qt::green);
        // DrawArrow(Volt2,DvDt,  "X",-10,800);
    }

    if ( m_neuron->EVENT_GenComp.DeliveringBegin.triggered() ) {
//        DrawArrow(key2, Volt2, "<R",+0.068,18);
        DrawItemText(key2, Volt2, "<R", Qt::yellow);
        m_T_DeliveringBegin = key2;
        PlotBrackets(0,0.,key2,20);
    }
    if ( m_neuron->EVENT_GenComp.RelaxingBegin.triggered() ) {
//        DrawArrow(key2, Volt2, "R>",-0.05,18);
        DrawItemText(key2, Volt2, "R>", Qt::yellow);
        m_T_RelaxingBegin = key2;
        PlotBrackets(1,key2,m_T_DeliveringBegin, +20);
    }
    if ( m_neuron->EVENT_GenComp.RelaxingEnd.triggered() ) {
//        DrawArrow(key2, Volt2, "E",-0.05,18);
        DrawItemText(key2, Volt2, "E", Qt::yellow);
 //       PlotBrackets(2,m_T_RelaxingBegin,key2, m_V_Peak+40);
    }

    if(GenCompStageMachine_t::gcsm_Delivering == m_neuron->StageFlag_Get())
    {
        if ((m_neuron->dVdtResultingLast_Get() >=0) && (m_neuron->dVdtResulting_Get() < 0))
        {   // We are at the point of maximum polarization

            if(!m_HaveAlreadyP){
//                DrawArrow(key2, Volt2, "P",0.03,-30);
                DrawItemText(key2, Volt2, "P", Qt::yellow);
                m_HaveAlreadyP = true;
                m_V_Peak = Volt2;
            }
        }
    }
    if(GenCompStageMachine_t::gcsm_Relaxing == m_neuron->StageFlag_Get())
    {
        if ((m_neuron->dVdtResultingLast_Get() <0) && (m_neuron->dVdtResulting_Get() > 0))
        {   // We are at the point of maximum hyperpolarization
            if(!m_HaveAlreadyH){
//                DrawArrow(key2, Volt2, "H",-0,50);
                DrawItemText(key2, Volt2, "H", Qt::yellow); m_HaveAlreadyH = true;
                PlotBrackets(2,m_T_RelaxingBegin,key2, -20);
                }
        }
    }
    ui->customPlot->replot();
}

void VoltageWindow::DrawItemText(double xpos, double ypos, QString S, QColor Col)
{
    // add the text label at the top:
    QCPItemText *textLabel = new QCPItemText(ui->customPlot);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    //textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel->position->setCoords(xpos, ypos); // place position at center/top of axis rect
    textLabel->setText(S);
    textLabel->setFont(QFont(font().family(), 8)); // make font a bit larger
    textLabel->setPen(QPen(Col)); // show red border around text
    textLabel->setBrush(QBrush(Col)); // show red border around text
}

#if 0
void VoltageWindow::DrawArrow(double xpos, double ypos, QString S, double xoffset, double yoffset)
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
#endif
VoltageWindow::~VoltageWindow()
{
  delete ui;
}

void VoltageWindow::screenShot()
{
    QTime now = QTime::currentTime();
    QDate today = QDate::currentDate();
    QString fileName = //QString("screenshots/")+
        QString(m_neuron->name())+QString("_Voltage Plot_"+today.toString("yy.MM.dd") + QString("_") + now.toString("hh:mm:ss"))+QString(".pdf");
    fileName.replace(" ", "-");
    ui->customPlot->savePdf(fileName, 0, 0);
}

#include "moc_VoltageWindow.cpp"

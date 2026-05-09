/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2022 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: https://www.qcustomplot.com/                         **
**             Date: 06.11.22                                             **
**          Version: 2.1.1                                                **
****************************************************************************/

#include "VoltageWindow.h"
#include "ui_VoltageWindow.h"
#include <QDebug>
//#include <iostream>
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
                      //                     "title-color:  LightGray;"
                      "border-color:  LightGray;"
                      //                        "background-color: rgb(50,50, 150);"
                      //                        "border: 1px blue;"
                      "background-color:  LightGray;");
  setupRealtimeDataDemo(ui->customPlot);
   setWindowTitle(QString(m_neuron->name())+QString(" ActionPotential"));
  statusBar()->clearMessage();
  ui->actionScreenshot->setIcon(QIcon(":/icons/analytics.svg"));
  setupMenus();
   connect(ui->actionScreenshot, &QAction::triggered, this, &VoltageWindow::screenShot);
}

void VoltageWindow::setupMenus()
{
    const QIcon saveIcon = QIcon(":/icons/save.svg");
    auto *screenshotAction = new QAction(saveIcon, "Screenshot to File", this);
    ui->menuFile->addAction(screenshotAction);
    screenshotAction->setShortcut(QKeySequence::Save);
    connect(screenshotAction, &QAction::triggered, this,
            &VoltageWindow::screenShot);
}



void VoltageWindow::replot(void)
{ui->customPlot->replot();}

void VoltageWindow::Reset()
{
    dataVoltagePlot.clear(); RunningPointPosition_Set(0,0); index = 0;
    VoltagePlot->data()->set(dataVoltagePlot, true);
    replot();
}

void VoltageWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
    double key2 = m_neuron->LocalTimeInMillisec_Get()*2.4;
    double Volt2 = m_neuron->MembraneRelativePotential_Get()*15;
    // Add an ellipse
    RunningPoint = new QCPItemEllipse(ui->customPlot);
    RunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
    RunningPoint->setPen(QPen(Qt::red));
    RunningPointPosition_Set(key2,Volt2);

    VoltagePlot = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    VoltagePlot->setName("ActionPotential");
    VoltagePlot->data()->set(dataVoltagePlot, true);
    VoltagePlot->setPen(QPen(Qt::blue));
    VoltagePlot->setBrush(QBrush(QColor(2, 20, 20, 20)));
    VoltagePlot->setLineStyle(QCPCurve::lsLine);
    VoltagePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    VoltagePlot->setLineStyle(QCPCurve::lsLine);
    VoltagePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

    // give the axes some labels:
    customPlot->xAxis->setLabel("Time (ms)");
    customPlot->yAxis->setLabel("Membrane voltage (mV)");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0,1);
    customPlot->yAxis->setRange(-30,130);
    // set some basic customPlot config:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->rescaleAxes();

  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(realtimeDataSlot()));
  customPlot->axisRect()->setupFullAxesBox();
  Reset();
}

void VoltageWindow::RunningPointPosition_Set(double xpos, double ypos)
{
    RunningPoint->topLeft->setCoords(xpos-0.01, ypos-1);    // Set coordinates
    RunningPoint->bottomRight->setCoords(xpos+0.01, ypos+1);
}

void VoltageWindow::realtimeDataSlot()
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
        DrawArrow(key2, Volt2, "X",0.05,-10);
    }
    if ( m_neuron->EVENT_GenComp.DeliveringBegin.triggered() ) {
        DrawArrow(key2, Volt2, "R<",+0.072,10);
    }
    if ( m_neuron->EVENT_GenComp.RelaxingBegin.triggered() ) {
        DrawArrow(key2, Volt2, ">R",-0.071,8);
    }

    if(GenCompStageMachine_t::gcsm_Delivering == m_neuron->StageFlag_Get())
    {
        if ((m_neuron->dVdtResultingLast_Get() >=0) && (m_neuron->dVdtResulting_Get() < 0))
        {   // We are at the point of maximum polarization
            DrawArrow(key2, Volt2, "P",0.05,-65);
        }
    }
    if(GenCompStageMachine_t::gcsm_Relaxing == m_neuron->StageFlag_Get())
    {
    //    cerr << m_neuron->dVdtResultingLast_Get() << "," << m_neuron->dVdtResulting_Get() << "\n";
        if ((m_neuron->dVdtResultingLast_Get() <0) && (m_neuron->dVdtResulting_Get() >= 0))
        {   // We are at the point of maximum polarization
            DrawArrow(key2, Volt2, "H",-0,50);
        }

    }

    ui->customPlot->replot();
}

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

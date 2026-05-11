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

#include "CurrentWindow.h"
#include "ui_CurrentWindow.h"
#include <QDebug>
//#include <iostream>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QApplication>

#include <QFile>

CurrentWindow::CurrentWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent ):
  QMainWindow(parent),
  ui(new Ui::CurrentWindow),
    m_Simulator(Simulator),
    m_neuron(Neuron)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
  this->setStyleSheet("color: Navy;"
                      "border: 1px solid white;"
                      "border-color:  LightGray;"
                      "background-color:  LightGray;"
                      "border: 1px solid white;"
  "border-width: 1px;"
  "border-style: solid;"
  "border-radius: 4px;"
);
  setupDataPlot();
   setWindowTitle(QString(m_neuron->name())+QString(" membrane currents"));
  setupMenus();
   connect(ui->actionScreenshot, &QAction::triggered, this, &CurrentWindow::screenShot);
}

void CurrentWindow::setupMenus()
{
    const QIcon saveIcon = QIcon(":/icons/save.svg");
    auto *screenshotAction = new QAction(saveIcon, "Screenshot to File", this);
    ui->menuFile->addAction(screenshotAction);
    screenshotAction->setShortcut(QKeySequence::Save);
}


void CurrentWindow::replot(void)
{ui->customPlot->replot();}

void CurrentWindow::Reset()
{
    dataNaCurrentPlot.clear(); NaRunningPointPosition_Set(0,0);
    NaCurrentPlot->data()->set(dataNaCurrentPlot, true);
    dataAISCurrentPlot.clear(); AISRunningPointPosition_Set(0,0);
    AISCurrentPlot->data()->set(dataAISCurrentPlot, true);

    dataResultingCurrentPlot.clear(); ResultingRunningPointPosition_Set(0,0);
    ResultingCurrentPlot->data()->set(dataResultingCurrentPlot, true);

    index = 0;// replot();
}

void CurrentWindow::setupDataPlot()
{
    double key2 = m_neuron->LocalTimeInMillisec_Get();
    double Volt2 = m_neuron->MembraneRelativePotential_Get();
/*    double I_AIS = m_neuron->I_AIS_Get();
*/    // Add an ellipse
    AISRunningPoint = new QCPItemEllipse(ui->customPlot);
    AISRunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
    AISRunningPoint->setPen(QPen(Qt::red));
    AISRunningPointPosition_Set(key2,Volt2);
    // Add an ellipse
    NaRunningPoint = new QCPItemEllipse(ui->customPlot);
    NaRunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
    NaRunningPoint->setPen(QPen(Qt::green));
    NaRunningPointPosition_Set(key2,Volt2);

    AISCurrentPlot = new QCPCurve(ui->customPlot->xAxis,ui->customPlot->yAxis);
    AISCurrentPlot->data()->set(dataAISCurrentPlot, true);
    AISCurrentPlot->setPen(QPen(Qt::red));
    AISCurrentPlot->setBrush(QBrush(QColor(20, 2, 2, 20)));
    AISCurrentPlot->setLineStyle(QCPCurve::lsLine);
    AISCurrentPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    AISCurrentPlot->setName("AIS (K+)");

    NaCurrentPlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    NaCurrentPlot->data()->set(dataNaCurrentPlot, true);
    NaCurrentPlot->setPen(QPen(Qt::green));
    NaCurrentPlot->setBrush(QBrush(QColor(2, 20, 2, 20)));
    NaCurrentPlot->setLineStyle(QCPCurve::lsLine);
    NaCurrentPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    NaCurrentPlot->setName("Na+");

    ResultingCurrentPlot = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ResultingCurrentPlot->data()->set(dataResultingCurrentPlot, true);
    ResultingCurrentPlot->setPen(QPen(Qt::blue));
    ResultingCurrentPlot->setBrush(QBrush(QColor(2, 2, 20, 20)));
    ResultingCurrentPlot->setLineStyle(QCPCurve::lsLine);
    ResultingCurrentPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    ResultingCurrentPlot->setName("Resulting");

    ui->customPlot->legend->setVisible(true); // Ensure legend is visible
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Time (ms)");
    ui->customPlot->yAxis->setLabel("Membrane currents (pA)");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0,1);
    ui->customPlot->yAxis->setRange(-50,100);
    // set some basic customPlot config:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->rescaleAxes();

//  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(realtimeDataSlot())); //Functionality moved to MainWindow
  ui->customPlot->axisRect()->setupFullAxesBox();
  Reset();
}

void CurrentWindow::AISRunningPointPosition_Set(double xpos, double ypos)
{
    AISRunningPoint->topLeft->setCoords(xpos-0.01, ypos-1);    // Set coordinates
    AISRunningPoint->bottomRight->setCoords(xpos+0.01, ypos+1);
}

void CurrentWindow::NaRunningPointPosition_Set(double xpos, double ypos)
{
    NaRunningPoint->topLeft->setCoords(xpos-0.01, ypos-1);    // Set coordinates
    NaRunningPoint->bottomRight->setCoords(xpos+0.01, ypos+1);
}

void CurrentWindow::ResultingRunningPointPosition_Set(double xpos, double ypos)
{
    /*
    ResultingRunningPoint->topLeft->setCoords(xpos-0.01, ypos-1);    // Set coordinates
    ResultingRunningPoint->bottomRight->setCoords(xpos+0.01, ypos+1);
*/
}


void CurrentWindow::displayDataSlot()
{

    double Volt2 = m_neuron->MembraneRelativePotential_Get();
    double key2 = m_neuron->LocalTimeInMillisec_Get();
    double I_AIS = -m_neuron->I_AIS_Get();
    double I_Na = m_neuron->I_Na_Get();
    double I_Resulting = m_neuron->I_Resulting_Get();
    if(index>0)
    {
        double OldTime = dataAISCurrentPlot[index-1].key;
        if(OldTime>key2)
        {// We step back on the time scale; reset plot
            Reset(); index = 0;
        }
    }
    AISRunningPointPosition_Set(key2,I_AIS);
    dataAISCurrentPlot.push_back(QCPCurveData(index,key2,I_AIS));
    AISCurrentPlot->data()->set(dataAISCurrentPlot, true);
    AISCurrentPlot->setPen(QPen(Qt::red));
    AISCurrentPlot->setBrush(QBrush(QColor(20, 2, 2, 20)));

    NaRunningPointPosition_Set(key2,I_Na);
    dataNaCurrentPlot.push_back(QCPCurveData(index,key2,I_Na));
    NaCurrentPlot->data()->set(dataNaCurrentPlot, true);
    NaCurrentPlot->setPen(QPen(Qt::green));
    NaCurrentPlot->setBrush(QBrush(QColor(2, 20, 2, 20)));
/*
    ResultingRunningPointPosition_Set(key2,I_Resulting);
*/
    dataResultingCurrentPlot.push_back(QCPCurveData(index,key2,I_Resulting));

    ResultingCurrentPlot->data()->set(dataResultingCurrentPlot, true);
    ResultingCurrentPlot->setPen(QPen(Qt::blue));
    ResultingCurrentPlot->setBrush(QBrush(QColor(2, 2, 20, 20)));

    index++;
    ui->customPlot->replot();
}


CurrentWindow::~CurrentWindow()
{
  delete ui;
}

void CurrentWindow::screenShot()
{
    QTime now = QTime::currentTime();
    QString fileName = //QString("screenshots/")+
        QString(m_neuron->name())+QString("_Current Plot_"+now.toString("hh:mm:ss"))+QString(".pdf");
    fileName.replace(" ", "");
    ui->customPlot->savePdf(fileName, 0, 0);
}


#include "moc_CurrentWindow.cpp"

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

#include "GradientWindow.h"
#include "ui_GradientWindow.h"
#include <QDebug>
#include <iostream>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QApplication>

#include <QFile>

GradientWindow::GradientWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent ):
  QMainWindow(parent),
  ui(new Ui::GradientWindow),
    m_Simulator(Simulator),
    m_neuron(Neuron)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
  setupRealtimeDataDemo(ui->customPlot);
   setWindowTitle(QString(m_neuron->name())+QString(" voltage gradient"));
  statusBar()->clearMessage();
   ui->actionScreenshot->setIcon(QIcon(":/icons/analytics.svg"));
  this->setStyleSheet("color: Navy;"
 //                     "title-color:  LightGray;"
                       "border-color:  LightGray;"
                      //                        "background-color: rgb(50,50, 150);"
                      //                        "border: 1px blue;"
                      "background-color:  LightGray;");

   connect(ui->actionScreenshot, &QAction::triggered, this, &GradientWindow::screenShot);

  statusBar()->showMessage( QString("Ready to go"));
//  ui->customPlot->replot();
  realtimeDataSlot();
//  QTimer::singleShot(4000, this, SLOT(screenShot()));
}


void GradientWindow::ProcessLine(QString line)
{
    QStringList firstColumn;
     // Handle the first two items only
//    while (!s1.atEnd()){
    first.append(line.split(",").at(0)); // appends first column to list, ',' is separator
    second.append(line.split(",").at(1));
    Time.push_back(line.split(",").at(0).toDouble());
    Gradient.push_back(line.split(",").at(1).toDouble());
 //    std::cout << line.toStdString() << '\n';
}
void GradientWindow::GetData(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line = in.readLine();   // Skip the heading line
    while (!in.atEnd()) {
        line = in.readLine();
        ProcessLine(line);
    }
}

void GradientWindow::replot(void)
{ui->customPlot->replot();}

// Fill area between graph 0 and graph 1
/* customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));
customPlot->graph(0)->setBrush(QBrush(QColor(20, 20, 20, 20)));
*/

void GradientWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
 /*   double Volt2 = m_neuron->MembraneRelativePotential_Get()*15;
    double DvDt = m_neuron->dVdtResulting_Get()/100.;
    double Time = m_neuron->LocalTimeInMillisec_Get()*2.4;
*/
    // Add an ellipse
    RunningPoint = new QCPItemEllipse(ui->customPlot);
    RunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
    RunningPoint->setPen(QPen(Qt::red));
    AISRunningPoint  = new QCPItemEllipse(ui->customPlot);
    RushinRunningPoint  = new QCPItemEllipse(ui->customPlot);

    RushinGradientPlot = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    RushinGradientPlot->setName("Rushin gradient");
    AISGradientPlot = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    AISGradientPlot->setName("AIS gradient");
    GradientPlot = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    GradientPlot->setName("Resulting");

    customPlot->legend->setVisible(true); // Ensure legend is visible
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // Set a semi-transparent brush for the legend:
    // Set position to upper left inside the axis rect
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignTop);

#if 0
    // add two new graphs and set their look:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->customPlot->graph(0)->setName("Resulting gradient");

    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    ui->customPlot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20))); // first graph will be filled with translucent blue
    ui->customPlot->graph(1)->setName("AIS gradient");

    ui->customPlot->addGraph();
    ui->customPlot->graph(2)->setPen(QPen(Qt::green)); // line color red for second graph
    ui->customPlot->graph(2)->setBrush(QBrush(QColor(0, 255, 0, 20))); // first graph will be filled with translucent blue
    ui->customPlot->graph(2)->setName("Rushin gradient");
#endif
    // generate some points of data (y0 for first, y1 for second graph):
#if 0
    for (int i=0; i<251; ++i)
    {
        x.push_back( i);
        y0.push_back(qExp(-i/150.0)*qCos(i/10.0)); // exponentially decaying cosine
        y1.push_back(qExp(-i/150.0));              // exponential envelope
    }
#endif
    index = 0;
/*    ui->customPlot->graph(0)->setData(x, y0);
    ui->customPlot->graph(1)->setData(x, y1);
    // create graph and assign data to it:
    customPlot->graph(0)->setData(x, y0);
*/    // give the axes some labels:
    customPlot->xAxis->setLabel("Time (ms)");
    customPlot->yAxis->setLabel("Membrane gradient (V/m)");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0,1);
    customPlot->yAxis->setRange(-100,200);
    // set some basic customPlot config:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->rescaleAxes();

  demoName = "Single AP draw demo";
#if 0

    GetData("/home/jvegh/REPO/LaTeX/figures/AP_Simulation/AP_0_offset.csv");
  // include this section to fully disable antialiasing for higher performance:
    customPlot->legend->setVisible(true); // Ensure legend is visible
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // Set a semi-transparent brush for the legend:
    // Set position to upper left inside the axis rect
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);
    // Optional: Add a slight margin so it doesn't touch the edge
//??    customPlot->axisRect()->insetLayout()->setInsetMargins(0, QMargins(10, 10, 10, 10));


  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */

  index = 0;

  // setup a timer that repeatedly calls VoltageWindow::realtimeDataSlot:
#endif
  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(realtimeDataSlot()));
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->replot();
/*  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(10); // Interval 0 means to refresh as fast as possible
*/
}


void GradientWindow::realtimeDataSlot()
{
    double key2 = m_neuron->LocalTimeInMillisec_Get()*2.4;
    double DvDt = m_neuron->dVdtResulting_Get();
    double Membrane_dVdt_AIS = m_neuron->dVdtAIS_Get();
    double Membrane_dVdt_Input = m_neuron->dVdtInput_Get();
//    std::cerr << key2 << "," <<  DvDt << "," << Membrane_dVdt_AIS << "," << Membrane_dVdt_Input << '\n';
//    uint64_t NCP = Gradient.count();
    //    QVector<QCPCurveData> dataPhasePlot(NCP);
    // Handle resultant gradient display
    dataGradientPlot.push_back(QCPCurveData(index,key2, DvDt));
    GradientPlot->data()->set(dataGradientPlot, true);
    GradientPlot->setPen(QPen(Qt::blue));
    GradientPlot->setBrush(QBrush(QColor(2, 2, 20, 20)));
    RunningPoint->topLeft->setCoords(-0.01+key2, DvDt-.5);    // Set coordinates
    RunningPoint->bottomRight->setCoords(0.01+key2, DvDt+5);

    // Handle AIS gradient display
    dataAISGradientPlot.push_back(QCPCurveData(index,key2, Membrane_dVdt_AIS));
    AISGradientPlot->data()->set(dataAISGradientPlot, true);
    AISGradientPlot->setPen(QPen(Qt::red));
    AISGradientPlot->setBrush(QBrush(QColor(20, 2, 2, 20)));
    AISRunningPoint->topLeft->setCoords(-0.01+key2, Membrane_dVdt_AIS-.5);    // Set coordinates
    AISRunningPoint->bottomRight->setCoords(0.01+key2, Membrane_dVdt_AIS+5);

    // Handle Rush-in gradient display
    dataRushinGradientPlot.push_back(QCPCurveData(index,key2, Membrane_dVdt_Input));
    RushinGradientPlot->data()->set(dataRushinGradientPlot, true);
    RushinGradientPlot->setPen(QPen(Qt::green));
    RushinGradientPlot->setBrush(QBrush(QColor(2, 20, 2, 20)));
    RushinRunningPoint->topLeft->setCoords(-0.01+key2, Membrane_dVdt_Input-.5);    // Set coordinates
    RushinRunningPoint->bottomRight->setCoords(0.01+key2, Membrane_dVdt_Input+5);

    index++;
    ui->customPlot->replot();
}


GradientWindow::~GradientWindow()
{
  delete ui;
}

void GradientWindow::screenShot()
{
    QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
    QScreen *screen1 = qApp->primaryScreen();
    QPixmap pixmap = screen1->grabWindow(0);
/*    const QRect screenGeometry = screen()->geometry();
    QPixmap pm = grabWindow(0, screenGeometry.x(), screenGeometry.y(), screenGeometry.width()+14, screenGeometry.height()+14);
*/    QString fileName = demoName.toLower()+".pdf";

    fileName.replace(" ", "");
  ui->customPlot->savePdf(fileName, 0, 0);

//  pm.save("./screenshots/"+fileName);
//  pm.save(fileName);
//  qApp->quit();
}


#include "moc_GradientWindow.cpp"

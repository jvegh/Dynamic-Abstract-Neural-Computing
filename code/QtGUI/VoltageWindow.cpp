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
#include <iostream>
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
  setupRealtimeDataDemo(ui->customPlot);
   setWindowTitle(QString(m_neuron->name())+QString(" ActionPotential"));
  statusBar()->clearMessage();
  statusBar()->showMessage( QString("Ready to go"));
//  ui->customPlot->replot();
// realtimeDataSlot();
//  QTimer::singleShot(4000, this, SLOT(screenShot()));
}


void VoltageWindow::ProcessLine(QString line)
{
    QStringList firstColumn;
     // Handle the first two items only
//    while (!s1.atEnd()){
    first.append(line.split(",").at(0)); // appends first column to list, ',' is separator
    second.append(line.split(",").at(1));
    Time.push_back(line.split(",").at(0).toDouble());
    Voltage.push_back(line.split(",").at(1).toDouble());
 //    std::cout << line.toStdString() << '\n';
}
void VoltageWindow::GetData(QString fileName)
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

// Fill area between graph 0 and graph 1
/* customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));
customPlot->graph(0)->setBrush(QBrush(QColor(20, 20, 20, 20)));
*/

void VoltageWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{

    double Volt2 = m_neuron->MembraneRelativePotential_Get()*15;
    double DvDt = m_neuron->dVdtResulting_Get()/100.;
    double Time = m_neuron->LocalTimeInMillisec_Get()*2.4;
    // Add an ellipse
    RunningPoint = new QCPItemEllipse(ui->customPlot);
    RunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
    RunningPoint->setPen(QPen(Qt::red));
    RunningPoint->topLeft->setCoords(-0.0001+Time, Volt2-5);    // Set coordinates
    RunningPoint->bottomRight->setCoords(0.0001+Time, Volt2+5);

    VoltagePlot = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
//    uint64_t NCP = Gradient.count();
    //    QVector<QCPCurveData> dataPhasePlot(NCP);
    dataVoltagePlot.push_back(QCPCurveData(index,Volt2, DvDt));
    index++;
    VoltagePlot->data()->set(dataVoltagePlot, true);
    VoltagePlot->setPen(QPen(Qt::blue));
    VoltagePlot->setBrush(QBrush(QColor(2, 20, 20, 20)));


    // add two new graphs and set their look:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    // generate some points of data (y0 for first, y1 for second graph):

    index = 0;
    ui->customPlot->graph(0)->setData(x, y0);
    ui->customPlot->graph(1)->setData(x, y1);
    // create graph and assign data to it:
    customPlot->graph(0)->setData(x, y0);
    // give the axes some labels:
    customPlot->xAxis->setLabel("Time (ms)");
    customPlot->yAxis->setLabel("Membrane voltage (mV)");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0,1);
    customPlot->yAxis->setRange(-30,100);
    // set some basic customPlot config:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->rescaleAxes();

#if 0
  demoName = "Single AP draw demo";

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
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setName("ActionPotential");
  customPlot->graph(0)->setPen(QPen(QColor(255, 110, 40)));
  customPlot->graph(0)->setLineStyle((QCPGraph::LineStyle)1);

  customPlot->addGraph(); // red line
  customPlot->graph(1)->setPen(QPen(QColor(40, 110, 255)));
  customPlot->graph(1)->setName("dV/dt gradient");
  customPlot->graph(1)->setLineStyle((QCPGraph::LineStyle)1);

  customPlot->addGraph(customPlot->xAxis,customPlot->yAxis2); // green line
  customPlot->graph(2)->setPen(QPen(QColor(31, 127, 31)));
  customPlot->graph(2)->setName("Another");
  customPlot->graph(2)->setLineStyle((QCPGraph::LineStyle)1);
  index = 0;

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%s:%z"); //"%m:%s:%Z"
  customPlot->xAxis->setTicker(timeTicker);
  customPlot->xAxis->setLabel("Time (ms)");
  customPlot->yAxis->setRange(-30, 110);
  customPlot->yAxis->setLabel("Voltage (mV)");
  // Create voltage gradient window

  /*
  // Setup the second y axis
  customPlot->yAxis2->setVisible(true); // Ensure second axis is visible
  customPlot->yAxis2->setLabel("dV/dt (V/m)");

  QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
  fixedTicker->setTickStep(10); //
  fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssMultiples);
  customPlot->yAxis2->setTicker(fixedTicker);
  customPlot->yAxis2->setTickLabels(true);


   customPlot->yAxis2->setRange(-100, 200);
  customPlot->yAxis2->setLabelColor(Qt::blue);
  customPlot->yAxis2->setTickLabelColor(Qt::blue);
  //??customPlot->yAxis2->setRotation (180);

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
 */
  // setup a timer that repeatedly calls VoltageWindow::realtimeDataSlot:
#endif
  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(realtimeDataSlot()));
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->replot();
/*  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(10); // Interval 0 means to refresh as fast as possible
*/
}


void VoltageWindow::realtimeDataSlot()
{

    double Volt2 = m_neuron->MembraneRelativePotential_Get()*15;
    double DvDt = m_neuron->dVdtResulting_Get()/100.;
    double key2 = m_neuron->LocalTimeInMillisec_Get()*2.4;
    RunningPoint->topLeft->setCoords(-0.05+key2, Volt2-.5);    // Set coordinates
    RunningPoint->bottomRight->setCoords(0.05+key2, Volt2+5);
    uint64_t NCP = Gradient.count();
    //    QVector<QCPCurveData> dataPhasePlot(NCP);
    dataVoltagePlot.push_back(QCPCurveData(index,key2,Volt2));
    index++;
    VoltagePlot->data()->set(dataVoltagePlot, true);
    VoltagePlot->setPen(QPen(Qt::blue));
    VoltagePlot->setBrush(QBrush(QColor(2, 20, 20, 20)));

    ui->customPlot->replot();

#if 0
    if (0==index)
    {
    x.clear(); y0.clear(); //y1.clear();
    }
//     for (int i=0; i <= index; ++i)
    {
        double key2 = m_neuron->LocalTimeInMillisec_Get()*2.4;
        double Volt2 = m_neuron->MembraneRelativePotential_Get()*15;
        dataVoltagePlot.push_back(QCPCurveData(index,key2,Volt2));
     }
     index ++;
    ui->customPlot->graph(0)->setData(x, y0);
 //   ui->customPlot->graph(1)->setData(x, y1);
 //   ui->customPlot->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
//    ui->customPlot->graph(1)->rescaleAxes(true);
    ui->customPlot->replot();
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
#endif

#if 0
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
#endif

#if 0
    // pass data points to graphs:
     // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->customPlot->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    ui->customPlot->graph(1)->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
#endif
#if 0
//  static QTime timeStart = QTime::currentTime();
  // calculate two new data points:
double Volt;
    if(!Voltage.count()) return;
   Volt = Voltage[index];
  double key = Time[index++];
//  double key2 = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
  double key2 = m_neuron->LocalTimeInMillisec_Get()*2.4;
  double Volt2 = m_neuron->MembraneRelativePotential_Get();
  double DvDt = m_neuron->dVdtResulting_Get();
  if(index>=Voltage.count())
      index = 0;
   static double lastPointKey = 0;
  if (key-lastPointKey > 0.005) // at most add point every 5 us
  {
    // add data to lines:
    //  Time.push_back(line.split(",").at(0).toDouble());
    //  Voltage.push_back(line.split(",").at(1).toDouble());


      ui->customPlot->graph(0)->addData(key, Volt);
    ui->customPlot->graph(1)->addData(key2, 17*Volt2);
      ui->customPlot->graph(2)->addData(key2, DvDt/2);
     // rescale value (vertical) axis to fit the current data:
    ui->customPlot->graph(0)->rescaleValueAxis();
    ui->customPlot->graph(1)->rescaleValueAxis(true);
    ui->customPlot->graph(2)->rescaleValueAxis(true);
    lastPointKey = key;
//    if(index>sizeof(Voltage))
//        Voltage.clear();
  }
  // make key axis range scroll with the data (at a constant range size of 10):
  ui->customPlot->xAxis->setRange(key, 1., Qt::AlignRight);
  ui->customPlot->replot();
  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 1) // average fps over 2 seconds
  {
    statusBar()->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->customPlot->graph(0)->data()->size()
//                   +ui->customPlot->graph(1)->data()->size()
                   )
          , 1000);
    lastFpsKey = key;
    frameCount = 0;
  }
#endif
}


VoltageWindow::~VoltageWindow()
{
  delete ui;
}

void VoltageWindow::screenShot()
{
  QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x[0]-7, this->y0[0]-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
    QString fileName = demoName.toLower()+".pdf";
  fileName.replace(" ", "");
  ui->customPlot->savePdf(fileName, 0, 0);

//  pm.save("./screenshots/"+fileName);
//  pm.save(fileName);
//  qApp->quit();
}

#include "moc_VoltageWindow.cpp"

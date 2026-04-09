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
  setupRealtimeDataDemo(ui->customPlot);
 // setWindowTitle("QCustomPlot: "+demoName);
  setWindowTitle(QString(m_neuron->name())+QString(" electrical parameters"));
  setupMenus();
  statusBar()->clearMessage();
  statusBar()->showMessage( QString("Ready to go"));
//  ui->customPlot->replot();
  realtimeDataSlot();
   //QTimer::singleShot(1500, this, SLOT(allScreenShots()));
//  QTimer::singleShot(4000, this, SLOT(screenShot()));
}

void VoltageWindow::setupMenus()
{
    const QIcon saveIcon = QIcon(":/icons/save.svg");
    auto *screenshotAction = new QAction(saveIcon, "Screenshot to File", this);
    ui->menuFile->addAction(screenshotAction);
    screenshotAction->setShortcut(QKeySequence::Save);
    connect(screenshotAction, &QAction::triggered, this,
            &VoltageWindow::screenshotFilesTriggered);
}

void VoltageWindow::screenshotFilesTriggered() {
    QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
    QString fileName = QString(m_neuron->name())+QString(" Voltage Plot")+".pdf";
    fileName.replace(" ", "");
    ui->customPlot->savePdf(fileName, 0, 0);

    /*
        static_cast<EditTab *>(m_tabWidgets.at(EditTabID).tab)->getSourceType());
    if (!RipesSettings::value(RIPES_SETTING_HAS_SAVEFILE).toBool()) {
        saveFilesAsTriggered();
        return;
    }

    emit prepareSave();
    QStringList savedFiles;
    if (!diag.sourcePath().isEmpty()) {
        if (!ensurePath(diag.sourcePath()))
            return;
        QFile file(diag.sourcePath());
        savedFiles << diag.sourcePath();
        if (!writeTextFile(file,
                           static_cast<EditTab *>(m_tabWidgets.at(EditTabID).tab)
                               ->getAssemblyText())) {
            QMessageBox::information(this, "File error",
                                     "Error when saving file: " + file.errorString());
            return;
        }
    }
     }
*/
    statusBar()->showMessage( QString("Screenshot saved to "+fileName));
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
  
  // setup a timer that repeatedly calls VoltageWindow::realtimeDataSlot:
  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(realtimeDataSlot()));
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->replot();
/*  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(10); // Interval 0 means to refresh as fast as possible
*/
}


void VoltageWindow::realtimeDataSlot()
{
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
}


VoltageWindow::~VoltageWindow()
{
  delete ui;
}

void VoltageWindow::screenShot()
{
  QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
    QString fileName = demoName.toLower()+".pdf";
  fileName.replace(" ", "");
  ui->customPlot->savePdf(fileName, 0, 0);

//  pm.save("./screenshots/"+fileName);
//  pm.save(fileName);
//  qApp->quit();
}

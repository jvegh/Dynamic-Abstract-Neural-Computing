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
            &VoltageWindow::screenshotFilesTriggered);
}

void VoltageWindow::screenshotFilesTriggered() {
 //   QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->Time()-7, this->Voltage()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
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
    VoltagePlot->data()->set(dataVoltagePlot, true);
    VoltagePlot->setPen(QPen(Qt::blue));
    VoltagePlot->setBrush(QBrush(QColor(2, 20, 20, 20)));
    VoltagePlot->setLineStyle(QCPCurve::lsLine);
    VoltagePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

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
  QPixmap pm = qGuiApp->primaryScreen()->grabWindow(0, this->x[0]-7, this->y0[0]-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
    QString fileName = demoName.toLower()+".pdf";
  fileName.replace(" ", "");
  ui->customPlot->savePdf(fileName, 0, 0);

//  pm.save("./screenshots/"+fileName);
//  pm.save(fileName);
//  qApp->quit();
}

#include "moc_VoltageWindow.cpp"

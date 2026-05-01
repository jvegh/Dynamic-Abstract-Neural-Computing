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

  Reset();
 //  ui->customPlot->replot();
//  realtimeDataSlot();
//  QTimer::singleShot(4000, this, SLOT(screenShot()));
}

void GradientWindow::replot(void)
{ui->customPlot->replot();}


void GradientWindow::Reset()
{
    dataRushinGradientPlot.clear(); RushinRunningPointPositionGradient_Set(0,0);
    dataAISGradientPlot.clear(); AISRunningPointPositionGradient_Set(0,0);
    dataGradientPlot.clear();  RunningPointPositionGradient_Set(0,0);
    RushinGradientPlot->data()->set(dataRushinGradientPlot, true);
    AISGradientPlot->data()->set(dataAISGradientPlot, true);
    GradientPlot->data()->set(dataGradientPlot, true);
    index = 0; replot();
}




// Fill area between graph 0 and graph 1
/* customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));
customPlot->graph(0)->setBrush(QBrush(QColor(20, 20, 20, 20)));
*/

void GradientWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
     // Add ellipses
    RunningPoint = new QCPItemEllipse(ui->customPlot);
    RunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
    RunningPoint->setPen(QPen(Qt::red));
    AISRunningPoint  = new QCPItemEllipse(ui->customPlot);
    RushinRunningPoint  = new QCPItemEllipse(ui->customPlot);

    RushinGradientPlot = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    RushinGradientPlot->setName("Rushin gradient");
    RushinGradientPlot->setLineStyle(QCPCurve::lsLine);
    RushinGradientPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

    AISGradientPlot = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    AISGradientPlot->setName("AIS gradient");
    AISGradientPlot->setLineStyle(QCPCurve::lsLine);
    AISGradientPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

    GradientPlot = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    GradientPlot->setName("Resulting gradient");
    GradientPlot->setLineStyle(QCPCurve::lsLine);
    GradientPlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

    GradientPlot->setPen(QPen(Qt::blue));
    GradientPlot->setBrush(QBrush(QColor(2, 2, 20, 20)));
    AISGradientPlot->setPen(QPen(Qt::red));
    AISGradientPlot->setBrush(QBrush(QColor(20, 2, 2, 20)));
    RushinGradientPlot->setPen(QPen(Qt::green));
    RushinGradientPlot->setBrush(QBrush(QColor(2, 20, 2, 20)));

    // Fill area between graph 0 and graph 1
/*     GradientPlot->setChannelFillGraph(customPlot->graph(1));
customPlot->graph(0)->setBrush(QBrush(QColor(20, 20, 20, 20)));
*/

    customPlot->legend->setVisible(true); // Ensure legend is visible
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // Set a semi-transparent brush for the legend:
    // Set position to upper left inside the axis rect
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignTop);

    index = 0;
    // give the axes some labels:
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

  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
#endif
  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(realtimeDataSlot()));
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->replot();
}


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

void GradientWindow::realtimeDataSlot()
{
    double key2 = m_neuron->LocalTimeInMillisec_Get()*2.4;
    double DvDt = m_neuron->dVdtResulting_Get();
    double Membrane_dVdt_AIS = m_neuron->dVdtAIS_Get();
    double Membrane_dVdt_Input = m_neuron->dVdtInput_Get();
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
    ui->customPlot->replot();
}


GradientWindow::~GradientWindow()
{
  delete ui;
}

 void GradientWindow::screenShot()
{

    QTime now = QTime::currentTime();
    qDebug() << "Current time:" << now.toString("hh:mm:ss");
    QString fileName = QString("~/screenshots/")+ QString(m_neuron->name())+QString(" graddieents plot"+now.toString("hh:mm:ss")+QString(".pdf"));
    fileName.replace(" ", "");
    if(ui->customPlot->savePdf(fileName))
      statusBar()->showMessage(fileName+QString(" prepared"));
    else
        statusBar()->showMessage("Saving to .pdf file failed");
}

/*
    QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
    QScreen *QS = qApp->primaryScreen();
    QScreen *screen1 = qApp->primaryScreen();
    QPixmap pixmap = screen1->grabWindow(0, this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
    const QRect screenGeometry = screen()->geometry();
    QPixmap pm = grabWindow(0, screenGeometry.x(), screenGeometry.y(), screenGeometry.width()+14, screenGeometry.height()+14);
*/
//  pm.save("./screenshots/"+fileName);
//  pm.save(fileName);
//  qApp->quit();

#if 0
void GradientWindow::screenShot()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QPixmap pm = QPixmap::grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
#elif QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
  QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
#elif QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
#else
  QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
#endif
  QString fileName = demoName.toLower()+".png";
  fileName.replace(" ", "");
  pm.save("./screenshots/"+fileName);
  qApp->quit();
}
#endif


#include "moc_GradientWindow.cpp"

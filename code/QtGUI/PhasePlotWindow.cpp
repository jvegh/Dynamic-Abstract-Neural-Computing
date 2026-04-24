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

#include "PhasePlotWindow.h"
#include "ui_PhasePlotWindow.h"
#include <QDebug>
#include <iostream>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
//#include <QApplication>

#include <QFile>

PhasePlotWindow::PhasePlotWindow(ScQtSimulator *Simulator,  NeuronPhysical *Neuron, QWidget *parent ):
  QMainWindow(parent),
  ui(new Ui::PhasePlotWindow),
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
 // setWindowTitle("QCustomPlot: "+demoName);
  setWindowTitle(QString(m_neuron->name())+QString(" phase plot"));
  setupMenus();
  statusBar()->clearMessage();
  statusBar()->showMessage( QString("Ready to go"));

  realtimeDataSlot();
   //QTimer::singleShot(1500, this, SLOT(allScreenShots()));
//  QTimer::singleShot(4000, this, SLOT(screenShot()));
}

void PhasePlotWindow::setupMenus()
{
    const QIcon saveIcon = QIcon(":/icons/save.svg");
    auto *screenshotAction = new QAction(saveIcon, "Screenshot to File", this);
    ui->menuFile->addAction(screenshotAction);
    screenshotAction->setShortcut(QKeySequence::Save);
    connect(screenshotAction, &QAction::triggered, this,
            &PhasePlotWindow::screenshotFilesTriggered);
}
    // Edit actions

void PhasePlotWindow::screenshotFilesTriggered() {
    QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
    QString fileName = QString(m_neuron->name())+QString(" phase plot")+".pdf";
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


void PhasePlotWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
//    GetData("/home/jvegh/REPO/LaTeX/figures/AP_Simulation/AP_0_offset.csv");
  // include this section to fully disable antialiasing for higher performance:
    customPlot->legend->setVisible(true); // Ensure legend is visible
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // Set a semi-transparent brush for the legend:
    // Set position to upper left inside the axis rect
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignTop);
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

  customPlot->xAxis->setLabel("Voltage (mV)");
   customPlot->yAxis->setLabel("Gradient (V/m)");
   customPlot->xAxis->setRange(-30, 110);
  customPlot->yAxis->setRange(-1, 3);
  // setup a timer that repeatedly calls PhasePlotWindow::realtimeDataSlot:
  connect(m_Simulator, SIGNAL(eventHappened()),this,  SLOT(realtimeDataSlot()));
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->replot();

  PhasePlot = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
//  itemPhaseTracer = new QCPItemTracer ( PhasePlot);
  // pass the data to the curves; we know t (i in loop above) is ascending, so set alreadySorted=true (saves an extra internal sort):
  PhasePlot->data()->set(dataPhasePlot, true);
  PhasePlot->setPen(QPen(Qt::blue));
  PhasePlot->setBrush(QBrush(QColor(2, 20, 20, 20)));
  PhasePlot->setName("AP phase plot");
  PhasePlot->setLineStyle(QCPCurve::lsLine);
  PhasePlot->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
  // Add an ellipse
  RunningPoint = new QCPItemEllipse(customPlot);
  RunningPoint->topLeft->setCoords(-1, -.05);    // Set coordinates
  RunningPoint->bottomRight->setCoords(1, +0.05);
  RunningPoint->setBrush(QBrush(QColor(255, 0, 0, 50)));
  RunningPoint->setPen(QPen(Qt::red));
  PhasePlot->data()->set(dataPhasePlot, true);
  PhasePlot->setPen(QPen(Qt::blue));
  PhasePlot->setBrush(QBrush(QColor(2, 20, 20, 20)));

  // set some basic customPlot config:
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->rescaleAxes();
}

void PhasePlotWindow::replot(void)
{ui->customPlot->replot();}

void PhasePlotWindow::realtimeDataSlot()
{
    double Volt2 = m_neuron->MembraneRelativePotential_Get()*15;
    double DvDt = m_neuron->dVdtResulting_Get()/100.;
    RunningPoint->topLeft->setCoords(-1+Volt2, DvDt-.05);    // Set coordinates
    RunningPoint->bottomRight->setCoords(1+Volt2, DvDt+0.05);
//    uint64_t NCP = Gradient.count();
//    QVector<QCPCurveData> dataPhasePlot(NCP);
    dataPhasePlot.push_back(QCPCurveData(index,Volt2, DvDt));
    PhasePlot->data()->set(dataPhasePlot, true);
    index++;

    ui->customPlot->replot();
    cerr << "Phase" << index<< ','<<  Volt2<< ',' << DvDt << '\n';
}


PhasePlotWindow::~PhasePlotWindow()
{
  delete ui;
}

void PhasePlotWindow::screenShot()
{
  QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
    QString fileName = demoName.toLower()+".pdf";
  fileName.replace(" ", "");
  ui->customPlot->savePdf(fileName, 0, 0);

//  pm.save("./screenshots/"+fileName);
//  pm.save(fileName);
//  qApp->quit();
}



#include "ScQtNeuron_dVdt_Window.h"
#include "ScQtNeuron_MainWindow.h"
#include "SimulatorControlWindow.h"
#include "ui_ScQtNeuron_dVdt_Window.h"
#include "ui_SimulatorControlWindow.h"
#include <QDebug>
#include <iostream>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QMainWindow>
#include <QTimer>
//#include <QWindow>

#include <QFile>

ScQtNeuron_dVdt_Window::ScQtNeuron_dVdt_Window(ScQtNeuron_MainWindow *parent) :
  QMainWindow(parent),
    ui(new Ui::ScQtNeuron_dVdt_Window)
    ,m_parent(parent)
    ,m_Simulator_ControlWindow(parent->m_Simulator_ControlWindow)
{
  ui->setupUi(this);
  setGeometry(700, 50, 842, 190);
  QString WindowTitle;
  ui->customPlot = new QCustomPlot(this);
  WindowTitle = "dVdt of";
      setupRealtimeDataDemo();
      ui->customPlot->replot();
//   setWindowTitle(WindowTitle + QString(m_Simulator_ControlWindow->MyNeuron->()));
  statusBar()->clearMessage();

   // 10: setupRealtimeDataDemo(ui->customPlot);
  //QTimer::singleShot(1500, this, SLOT(allScreenShots()));
  //QTimer::singleShot(4000, this, SLOT(screenShot()));
}


void ScQtNeuron_dVdt_Window::ProcessLine(QString line)
{
    QStringList firstColumn;
     // Handle the first two items only
//    while (!s1.atEnd()){
    first.append(line.split(",").at(0)); // appends first column to list, ',' is separator
    second.append(line.split(",").at(1));
    Time.push_back(line.split(",").at(0).toDouble());
    Voltage.push_back(line.split(",").at(1).toDouble());
    dVdt.push_back(line.split(",").at(3).toDouble());
 //    std::cout << line.toStdString() << '\n';
}
void ScQtNeuron_dVdt_Window::GetData(QString fileName)
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

void ScQtNeuron_dVdt_Window::setupRealtimeDataDemo(void)
{
    GetData("/home/jvegh/REPO/LaTeX/figures/dVdt_vs_V_3D/data.csv");
  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  ui->customPlot->addGraph(); // blue line
/*  customPlot->addGraph(); // red line
*/
  index = 0;

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%s:%z"); //"%m:%s:%Z"
  ui->customPlot->xAxis->setTicker(timeTicker);
  ui->customPlot->axisRect()->setupFullAxesBox();
  ui->customPlot->yAxis->setRange(-1500, 4000);
  ui-> customPlot->graph(0)->setPen(QPen(QColor(255, 110, 40)));
  
  // make left and bottom axes transfer their ranges to right and top axes:
  connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
  
  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(25); // Interval 0 means to refresh as fast as possible

  //eventHappened
  connect(m_parent->m_Simulator, &ScQtSimulator::eventHappened, this, &ScQtNeuron_dVdt_Window::simulatorEvent);
}


void ScQtNeuron_dVdt_Window::realtimeDataSlot()
{
  static QTime timeStart = QTime::currentTime();
  // calculate two new data points:
//  double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
  double Value;
  Value = dVdt[index];
  double key = Time[index++];
  if(index>=Voltage.count())
      index = 0;
   static double lastPointKey = 0;
  if (key-lastPointKey > 0.005) // at most add point every 5 us
  {
    // add data to lines:
    //  Time.push_back(line.split(",").at(0).toDouble());
    //  Voltage.push_back(line.split(",").at(1).toDouble());

      ui->customPlot->graph(0)->addData(key, Value);
 //   ui->customPlot->graph(1)->addData(key, VGrad);
    // rescale value (vertical) axis to fit the current data:
    ui->customPlot->graph(0)->rescaleValueAxis();
    //ui->customPlot->graph(1)->rescaleValueAxis(true);
    lastPointKey = key;
//    if(index>sizeof(Voltage))
//        Voltage.clear();
  }
  // make key axis range scroll with the data (at a constant range size of 2.5):
  ui->customPlot->xAxis->setRange(key, 2.5, Qt::AlignRight);
  ui->customPlot->replot();
  
  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 1) // average fps over 2 seconds
  {
    m_parent->statusBar()->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->customPlot->graph(0)->data()->size()
//                   +customPlot->graph(1)->data()->size()
                   )
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}

ScQtNeuron_dVdt_Window::~ScQtNeuron_dVdt_Window()
{
    delete ui->customPlot;
  delete ui;
}

void ScQtNeuron_dVdt_Window::simulatorEvent()
{
    QMessageBox aboutDialog(this);
    aboutDialog.setWindowIcon(QIcon(":/icons/neurer.png"));
    double SC = sc_time_stamp().to_seconds()*1000.;
    std::string varAsString = std::to_string(SC);
    QString sim_time = QString(varAsString.c_str());
    m_Simulator_ControlWindow->ui->SimulatedTime->setText(sim_time);

    SC = m_Simulator_ControlWindow->MyNeuron->MembraneRelativePotential_Get();
    varAsString = std::to_string(SC);
    sim_time = QString(varAsString.c_str());
    m_Simulator_ControlWindow->ui->timeSystem->setText(sim_time);
 /*   aboutDialog.setText("Event happened in AP");
    aboutDialog.exec();
*/
}
void ScQtNeuron_dVdt_Window::screenShot()
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
  QString fileName = "A.png";
//  demoName.toLower()+".png";
  fileName.replace(" ", "");
  pm.save("./screenshots/"+fileName);
  qApp->quit();
}

/*
void ScQtNeuron_AP_Window::allScreenShots()
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
  
  if (currentDemoIndex < 19)
  {
    if (dataTimer.isActive())
      dataTimer.stop();
    dataTimer.disconnect();
    delete ui->customPlot;
    ui->customPlot = new QCustomPlot(ui->centralWidget);
    ui->verticalLayout->addWidget(ui->customPlot);
//    setupDemo(currentDemoIndex+1);
    // setup delay for demos that need time to develop proper look:
    int delay = 250;
    if (currentDemoIndex == 10) // Next is Realtime data demo
      delay = 12000;
    else if (currentDemoIndex == 15) // Next is Item demo
      delay = 5000;
    QTimer::singleShot(delay, this, SLOT(allScreenShots()));
  } else
  {
    qApp->quit();
  }
}

*/






































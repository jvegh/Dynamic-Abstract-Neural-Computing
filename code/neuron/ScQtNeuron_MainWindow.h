#ifndef ScQtNeuron_MAINWINDOW_H
#define ScQtNeuron_MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QSettings>
#include <QMessageBox>
#include <QVector>
#include "ScQtAbout.h"
#include "ScQtSimulator.h"
#include "SimulatorControlWindow.h"
//#include "ScQtNeuron_Plot_AP.h"
#include "neurontab.h"
#include "ScQtNeuron_AP_Window.h"
/*
//#include "filefilters.h"

extern struct SystemDirectories Directories;
*/

class ScQtNeuron_ParameterWindow;

QT_FORWARD_DECLARE_CLASS(QToolBar)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QActionGroup)

#include "TestNeuronPhysical.h"
namespace Ui {
class ScQtNeuron_MainWindow;
}

/*!
 * \brief The Overall plot sytyle structure, as used by QCustomPlot
 *
 * All subwindows have their own plot styles, only the structure is the same
 */
struct PlotStyle
{
    bool ConfidenceBands_Present;   // If to draw component in question
    bool Legend_Present;
    bool TheoryCurve_Present;
    bool MeasurementData_Present;
    bool MeasurementError_Present;
    Qt::PenStyle ConfidenceBands_PenStyle; // The style of line drawing
    Qt::PenStyle TheoryCurve_PenStyle;
    QColor ConfidenceBands_PenColor;
    QColor MeasurementData_PenColor;
    QColor TheoryCurve_PenColor;
   /* QCPGraph::LineStyle MeasurementData_LineStyle;
    QCPScatterStyle  TheoryCurve_ScatterShape;
    QCPScatterStyle  MeasurementData_ScatterShape;
*/
    QColor ConfidenceBands_BrushColor;
    short int ConfidenceBands_LineWidth; // The width of the line
    short int TheoryCurve_LineWidth;

    Qt::PenStyle Theory_LineStyle; // The style of line drawing

};
class RipesTab;

struct TabWidgets {
    RipesTab *tab;
    QToolBar *toolbar;
};


class ScQtNeuron_MainWindow : public QMainWindow
{
    Q_OBJECT
    enum TabIndex {
        NeuronTabID,
        EditTabID,
        NTabsID
    };
public:
    explicit ScQtNeuron_MainWindow(QWidget *parent = 0);
    ~ScQtNeuron_MainWindow();
    QToolBar *toolBar;
    QToolBar * plotToolBar;
    QToolBar * procToolBar;
    QAction *fileNewWindowAction, *fileOpenAction, *fileExitAction;
    QAction *processFitAction;
    ScQtSimulator * m_Simulator;
    SimulatorControlWindow *m_Simulator_ControlWindow;
    ScQtNeuron_AP_Window *m_AP_Window;//, *m_dVdt_Window;

     //    gsysHierarchyTree * hierTree;
    //    gsysPortViewer * portViewer;
    //    gsysBuffer* bufferWdw;
    ScQtAbout* aboutWdw;
    void openAboutWdw();
//    QAction* fileSaveAction;
    QMenuBar *menubar;
    QMenu *fileMenu, *processMenu;
//    ScQtNeuronVoltagePlot *VoltagePlot;
    uint32_t index, lastindex;

//    QList<BaseFileFilter* > fileFilterList;
//    void initFileFilterList();
     void readSettings();
     void readSettings(QSettings* settings);
     void writeSettings();
     void setupToolBoxes(void);
     void fitToView();
     void ProcessLine(QString line);
     void GetData(QString fileName);

//protected:
     void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
     void wiki() ;
     void version();
//     void event_happened(); // Not sure if needed
/*    void fileNewWindow();
    void fileOpenDialog();
    virtual void fileExit();
    void realtimeDataSlot();*/
private:
    void SetFileMenu(void);
    void SetProcessMenu(void);
    void fileOpen(QString name,  QString filter, int flags);
    QString working_directory; //contains working directory
    bool maybeClose();
    void setupStatusBar();
    void setupMenus();
    void setupExamplesMenu(QMenu *parent);
    void addTreeRoot(QString name, QString description);
    void addTreeChild(QTreeWidgetItem *parent,
                      QString name, QString description);

    NeuronPhysicalTEST *MyNeuron;
    // Tabs
    QStackedWidget *m_stackedTabs = nullptr;
    std::map<TabIndex, TabWidgets> m_tabWidgets;
    TabIndex m_currentTabID = NeuronTabID;
 //   uint32_t index, lastindex;
    QStringList first,second;
protected:
    Ui::ScQtNeuron_MainWindow *ui;
};



class global_options_ {
public:
        size_t ticks;
        bool hasXaxis;
        bool hasYaxis;
        bool hasExtraTicks;
        size_t extraTicks;
        QString axisFont;
        bool antialiasing;
        bool smoothTransformations;
        bool animatedMarkers;
        QString customPrintLogo;
        bool printPars;
        int printLineWidth;
        int print1Dmode; //PRINT1D_ALL-print single line; PRINT1D_SKIP-reduce resolution; PRINT1D_SPLIT-split
        QString style;
        int xunits,  yunits;
//	bool useHypercomplex;
        bool useBold1D;
        bool mixFidSpec;
        bool useSkyline;
        bool useSystemPrintPars;
        bool yLeftLabels; //print labels on the left from y-axis
//	QStringList procToolBarList;
//	size_t maxAttachedProc; //maximum of remembered files with attached processing
        global_options_() {readSettings();//maxAttachedProc=100;
                           useSystemPrintPars=true;};
        ~global_options_() {writeSettings();};
private:
        void readSettings();
        void writeSettings();
};

#endif // ScQtNeuron_MAINWINDOW_H

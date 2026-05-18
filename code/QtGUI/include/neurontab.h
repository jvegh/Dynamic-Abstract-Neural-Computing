#pragma once

#include <QAction>
#include <QSpinBox>
#include <QTimer>
#include <QToolBar>
#include <QWidget>

//#include "ripestab.h"
class ScQtNeuron_MainWindow;
class ScQtNeuronVoltagePlot;

namespace Ui {
class NeuronTab;
}

struct Layout;

class NeuronTab : public QWidget{ //
//    Ripes::RipesTab{

  friend class ScQtNeuron_MainWindow;
  Q_OBJECT

public:
  NeuronTab(QToolBar *controlToolbar, QToolBar *additionalToolbar,
               ScQtNeuron_MainWindow *parent = nullptr);
  ~NeuronTab() override;

public slots:
  void restart();


private:
  void setupSimulatorActions(QToolBar *controlToolbar);
  void enableSimulatorControls();
  void SetupGUI();

  Ui::NeuronTab *ui = nullptr;
  // Actions
  QAction *m_selectNeuronAction = nullptr;
  QAction *m_runAction = nullptr;
/*//  QAction *m_darkmodeAction = nullptr;
  private:
*/
};

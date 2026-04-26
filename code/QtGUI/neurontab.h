#pragma once

#include <QAction>
#include <QSpinBox>
#include <QTimer>
#include <QToolBar>
#include <QWidget>

#include "ripestab.h"
class ScQtNeuron_MainWindow;
class ScQtNeuronVoltagePlot;

namespace Ui {
class NeuronTab;
}

struct Layout;

class NeuronTab : public RipesTab {
  friend class ScQtNeuron_MainWindow;
  Q_OBJECT

public:
  NeuronTab(QToolBar *controlToolbar, QToolBar *additionalToolbar,
               QMainWindow *parent = nullptr);
  ~NeuronTab() override;

//  void initRegWidget();

public slots:
  void restart();


private:
  void setupSimulatorActions(QToolBar *controlToolbar);
  void enableSimulatorControls();

  Ui::NeuronTab *ui = nullptr;
/*
 *   QTimer *m_statUpdateTimer;
*/
  // Actions
  QAction *m_selectNeuronAction = nullptr;
  QAction *m_runAction = nullptr;
/*  QAction *m_displayValuesAction = nullptr;
  QAction *m_resetAction = nullptr;
//  QAction *m_darkmodeAction = nullptr;
  private:
*/
};

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
  friend class RunDialog;
  friend class MainWindow;
  Q_OBJECT

public:
  NeuronTab(QToolBar *controlToolbar, QToolBar *additionalToolbar,
               QMainWindow *parent = nullptr);
  ~NeuronTab() override;

//  void initRegWidget();

public slots:
  void pause();
  void restart();
  void reset();
  void reverse();
  void neuronFinished();
  void runFinished();
  void updateStatistics();

  void neuronSelection();

private slots:
  void run(bool state);

private:
  void setupSimulatorActions(QToolBar *controlToolbar);
  void enableSimulatorControls();
  void loadNeuronToWidget(const Layout *);

  Ui::NeuronTab *m_ui = nullptr;

  QTimer *m_statUpdateTimer;

  // Actions
  QAction *m_selectNeuronAction = nullptr;
  QAction *m_clockAction = nullptr;
//  QAction *m_autoClockAction = nullptr;
  QAction *m_runAction = nullptr;
  QAction *m_displayValuesAction = nullptr;
  QAction *m_resetAction = nullptr;
//  QAction *m_darkmodeAction = nullptr;
//  QTimer *m_autoClockTimer = nullptr;

  QSpinBox *m_autoClockInterval = nullptr;
  private:
};

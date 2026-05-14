#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QPushButton>

#include <systemc.h>

// Simple SystemC module
SC_MODULE(SimpleModule) {
    sc_in<bool> clock; // Clock input

    void process() {
        std::cout << "Hello, SystemC! Time: " << sc_time_stamp() << std::endl;
    }

    SC_CTOR(SimpleModule) {
        SC_METHOD(process);
        sensitive << clock.pos(); // Trigger on positive edge of clock
    }
};

class MyWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MyWindow(QWidget *parent = 0);
private:
    QPushButton *m_button;
};

#endif //MYWINDOW_H

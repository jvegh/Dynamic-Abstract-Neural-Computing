#if 0
#include <systemc>
#include <pthread.h>
#include <unistd.h>

using namespace sc_core;

class ThreadSafeEventIf : public sc_interface {
    virtual void notify(sc_time delay = SC_ZERO_TIME) = 0;
    virtual const sc_event &default_event(void) const = 0;
protected:
    virtual void update(void) = 0;
};

class ThreadSafeEvent : public sc_prim_channel, public ThreadSafeEventIf {
public:
    ThreadSafeEvent(const char *name = ""): event(name) {}

    void notify(sc_time delay = SC_ZERO_TIME) {
        this->delay = delay;
        std::cout << "ThreadSafeNotify called\n";
        async_request_update();
    }

    const sc_event &default_event(void) const {
        return event;
    }
protected:
    virtual void update(void) {
        event.notify(delay);
        std::cout << "ThreadSafeUpdate called\n";
    }
    sc_event event;
    sc_time delay;
};

SC_MODULE(Foo)
{
public:
    SC_CTOR(Foo)
    {
        SC_THREAD(main);
        sensitive << event;

        SC_METHOD(eventTriggered);
        sensitive << event;
        dont_initialize();
        std::cout << "SC started\n";
    }
private:
    void main() {
        std::cout << "Initializing main()" << std::endl;
        int i=0;
        while(i++<3)
        {
            wait();
            std::cout << "Got event in main()" << std::endl;
        }
    }

    void eventTriggered() {
        std::cout << "Got event in triggered" << std::endl;
        next_trigger(10,SC_NS);
        std::cout << "Got event after next_trigger" << std::endl;
        next_trigger();
        std::cout << "Got event in triggered again" << std::endl;
    }

public:
    ThreadSafeEvent event;
};

void *externalHostThread(void *arg) {
    std::cout << "External host thread started" << std::endl;
    Foo* foo = (Foo*)(arg);
    std::cout << "Before Event 1 notified from an external host thread" << std::endl;
    foo->event.notify();
    std::cout << "After Event 1 notified from an external host thread" << std::endl;
    usleep(10 * 1000 * 1000); // Just for the example
    std::cout << "After usleep from an external host thread" << std::endl;
    std::cout << "Event 2 notified from an external host thread" << std::endl;
    foo->event.notify();
    return NULL;
}

int sc_main(int argc, char *argv[])
{
    Foo foo("foo");

    pthread_t thread;
    pthread_create(&thread, NULL, externalHostThread, &foo);

    sc_start();
    usleep(15 * 1000 * 1000); // Just for the example
    std::cout << "SC returned" << std::endl;
    return 0;
}
#endif

#include "mainwindow.h"
#include <QApplication>
#include <systemc>

int sc_main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


/*
   Copyright 2013 Fabien Pierre-Nicolas.
      - Primarily authored by Fabien Pierre-Nicolas

   This file is part of simple-qt-thread-example, a simple example to demonstrate how to use threads.
   This example is explained on http://fabienpn.wordpress.com/qt-thread-simple-and-stable-with-sources/

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This progra is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scQtSimulator.h"
#include <QTimer>
#include <QEventLoop>

#include <QThread>
#include <QDebug>

extern scQtModule A;
scQtSimulator::scQtSimulator(sc_core::sc_module_name nm,
                             QObject *parent) :
    QObject(parent), sc_core::sc_module(nm)
{
//    A = new scQtModule("A");

    _working =false;
    _abort = false;
 }

void scQtSimulator::requestWork()
{
    mutex.lock();
    _working = true;
    _abort = false;
    qDebug()<<"Request scQtSimulator start in Thread "<<thread()->currentThreadId();
    mutex.unlock();

    emit workRequested();
}

void scQtSimulator::requestReset()
{
    mutex.lock();
    _working = true;
    _abort = false;
    qDebug()<<"Request scQtSimulator reset in Thread "<<thread()->currentThreadId();
    emit resetRequested();
    mutex.unlock();
 }

void scQtSimulator::abort()
{
    mutex.lock();
    if (_working) {
        _abort = true;
        qDebug()<<"Request scQtSimulator aborting in Thread "<<thread()->currentThreadId();
    }
    mutex.unlock();
}

/*
    void scQtSimulator::start()
{
    qDebug()<<"Starting worker process in Thread "<<thread()->currentThreadId();
        sc_start( SC_ZERO_TIME );// Run the initialization phase to create pending activity
    while( sc_pending_activity() ) {
        sc_start( sc_time_to_pending_activity() );
        qDebug()<<"Processing events in Simulator "<<thread()->currentThreadId();
        emit runChanged("Event ",1);
    }
    for (int i = 0; i < 15; i ++) {
        if( QThread::currentThread()->isInterruptionRequested() )
            break;

        // This will stupidly wait 1 sec doing nothing...
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

        // Once we're done waiting, value is updated
        emit valueChanged(QString::number(i));
    }

     qDebug()<<"Worker process finished in Thread "<<thread()->currentThreadId();

    //Once 15 sec passed, the finished signal is sent
    emit finished();
}
*/


void scQtSimulator::doReset()
{
    qDebug()<<"Reseting scQtSimulator process in Thread "<<thread()->currentThreadId();
    mutex.lock();
    _working = false;
    mutex.unlock();

    qDebug()<<"Modules inited in Thread "<<thread()->currentThreadId();

    //Once 60 sec passed, the finished signal is sent
    emit requestModuleReset();
    mutex.unlock();
}

void scQtSimulator::doWork()
{
    qDebug()<<"Starting scQtSimulator process in Thread "<<thread()->currentThreadId();

    for (int i = 0; i < 15; i ++) {

        // Checks if the process should be aborted
        mutex.lock();
        bool abort = _abort;
        mutex.unlock();

        if (abort) {
            qDebug()<<"Aborting scQtSimulator process in Thread "<<thread()->currentThreadId();
            break;
        }

        // This will stupidly wait 1 sec doing nothing...
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

        // Once we're done waiting, value is updated
        emit valueChanged(QString::number(i));
    }
    // Set _working to false, meaning the process can't be aborted anymore.
    mutex.lock();
    _working = false;
    mutex.unlock();

    qDebug()<<"Worker process finished in Thread "<<thread()->currentThreadId();

    //Once 60 sec passed, the finished signal is sent
    emit finished();
}
/*
void scQtSimulator::finish()
{
    qDebug()<<"Simulator finish in Thread "<<thread()->currentThreadId();

}
*/

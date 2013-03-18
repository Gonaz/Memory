// Copyright (c) 2011, Jonas Vanthornhout
// All rights reserved.
 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QMap>
#include <QVector>
#include <QPushButton>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void init();
    void enableAllCards();
    void disableAllCards();
    void colorCards();
    QVector<QPushButton*> *getCards();
    void makeTimerProgressBar();
    void makeSignalMapper();
    void makeNameMapper();
    QVector<QPair<QColor, int> > *makeColors();

    unsigned int numberOfColors;
    int cardsTurned;
    unsigned int scorePlayer1;
    unsigned int scorePlayer2;
    int currentPlayer;

    QPushButton *turnedCard1;
    QPushButton *turnedCard2;
    QVector<QPair<QPushButton*, QPushButton*> > *pairs;
    QVector<QPushButton*> *buttons;
    QMap<QString, QPushButton*> *nameMapper;
    QTimer *progress;
    QSignalMapper *signalMapper;

private slots:
    void on_actionStart_triggered();
    void checkCards();
    void progressBar();
    void cardClicked(const QString &text);
};

#endif // MAINWINDOW_H

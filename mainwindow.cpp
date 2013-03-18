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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QTimer>
#include <QMessageBox>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

    buttons = getCards();
    colorCards();
    makeTimerProgressBar();
    makeSignalMapper();
    makeNameMapper();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){
    srand(time(NULL));

    numberOfColors = 6;
    cardsTurned = 0;
    scorePlayer1 = 0;
    scorePlayer2 = 0;
    currentPlayer = 1;

    buttons = new QVector<QPushButton*>();
    pairs = new QVector<QPair<QPushButton*, QPushButton*> >();
}

void MainWindow::makeSignalMapper(){
    signalMapper = new QSignalMapper(this);

    for (int i=0; i < buttons->size(); ++i) {
        QPushButton *button = buttons->at(i);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(button, button->objectName());
    }

    connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(cardClicked(const QString &)));
}

void MainWindow::makeNameMapper(){
    nameMapper = new QMap<QString, QPushButton*>();
    for(int i=0; i<buttons->size(); ++i){
        nameMapper->insert(buttons->at(i)->objectName(), buttons->at(i));
    }
}

QVector<QPushButton*> *MainWindow::getCards(){
    QVector<QPushButton*> *btns = new QVector<QPushButton*>();
    btns->push_back(ui->card1);
    btns->push_back(ui->card2);
    btns->push_back(ui->card3);
    btns->push_back(ui->card4);
    btns->push_back(ui->card5);
    btns->push_back(ui->card6);
    btns->push_back(ui->card7);
    btns->push_back(ui->card8);
    btns->push_back(ui->card9);
    btns->push_back(ui->card10);
    btns->push_back(ui->card11);
    btns->push_back(ui->card12);
    return btns;
}

QVector<QPair<QColor, int> > *MainWindow::makeColors(){
    QVector<QPair<QColor, int> > *colors = new QVector<QPair<QColor, int > >();
    QColor color1(128,0,0);
    QColor color2(255,0,0);
    QColor color3(0,128,0);
    QColor color4(0,255,0);
    QColor color5(0,0,128);
    QColor color6(0,0,255);

    colors->push_back(QPair<QColor, int>(color1, 2));
    colors->push_back(QPair<QColor, int>(color2, 2));
    colors->push_back(QPair<QColor, int>(color3, 2));
    colors->push_back(QPair<QColor, int>(color4, 2));
    colors->push_back(QPair<QColor, int>(color5, 2));
    colors->push_back(QPair<QColor, int>(color6, 2));
    return colors;
}

void MainWindow::colorCards(){
    QVector<QPair<QColor, int> > *colors = makeColors();

    //Map colors to buttons
    int color;

    QVector<QPair<QPushButton*, QColor> > *buttonColors = new QVector< QPair<QPushButton*, QColor> >();

    QVectorIterator<QPushButton*> it(*buttons);
    while(it.hasNext()){
        color = rand()%numberOfColors;

        while(colors->at(color).second == 0){
            color = rand()%numberOfColors;
        }

        buttonColors->push_back(QPair<QPushButton*, QColor>(it.next(), colors->at(color).first));
        colors->replace(color, QPair<QColor, int>(colors->at(color).first, colors->at(color).second-1));;
    }

    for(int i=0; i<buttonColors->size(); ++i){
        QString style_sheet_btn =" QPushButton:checked {background-color:" + buttonColors->at(i).second.name() + ";border-style:solid;}";
        buttonColors->at(i).first->setStyleSheet(style_sheet_btn);
    }

    //Make pairs of the buttons
    for(int i=0; i<buttonColors->size(); ++i){
        for(int j=0; j<buttonColors->size(); ++j){
            if(i!=j){
                if(buttonColors->at(i).second == buttonColors->at(j).second){
                    pairs->push_back(QPair<QPushButton*, QPushButton*> (buttonColors->at(i).first, buttonColors->at(j).first));
                }
            }
        }
    }

}

void MainWindow::makeTimerProgressBar(){
    progress = new QTimer(this);
    connect(progress, SIGNAL(timeout()), this, SLOT(progressBar()));
}

void MainWindow::cardClicked(const QString &text){
    QString *key = new QString(text.data());
    nameMapper->value(*key)->setDisabled(true);
    if(cardsTurned == 0){
        cardsTurned = 1;
        turnedCard1 = nameMapper->value(*key);
    } else if(cardsTurned == 1){
        disableAllCards();
        cardsTurned = 2;
        turnedCard2 = nameMapper->value(*key);
        QTimer::singleShot(2000, this, SLOT(checkCards()));
        progress->start(20);
    }
}

void MainWindow::enableAllCards(){
    for(int i=0; i<buttons->size(); ++i){
        buttons->at(i)->setEnabled(true);
    }
}

void MainWindow::disableAllCards(){
    for(int i=0; i<buttons->size(); ++i){
        buttons->at(i)->setDisabled(true);
    }
}

void MainWindow::checkCards(){
    progress->stop();
    ui->progressBar->setValue(0);
    if(pairs->contains(QPair<QPushButton*, QPushButton*>(turnedCard1, turnedCard2))){
        turnedCard1->setVisible(false);
        turnedCard2->setVisible(false);

        std::stringstream ss(std::stringstream::in | std::stringstream::out);

        if(currentPlayer == 1){
            ++scorePlayer1;
            ss << scorePlayer1;
            ui->lblScorePlayer1->setText(QString(ss.str().c_str()));
        } else {
            ++scorePlayer2;
            ss << scorePlayer2;
            ui->lblScorePlayer2->setText(QString(ss.str().c_str()));
        }
    } else {
        turnedCard1->setEnabled(true);
        turnedCard2->setEnabled(true);
        turnedCard1->setChecked(false);
        turnedCard2->setChecked(false);

        if(currentPlayer == 1){
            currentPlayer = 2;
            ui->currentPlayer->setText(ui->namePlayer2->text());
        } else {
            currentPlayer = 1;
            ui->currentPlayer->setText(ui->namePlayer1->text());
        }
    }
    cardsTurned = 0;
    enableAllCards();

    if(scorePlayer1+scorePlayer2 == numberOfColors){
        QMessageBox *msg = new QMessageBox(this);
        if(scorePlayer1 > scorePlayer2){
            msg->setText(ui->namePlayer1->text() + " has won!");
        } else if(scorePlayer2 > scorePlayer1){
            msg->setText(ui->namePlayer2->text() + " has won!");
        } else {
            msg->setText("It's a tie!");
        }
        msg->show();
    }
}

void MainWindow::on_actionStart_triggered()
{
    for(int i=0; i<buttons->size(); ++i){
        buttons->at(i)->setEnabled(true);
    }
    ui->currentPlayer->setText(ui->namePlayer1->text());
    ui->namePlayer1->setDisabled(true);
    ui->namePlayer2->setDisabled(true);
}

void MainWindow::progressBar(){
    ui->progressBar->setValue(ui->progressBar->value()+20);
}

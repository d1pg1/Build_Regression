#include <math.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->Button_build, SIGNAL (released()), this, SLOT (on_build_clicked()));
    connect(ui->Button_points, SIGNAL (released()), this, SLOT (on_points_clicked()));
    connect(ui->Button_clear, SIGNAL (released()), this, SLOT (on_clear_clicked()));
    connect(ui->Button_calculate, SIGNAL (released()), this, SLOT (on_solve_clicked()));
    ui->Grafick->xAxis->setLabel("Середній дохід");
    ui->Grafick->xAxis->setRange(0,13); //1

    ui->Grafick->yAxis->setLabel("Кількість сбережень");
    ui->Grafick->yAxis->setRange(-3,10);
    ui->Grafick->replot();
    read_input();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::read_input(){
    QFile file("input.txt");
    file.open(QIODevice::ReadOnly);
    QStringList lineData = QString(file.readLine()).split(" ");
    for(int i = 0; i < lineData.length(); ++i)
    {
         X.push_back(lineData[i].toDouble());
    }
    lineData=QString(file.readLine()).split(" ");
    for(int i = 0; i < lineData.length(); ++i)
    {
         Y.push_back(lineData[i].toDouble());
    }
    file.close();
}

void MainWindow::on_solve_clicked(){
    QVector<Regression_model> Models;
    for(int i=0;i<4;i++){
        Models.push_back(Regression_model(X, Y, i));
        if(Models[i].determination > Best_model.determination)
            Best_model=Models[i];
    }

    ui->label_6->setText(Best_model.equation);
    ui->label_4->setText(QString::number(std::sqrt(Best_model.determination)));
    ui->label_5->setText(QString::number(Best_model.determination));
    ui->Button_build->setEnabled(true);
}
void MainWindow::on_build_clicked() {
    const int size = 125;
    QVector <double> x(size),y(size);
    for (int i=0; i<size; i++) {
        x[i] = i/10.0;
        y[i] = Best_model.function(x[i]);
    }
    ui->Grafick->addGraph();
    ui->Grafick->graph()->setData(x,y);
    ui->Grafick->replot();

}
void MainWindow::on_points_clicked() {
    ui->Grafick->addGraph();
    ui->Grafick->graph()->setData(X,Y);
    ui->Grafick->graph()->setPen(QPen(Qt::red));
    ui->Grafick->graph()->setLineStyle(QCPGraph::lsNone);
    ui->Grafick->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    ui->Grafick->replot();
}
void MainWindow::on_clear_clicked() {
    ui->Grafick->clearGraphs();
    ui->Grafick->replot();
}

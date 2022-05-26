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
    connect(ui->Button_input, SIGNAL (released()), this, SLOT (on_input_clicked()));
    ui->Grafick->xAxis->setLabel("Середній дохід");
    ui->Grafick->xAxis->setRange(0,13);

    ui->Grafick->yAxis->setLabel("Кількість збережень");
    ui->Grafick->yAxis->setRange(-3,11);
    ui->Grafick->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_solve_clicked(){
    if(X.empty()){
        QMessageBox msgBox;
        msgBox.setText("Для розрахунку необхідно імпортувати вхідні дані");
        msgBox.exec();
        return;
    }
    // обираємо модель з найбільшим коофіцієнтом детермінації
    QVector<Regression_model> Models;
    for(int i=0;i<4;i++){
        Models.push_back(Regression_model(X, Y, i));
        qDebug() << QString::number(Models[i].determination);
        if(Models[i].determination > Best_model.determination)
            Best_model=Models[i];
    }

    ui->label_6->setText(Best_model.equation);
    ui->label_4->setText(QString::number(std::sqrt(Best_model.determination)));
    ui->label_5->setText(QString::number(Best_model.determination));
    ui->Button_build->setEnabled(true);
    QMessageBox msgBox;

}
void MainWindow::on_build_clicked() {
    const int size = (ui->Grafick->xAxis->range().size())*10;
    QVector <double> x(size),y(size);
    // обчислюємо значення регресійного рівняння на проміжку вхідних даних
    double leftX=ui->Grafick->xAxis->range().lower;
    for (int i=0; i<size; i++) {
        x[i] = leftX+(i/10.0);
        y[i] = Best_model.function(x[i]);
    }
    // будуємо графік регресійного рівняння по обчисленим точкам
    ui->Grafick->addGraph();
    ui->Grafick->graph()->setData(x,y);
    ui->Grafick->replot();

}
void MainWindow::on_points_clicked() {
    // будуємо на графіку точки з вхідних даних
    ui->Grafick->addGraph();
    ui->Grafick->graph()->setData(X,Y);
    ui->Grafick->graph()->setPen(QPen(Qt::red));
    ui->Grafick->graph()->setLineStyle(QCPGraph::lsNone);
    ui->Grafick->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    ui->Grafick->replot();
}
void MainWindow::on_clear_clicked() {
    // видаляємо всі намальовані елементи з графіку
    ui->Grafick->clearGraphs();
    ui->Grafick->replot();
}
void MainWindow::on_input_clicked(){
    // виводимо повідомлення про формат вхідних даних
    QMessageBox msgBox;
    msgBox.setText("Будь ласка оберіть текстовий файл з вхідними даними\n"
                   "Формат вхідних даних:\n"
                   "У першому рядку через пробіл записати дані про середній дохід X,\n"
                   "у другому рядку через пробіл записати дані про кількість сбережень Y.\n"
                   "Кількість елементів в обох рядках потрібна співпадати");
    msgBox.exec();
    // видаляємо попердні дані та розрахунки, якщо вони були
    X.clear();
    Y.clear();
    Best_model=Regression_model();
    ui->Grafick->clearGraphs();
    ui->label_6->setText("");
    ui->label_4->setText("");
    ui->label_5->setText("");
    ui->Button_build->setEnabled(false);
    // відкрваємо файл з вхідними даними
    QFileDialog file_name;
    QFile file(file_name.getOpenFileName(this,"Choose an input file","","Text files (*.txt)"));
    // якщо файл не відкрито виводимо повідомлення про помилку
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox msgerrorBox;
        msgerrorBox.setText("Файл не відкрито");
        msgerrorBox.exec();
        return;
    }
    // зчитуємо вхідні дані та обчислюємо проміжки координатних осей, що будуть відображені на графіку
    QStringList lineData = QString(file.readLine()).split(" ");
    double leftX=0,rightX=0;
    for(int i = 0; i < lineData.length(); ++i)
    {
         X.push_back(lineData[i].toDouble());
         leftX=std::min(leftX,X.back());
         rightX=std::max(rightX,X.back());
    }
    lineData=QString(file.readLine()).split(" ");
    double leftY=0,rightY=0;
    for(int i = 0; i < lineData.length(); ++i)
    {
         Y.push_back(lineData[i].toDouble());
         leftY=std::min(leftY,Y.back());
         rightY=std::max(rightY,Y.back());
    }
    file.close();
    // якщо дані введено некоректні дані виводимо повідомлення про помилку
    if(X.size()!=Y.size() || X.empty()){
        QMessageBox msgerrorBox;
        msgerrorBox.setText("Неправильний формат вхідних даних");
        msgerrorBox.exec();
        X.clear();
        Y.clear();
        return;
    }
    else{
        QMessageBox msgsuccessBox;
        msgsuccessBox.setText("Дані успішно імпортовано");
        msgsuccessBox.exec();
    }
    // встановлюємо проміжки координатних осей, що будуть відображені на графіку
    ui->Grafick->xAxis->setRange(0,rightX+1);
    ui->Grafick->yAxis->setRange(leftY-3,rightY+3);
    ui->Grafick->replot();
}

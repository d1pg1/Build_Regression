#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <regression_model.h>

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
    QVector<double> X,Y;
    Regression_model Best_model;


private slots:
     void on_clear_clicked(void);
     void on_build_clicked(void);
     void on_points_clicked(void);
     void on_solve_clicked(void);
     void on_input_clicked(void);
};


#endif // MAINWINDOW_H

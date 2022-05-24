#ifndef REGRESSION_MODEL_H
#define REGRESSION_MODEL_H

#include<QVector>
#include<QString>
#include <iostream>
#include <utility>
#include <qmath.h>
class Regression_model
{
public:
    Regression_model(QVector<double>X_input,QVector<double>Y_input,int type);
    Regression_model();
    QVector<double> factors;
    QString equation;
    double determination=0;
    int type_of_model;
    double function(double x);
private:
    QVector<double> X,Y;
    QVector<double> X_t,Y_t;
    QVector<double> calculate_factors();
    double calculate_determination();
    void data_transformation();
    QString get_equation();
};

#endif // REGRESSION_MODEL_H

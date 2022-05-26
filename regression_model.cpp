#include "regression_model.h"

Regression_model::Regression_model(QVector<double>X_input,QVector<double>Y_input,int type)
{
    // ініціалізуємо та обчислюємо всі необхідні змінні моделі
    X=X_input;
    Y=Y_input;
    type_of_model=type;
    data_transformation();
    factors=calculate_factors();
    determination=calculate_determination();
    equation=get_equation();
}
Regression_model::Regression_model()
{
}
QVector<double> Regression_model::calculate_factors(){
    int n=X_t.size();
    double X1T[2][n],X1[n][2],Y1[n][1],matrix1[2][2]={},matrix2[2][1];
    // ініціалізуємо матриці для подальших обчислень
    for(int i=0;i<n;i++){
        X1T[0][i]=1;
        X1T[1][i]=X_t[i];
        X1[i][0]=X1T[0][i];
        X1[i][1]=X1T[1][i];
        Y1[i][0]=Y_t[i];
    }
    // обчислюємо X1T*X1
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            for(int k=0;k<n;k++){
                matrix1[i][j]+=X1T[i][k]*X1[k][j];
            }
        }
    }
    // обчислюємо (X1T*X1)^-1
    double det=1/(matrix1[0][0]*matrix1[1][1]-matrix1[0][1]*matrix1[1][0]);
    std::swap(matrix1[0][0],matrix1[1][1]);
    std::swap(matrix1[1][0],matrix1[0][1]);
    matrix1[1][0]=-matrix1[1][0];
    matrix1[0][1]=-matrix1[0][1];
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++)matrix1[i][j]*=det;
    }
    // обчислюємо X1T*Y1
    for(int i=0;i<2;i++){
        for(int j=0;j<1;j++){
            matrix2[i][j]=0;
            for(int k=0;k<n;k++){
                matrix2[i][j]+=X1T[i][k]*Y1[k][j];
            }
        }
    }
    // знаходимо параметри регресії
    QVector<double>b(2);
    for(int i=0;i<2;i++){
        for(int k=0;k<2;k++){
            b[i]+=matrix1[i][k]*matrix2[k][0];
        }
    }
    return b;
}

double Regression_model::calculate_determination(){
    double sum=0,sum2=0;
    double sr=0;
    int n=Y.size();
    // обчислюємо залишкова дисперсія
    for(int i = 0; i < X.size(); ++i){
        sum+=(Y[i]-function(X[i]))*(Y[i]-function(X[i]));
    }
    sum*=(1.0/n);
    // обчислюємо середне значення y
    for(auto y:Y)sr+=y;
    sr=sr/1.0/n;
    // обчислюємо загальну дисперсію результативної ознаки
    for(int i=0;i<n;i++)sum2+=(Y[i]-sr)*(Y[i]-sr);
    sum2*=(1.0/n);
    // обчислюємо коофіцієнт детермінації
    double p=1-sum/sum2;
    return p;
}

void Regression_model::data_transformation(){
    // перетворюємо дані в залежеості від типу моделі
    if(type_of_model==0){
        X_t=X;
        Y_t=Y;
    }
    if(type_of_model==1){
        for(auto x:X)X_t.push_back(std::sqrt(x));
        Y_t=Y;
    }
    if(type_of_model==2){
        for(auto x:X)X_t.push_back(std::log(x));
        for(auto y:Y)Y_t.push_back(std::log(y));
    }
    if(type_of_model==3){
        for(auto x:X)X_t.push_back(std::log(x));
        Y_t=Y;
    }
    return;
}

QString Regression_model::get_equation(){
    // функція повертає розраховане регресійне ріняння у вигляді рядка
    if(type_of_model==0)return "y= "+QString::number(factors[0])+"+"+QString::number(factors[1])+"x";
    if(type_of_model==1)return "y= "+QString::number(factors[0])+"+"+QString::number(factors[1])+"sqrt(x)";
    if(type_of_model==2)return "y= "+QString::number(factors[0])+"*x^"+QString::number(factors[1]);
    if(type_of_model==3)return "y= "+QString::number(factors[0])+"+"+QString::number(factors[1])+"ln(x)";
}
double Regression_model::function(double x){
    // обчислюємо значення регресійного рівняння від x
    if(type_of_model==0)return (factors[0]+factors[1]*x);
    if(type_of_model==1)return (factors[0]+factors[1]*std::sqrt(x));
    if(type_of_model==2)return (std::exp(factors[0])*pow(x,factors[1]));
    if(type_of_model==3)return (factors[0]+factors[1]*std::log(x));
}

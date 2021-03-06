﻿#include "viktorovichev.h"

/**
 * Введение в дисциплину
 */
void viktorovichev::lab1()
{
cout<<"Hello world!";
}


/**
 * Метод Гаусса с выбором главного элемента
 */
void viktorovichev::lab2()
{
double p;
	int maxn;
    for (int k=0; k<N-1; k++)
    {
        maxn = k;
        for (int i=k+1; i<N; i++)
			if(abs(A[i][k]) > abs(A[maxn][k])) maxn = i; ///Выбор главного элемента
        std::swap(A[maxn], A[k]); ///Меняем строки местами
        std::swap(b[maxn], b[k]);

        for (int i=k+1; i<N; i++)
        {
            p = A[i][k]/A[k][k];
            for (int j=k; j<N; j++)
                A[i][j] -= p*A[k][j];
            b[i] -= p*b[k];
        }
    }

    for(int i = 0; i<N; i++)
    {
        x[i]=b[i];
    }

    for (int i=N-1; i>=0; i--)
    {
        for (int j=i+1;j<N;j++)
            x[i]-=A[i][j]*x[j];
        x[i] /= A[i][i];
    }
}



/**
 * Метод прогонки
 */
void viktorovichev::lab3()
{
double *P = new double [N]; ///Коэффициенты "альфа"
	double *Q = new double [N]; ///Коэффициенты "бетта"

	P[0] = -A[0][1]/A[0][0];
	Q[0] = b[0]/A[0][0];

	for(int i=1; i<N; i++) ///Определяем прогоночные коэффициенты
	{
		P[i] = A[i][i+1]/(-A[i][i] - A[i][i-1]*P[i-1]);
		Q[i] = (-b[i] + A[i][i-1]*Q[i-1])/(-A[i][i] - A[i][i-1]*P[i-1]);
	}

	x[N-1] = Q[N-1];
	for(int i=N-2; i>=0; i--) ///Определяем решение
		x[i] = P[i]*x[i+1] + Q[i];

	delete [] P;
	delete [] Q;
}



/**
 * Метод простых итераций
 */
void viktorovichev::lab4()
{
double eps = 1e-15; //чтобы выйти из цикла
double t = 1e-5; //(приближенный парметр) достаточно малое число, ПРИ ПОИМОМЩИ КОТОРОГО МЫ ПОЛУЧАЕМ КАЖДЫЙ РАЗ ВСЁ БОЛЕЕ И БОЛЕЕ ТОЧНОЕ ЧИСЛО

for (int i = 0; i < N; i++) //берем первое приближенное значение и прогоняем его
    {
		x[i] = 0;
	}

	double x1;
	double *xr = new double[N];//рассчитываем новые значения x-ов
	int step = 0;

	do {
		step++;

		for (int i = 0; i < N; i++) //рассчитываем новое точное значение в цикле
        {
			xr[i] = x[i];
			for (int k = 0; k < N; k++)
				xr[i] -= t*A[i][k] * x[k];
			xr[i] += t * b[i];

		}

		x1 = 0;
		for (int i = 0; i < N; i++) { //рассчитываем норму (с её помощью ведем учет того, что значение достаточно близкое к необходимое)
			x1 += (xr[i]-x[i])*(xr[i]-x[i]);
		}

		for (int i = 0; i < N; i++)
        {
			x[i] = xr[i]; //осуществляем перепресвоение, новое становится старым и мы возобновляем цикл
		}
	}
	while (sqrt(x1)>eps); //как меньш8е, то выходим из цимкла
}



/**
 * Метод Якоби или Зейделя
 */
void viktorovichev::lab5()
{
//Метод Якоби
double *prevX = new double[N];

for (int i=0; i<N; i++)
{
x[i] = 0; // первоначальное новое решение
}
double eps = 1e-13;
double eact = 0.0; //погрешность
int k = 0;

do  {
k++;
eact = 0.0;
for(int i=0; i<N; i++)
prevX[i]=x[i]; //записываем предыдущее решение
for(int i=0; i<N; i++)

{
double s = 0; //вычисляем s, но мы не берём диагональные элементы
for(int j=0; j<i; j++)
s += A[i][j] * prevX[j];
for(int j=i+1; j<N; j++)
s += A[i][j] * prevX[j];
x[i]=(b[i] - s)/A[i][i]; //находим новое решение
}

eact = abs(prevX[0]-x[0]);
for(int i=0; i<N; i++)
{
if(abs(prevX[i]-x[i]) > eact )
eact  = abs(prevX[i]-x[i]);//Вычисление погрешности текущего приближения (разница между предыдущим и текущим решением)
}

    }
while(eact  >= eps);
delete [] prevX;
}



/**
 * Метод минимальных невязок
 */
void viktorovichev::lab6()
{
    double eps = 1e-15;

// Задаем вектор значений x на предыдущий итерации
	double *px = new double[N];
	for (int i = 0; i < N; i++) {
		px[i] = 0.0;
	}

// Задаем вектор невязок
	double *r = new double[N];

	int iteration = 0;
	while (true) {
		iteration++;

// Рассчитываем вектор невязки
		for (int i = 0; i < N; i++) {
			r[i] = b[i];

			for (int j = 0; j < N; j++) {
				r[i] -= (A[i][j] * px[j]);
			}
		}

// Рассчитываем итерационный параметр
		double t = 0.0;
		double temp = 0.0;
		for (int i = 0; i < N; i++) {
			double Ar = 0.0;

			for (int j = 0; j < N; j++) {
				Ar += (A[i][j] * r[j]);
			}

			t += (Ar * r[i]);
			temp += (Ar * Ar);
		}
		t /= temp;

// Рассчитывается новое приближение к вектору x
		for (int i = 0; i < N; i++) {
			x[i] = px[i] + t * r[i];
		}

// Посчитаем максимальную по модулю погрешность
		double err = 0.0;
		for (int i = 0; i < N; i++) {
			if (abs(x[i] - px[i]) > err) {
				err = abs(x[i] - px[i]);
			}
		}

// При достижении необходимой точности завершаем процесс
		if (err < eps) {
			break;
		}

// Текущее зачение итерации представим как предыдущее
		for (int i = 0; i < N; i++) {
			px[i] = x[i];
		}
	}

	cout << "Number of iterations : " << iteration << endl;

	delete[] px;
	delete[] r;
}



/**
 * Метод сопряженных градиентов
 */
void viktorovichev::lab7()
{
for (int i=0; i<N; i++)
              x[i]=0;
       double *r=new double[N];
       for (int i=0; i<N; i++){
            r[i]=b[i];
         for (int j=0; j<N; j++)
            r[i]-=A[i][j]*x[j];
       }
       double *z=new double[N];
         for (int i=0; i<N; i++)
            z[i]=r[i];
       double eps=10e-16;
       double var=0;
       double alpha = 0;
       for(;;){
            double differ=0, sum1=0, sum2=0, vec=0;
             for (int i=0; i<N; i++){
                vec=0;
                for (int k=0; k<N; k++)
                     vec+=A[i][k]*z[k];
                sum1+=r[i]*r[i];
                sum2+=vec*z[i];
            }
            alpha=sum1/sum2;
             for (int i=0; i<N; i++){
                    var=x[i];
            x[i]+=alpha*z[i];
                if(abs(x[i]-var)>differ)
                    differ=abs(x[i]-var);
                            }
            if(differ<eps) break;
            sum2=0;
            sum1=0;
             for (int i=0; i<N; i++){
                  vec=0;
                sum2+=r[i]*r[i];
                for (int j=0; j<N; j++)
                 vec+=A[i][j]*z[j];
                 r[i]-=alpha*vec;
                 sum1+=r[i]*r[i];

              }
            for (int i=0; i<N; i++){
                sum1+=r[i]*r[i];
                sum2+=vec*z[i];
            }
            for (int i=0; i<N; i++)
            z[i]=r[i]+sum1*z[i]/sum2;
      }
      delete[] r;
      delete[] z;
}


void viktorovichev::lab8()
{
double **H = new double*[N], eps = 1.e-10;
    for (int i = 0; i < N; i++) H[i] = new double[N];

    do
    {
        double n = 0;
        int i_max = 0, j_max = 1;
        for (int i = 0; i < N; i++)
            for (int j = i + 1; j < N; j++)
            {
                if (fabs(A[i][j]) > fabs(A[i_max][j_max]))
                {
                    i_max = i;
                    j_max = j;
                }

                n += A[i][j] * A[i][j];
            }

        if (sqrt(n) < eps) break;

        double fi = 0.5 * atan(2 * A[i_max][j_max] / (A[i_max][i_max] - A[j_max][j_max]));
        for (int i = 0; i < N; i++)
        {
            H[i][i_max] = A[i][i_max] * cos(fi) + A[i][j_max] * sin(fi);
            H[i][j_max] = A[i][j_max] * cos(fi) - A[i][i_max] * sin(fi);
        }

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (j != i_max && j != j_max) H[i][j] = A[i][j];

        for (int j = 0; j < N; j++)
        {
            A[i_max][j] = H[i_max][j] * cos(fi) + H[j_max][j] * sin(fi);
            A[j_max][j] = H[j_max][j] * cos(fi) - H[i_max][j] * sin(fi);
        }

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (i != i_max && i != j_max) A[i][j] = H[i][j];

    }while(true);

    for (int i = 0; i < N; i++) x[i] = A[i][i];

    for (int i = 0; i < N; i++) delete[] H[i];
    delete[] H;
}

void maslovma::lab9()
{
   double *new_x = new double[N], lam = 0, eps = 1.e-10;
    for (int i = 1; i < N; i++) x[i] = 0;

    x[0] = 1;
    do
    {
        double new_lam = 0;
        for (int i = 0; i < N; i++)
        {
            new_x[i] = 0;

            for (int j = 0; j < N; j++)
                new_x[i] += A[i][j] * x[j];

            new_lam += x[i] * new_x[i];
        }

        if (fabs(new_lam - lam) < eps) break;

        lam = new_lam;
        double n = 0;
        for (int i = 0; i < N; i++) n += new_x[i] * new_x[i];

        n = sqrt(n);
        for (int i = 0; i < N; i++) x[i] = new_x[i] / n;

    } while (true);

    x[0] = lam;
    delete[] new_x;
}


void viktorovichev::lab9()
{
 for (int i=0; i<N; i++)
                        x[i]=0;
                                x[0]=1;
       double *y=new double[N];
       double eps=1e-15;
       double prev_l;
       double l = 0;
       double sum;

       for(;;){
            sum = 0;
            prev_l = l;
            l = 0;
            for (int i = 0; i<N; i++){
                y[i] = 0;
                for (int k=0; k<N; k++)
                     y[i] += A[i][k]*x[k];
                l += y[i]*x[i];
                sum += y[i]*y[i];
            }
           sum = pow(sum,0.5);
             for (int i=0; i<N; i++)
                x[i] = y[i]/sum;
             if(abs(l - prev_l)<eps) break;
            }
            x[0] = l;
       delete[] y;
}


std::string viktorovichev::get_name()
{
  return "Viktorovichev E.V";
}

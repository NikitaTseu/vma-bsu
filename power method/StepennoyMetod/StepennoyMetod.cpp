#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

ofstream fout("out.txt");

// заполнение матрицы
void generateMatrix(float** a, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			a[i][j] = -1 * rand() % 5;
		}
	}
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < i; j++) {
			a[i][j] = a[j][i];
		}
	}
	for (int i = 0; i < n; i++) {
		a[i][i] = 0;
		for (int j = 0; j < n; j++) {
			if (i != j) {
				a[i][i] -= a[i][j];
			}
		}
	}
	a[0][0]++;
	return;
}

// вывод матрицы
void printMatrix(float** a, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (abs(a[i][j]) < 10) {
				cout << " ";
			}
			if (a[i][j] >= 0) {
				cout << " ";
			}
			cout << setprecision(1) << fixed << a[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

// вывод вектора
void printVector(float* a, int n) {
	for (int i = 0; i < n; i++) {
		cout << setprecision(3) << fixed << a[i] << " ";
	}
	cout << endl;
}

// копирование матрицы
float** copyMatrix(float** a, int n) {
	float** b = new float*[n];
	for (int i = 0; i < n; i++) {
		b[i] = new float[n];
		for (int j = 0; j < n; j++) {
			b[i][j] = a[i][j];
		}
	}
	return b;
}

float* Ay(float** a, float* y, int n) {
	float* ans = new float[n];
	for (int i = 0; i < n; i++) {
		ans[i] = 0;
		for (int j = 0; j < n; j++) {
			ans[i] += a[i][j] * y[j];
		}
	}
	return ans;
}

int sgn(float x) {
	if (x < -0.00000001) {
		return -1;
	}
	else {
		if (x > 0.00000001) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

// максимум-норма вектора
float getNorm(float* x, int n) {
	float norm = fabs(x[0]);
	for (int i = 1; i < n; i++) {
		if (fabs(x[i]) > norm) {
			norm = fabs(x[i]);
		}
	}
	return norm;
}

// поиск подходящего i для рассчета собственного значения 1
int findTheBest_i1(float* x, int n) {
	int tbi = 0;
	for (int i = 1; i < n; i++) {
		if (fabs(x[i]) > fabs(x[tbi])) {
			tbi = i;
		}
	}
	return tbi;
}

// поиск подходящего i для рассчета собственного значения 2
int findTheBest_i2(float* v, float* u, float l1, int n) {
	int tbi = 0;
	for (int i = 1; i < n; i++) {
		if (fabs(v[i] - l1 * u[i]) > fabs(v[tbi] - l1 * u[tbi])) {
			tbi = i;
		}
	}
	return tbi;
}

// декартово произведение двух векторов
float DecMultiply(float* x, float* y, int n) {
	float ans = 0;
	for (int i = 0; i < n; i++) {
		ans += x[i] * y[i];
	}
	return ans;
}

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int n = 12; // размер матрицы
	int n_iter = 50; // число итераций

	float** a = new float*[n]; // распределяем A
	for (int i = 0; i < n; i++) {
		a[i] = new float[n];
	}
	generateMatrix(a, n); // заполняем A
	/*a[0][0] = 1;
	a[0][1] = 2;
	a[0][2] = 3;

	a[1][0] = 2;
	a[1][1] = 1;
	a[1][2] = 2;

	a[2][0] = 3;
	a[2][1] = 2;
	a[2][2] = 1;*/

	cout << "Исходная матрица: " << endl;
	printMatrix(a, n);

	// рассчет u_n и v_n
	float** u = new float*[n_iter];
	float** v = new float*[n_iter];
	for (int i = 0; i < n_iter; i++) {
		u[i] = new float[n];
		v[i] = new float[n];
	}

	u[0][0] = 1;
	for (int j = 1; j < n; j++) {
		u[0][j] = 0;
	}

	for (int i = 1; i < n_iter; i++) {
		float* tmp = Ay(a, u[i - 1], n);
		for (int j = 0; j < n; j++) {
			v[i][j] = tmp[j];
		}
		delete[] tmp;
		
		float norm = getNorm(v[i], n);
		for (int j = 0; j < n; j++) {
			u[i][j] = v[i][j]/norm;
		}
	}

	// поиск собственного значения l1 для каждой итерации по первой формуле
	float* l1_1 = new float[n_iter];
	for (int i = 0; i < n_iter - 1; i++) {
		int tbi = findTheBest_i1(v[i + 1], n);
		l1_1[i] = v[i + 1][tbi] * sgn(u[i][tbi]);
	}

	// поиск собственного значения l1 для каждой итерации по второй формуле
	float* l1_2 = new float[n_iter];
	for (int i = 0; i < n_iter - 1; i++) {
		l1_2[i] = DecMultiply(v[i+1], u[i], n)/ DecMultiply(u[i], u[i], n);
	}

	// вычисление вектора-погрешности
	float* error1 = new float[n];
	for (int i = 0; i < n; i++) {
		error1[i] = v[n_iter - 2][i] - l1_2[n_iter - 2] * u[n_iter - 3][i];
	}
	float errNorm1 = getNorm(error1, n);

	cout << "Первое по модулю СЗ:" << endl;
	cout << "1 способ:" << endl;
	for (int i = 46; i <= 50; i++) {
		cout << "(k=" << i << ")  l1 = " << setprecision(5) << fixed << l1_1[i - 2] << endl;
	}
	cout << endl;
	for (int i = 46; i <= 50; i++) {
		cout << "(k=" << i << ")  u1 = "; printVector(u[i - 2], n);
	}
	cout << endl;
	cout << "2 способ:" << endl;
	for (int i = 46; i <= 50; i++) {
		cout << "(k=" << i << ")  l1 = " << setprecision(5) << fixed << l1_2[i - 2] << endl;
	}
	cout << "Норма вектора погрешности ||err1|| = " << setprecision(5) << fixed << errNorm1 << endl;

	cout << "-------------------------------------------------------------------------------------------------------" << endl;

	/*for (int i = 0; i < n_iter - 1; i++) {
		cout << setprecision(5) << fixed << l1_1[i] << "   " << l1_2[i] << endl;
	}
	cout << endl;*/

	// поиск собственного значения l2 (случай 1) и соответствующего собственного вектора
	int m = 28;
	float l2_1;
	int tbi = findTheBest_i2(v[m], u[m - 1], l1_1[n_iter - 2], n);
	l2_1 = (v[m + 1][tbi] * getNorm(v[m], n) - l1_1[n_iter - 2] * v[m][tbi]) / (v[m][tbi] - l1_1[n_iter - 2] * u[m - 1][tbi]);
	float* u2_1 = new float[n];
	for (int i = 0; i < n; i++) {
		u2_1[i] = v[m + 1][i] - l1_1[n_iter - 2] * u[m][i];
	}
	
	// поиск собственного значения l2 (случай 2) и соответствующего собственного вектора
	m = 48;
	float l2_2;
	tbi = findTheBest_i2(v[m], u[m - 1], l1_1[n_iter - 2], n);
	l2_2 = (v[m + 1][tbi] * getNorm(v[m], n) - l1_1[n_iter - 2] * v[m][tbi]) / (v[m][tbi] - l1_1[n_iter - 2] * u[m - 1][tbi]);
	float* u2_2 = new float[n];
	for (int i = 0; i < n; i++) {
		u2_2[i] = v[m + 1][i] - l1_1[n_iter - 2] * u[m][i];
	}

	// поиск собственного значения l2 (случай 3) и соответствующего собственного вектора
	m = 48;
	float l2_3;
	tbi = findTheBest_i2(v[m], u[m - 1], l1_2[n_iter - 2], n);
	l2_3 = (v[m + 1][tbi] * getNorm(v[m], n) - l1_2[n_iter - 2] * v[m][tbi]) / (v[m][tbi] - l1_2[n_iter - 2] * u[m - 1][tbi]);
	float* u2_3 = new float[n];
	for (int i = 0; i < n; i++) {
		u2_3[i] = v[m + 1][i] - l1_2[n_iter - 2] * u[m][i];
	}

	// вычисление вектора-погрешности
	float* error2 = new float[n];
	for (int i = 0; i < n; i++) {
		error2[i] = Ay(a, u2_1, n)[i] - l2_1 * u2_1[i];
	}
	float errNorm2_1 = getNorm(error2, n);

	for (int i = 0; i < n; i++) {
		error2[i] = Ay(a, u2_2, n)[i] - l2_2 * u2_2[i];
	}
	float errNorm2_2 = getNorm(error2, n);
	
	for (int i = 0; i < n; i++) {
		error2[i] = Ay(a, u2_3, n)[i] - l2_3 * u2_3[i];
	}
	float errNorm2_3 = getNorm(error2, n);

	cout << "Второе по модулю СЗ:" << endl;
	cout << "1 способ:" << endl << "l2 = " << setprecision(5) << fixed << l2_1 << endl;
	cout << "Cобственный вектор u2 = "; printVector(u2_1, n); 
	cout << "Норма вектора погрешности ||err2_1|| = " << setprecision(5) << fixed << errNorm2_1 << endl;
	cout << endl;
	cout << "2 способ:" << endl << "l2 = " << setprecision(5) << fixed << l2_2 << endl;
	cout << "Cобственный вектор u2 = "; printVector(u2_2, n);
	cout << "Норма вектора погрешности ||err2_2|| = " << setprecision(5) << fixed << errNorm2_2 << endl;
	cout << endl;
	cout << "3 способ:" << endl << "l2 = " << setprecision(5) << fixed << l2_3 << endl;
	cout << "Cобственный вектор u2 = "; printVector(u2_3, n); 
	cout << "Норма вектора погрешности ||err2_3|| = " << setprecision(5) << fixed << errNorm2_3 << endl;
	cout << endl;
	return 0;
}



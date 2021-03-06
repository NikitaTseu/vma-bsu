#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

ofstream fout("out.txt");

void generateMatrix(float** a, int n, float k, float m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			a[i][j] = 0;
		}
		if (i != 0 && i != (n - 1)) {
			a[i][i - 1] = -k;
			a[i][i] = m + k + i - 1;
			a[i][i + 1] = m + i - 1;
		}
	}

	a[0][0] = m;
	a[0][1] = m - 1;
	a[n - 1][n - 2] = -k;
	a[n - 1][n - 1] = m + k + n - 2;

	return;
}

void printAnswer(float* x, int n) {
	fout << endl << "Решение системы:" << endl << "y = (" << setprecision(9) << fixed << x[0] << "," << endl;
	for (int i = 1; i < n - 1; i++) {
		fout << "     " << setprecision(7) << fixed << x[i] << "," << endl;
	}
	fout << "     " << x[n - 1] << ")" << endl << endl;
}

void getB(float* b, float* x, float** a, int n) {
	for (int i = 0; i < n; i++) {
		float sum = 0;
		for (int j = 0; j < n; j++) {
			sum += (a[i][j] * x[j]);
		}
		b[i] = sum;
	}
}

void printSystem(float** a, float* b, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (a[i][j] == 0) {
				fout << " ";
			}
			fout << setprecision(2) << fixed << a[i][j] << " ";
			//fout << default << a[i][j] << " ";
			if (j + 1 == n) {
				fout << "| " << b[i];
			}
		}
		fout << endl;
	}
	fout << endl;
}

float getNorm(float* x, int n) {
	float norm = abs(x[0]);
	for (int i = 0; i < n; i++) {
		if (abs(x[i]) > norm) {
			norm = abs(x[i]);
		}
	}
	return norm;
}

float getError(float* x, float* x_ideal, int n) {
	float* xtmp = new float[n];
	for (int i = 0; i < n; i++) {
		xtmp[i] = x_ideal[i] - x[i];
	}
	return (getNorm(xtmp, n) / getNorm(x_ideal, n) * 100);
}

float getA(float** m, int i) {
	return -m[i][i-1];
}

float getC(float** m, int i) {
	return m[i][i];
}

float getB(float** m, int i) {
	return -m[i][i+1];
}

float* progonka(float** m, int n, float* f) {
	float* y = new float[n];
	float* a = new float[n];
	float* b = new float[n];

	a[1] = getB(m, 0) / getC(m, 0);
	b[1] = f[0] / getC(m, 0);

	for (int i = 2; i < n + 1; i++) {
		a[i] = getB(m, i - 1) / (getC(m, i - 1) - a[i - 1] * getA(m, i - 1));
		b[i] = (f[i - 1] + getA(m, i - 1) * b[i - 1]) / (getC(m, i - 1) - a[i - 1] * getA(m, i - 1));
	}

	b[n] = (f[n - 1] + getA(m, n - 1) * b[n - 1]) / (getC(m, n - 1) - a[n - 1] * getA(m, n - 1));

	y[n - 1] = b[n];
	for (int i = n - 2; i >= 0; i--) {
		y[i] = a[i + 1] * y[i + 1] + b[i + 1];
	}

	fout << "Матрица после прогонки: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j) {
				fout << setprecision(2) << fixed << " " << 1 << "    ";
			}
			else {
				
				
					if (i == j - 1) {
						fout << setprecision(2) << fixed << -a[i+1] << "  ";
					}
					else {
						fout << setprecision(2) << fixed << " " << 0 << "    ";
					}
				
			}
			if (j + 1 == n) {
				fout << "| " << b[i + 1] << endl;
			}
		}
		fout << endl;
	}
	fout << endl;

	return y;
}

int main()
{
	setlocale(LC_ALL, "RUS");
	int n = 12; // задаем значения констант
	float m = 21, k = 2;

	float** a = new float*[n]; // распределяем и заполняем временную квадратную матрицу A
	for (int i = 0; i < n; i++) {
		a[i] = new float[n];
	}
	generateMatrix(a, n, k, m);

	float* y_ideal = new float[n]; // распределяем и заполняем эталонный вектор значений y
	for (int i = 0; i < n; i++) {
		y_ideal[i] = i + 1;
	}
	fout << "Эталонный вектор значений: " << endl; // выводим эталонный вектор значений на экран
	printAnswer(y_ideal, n);

	float* f = new float[n]; // распределяем B
	getB(f, y_ideal, a, n); // рассчитываем B
	fout << "Расширенная матрица системы при k = 2, m = 21: " << endl;
	printSystem(a, f, n); // выводим на экран расширенную матрицу системы уравнений

	float* y = progonka(a, n, f);
	
	printAnswer(y, n);

	fout << endl << "Погрешность: E = " << getError(y, y_ideal, n) << "%" << endl;
	return 0;
}


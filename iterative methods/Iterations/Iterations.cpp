#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

ofstream fout("output.txt");

void generateMatrix(float** a, int n) {
	// Генерируем недиагональные элементы
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j) {
				a[i][j] = -(rand() % 5);
			}
		}
	}

	// Генерируем диагональные элементы
	for (int i = 0; i < n; i++) {
		int sum = 0;
		for (int j = 0; j < n; j++) {
			if (i != j) {
				sum += a[i][j];
			}
		}
		a[i][i] = -(sum);
	}

	a[0][0]++;

	return;
}

void printAnswer(float* x, int n) {
	fout << endl << "x = (" << setprecision(5) << fixed << x[0] << "," << endl;
	for (int i = 1; i < n - 1; i++) {
		fout << "     " << setprecision(5) << fixed << x[i] << "," << endl;
	}
	fout << "     " << x[n - 1] << ")" << endl;
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
			if (j + 1 == n) {
				fout << "| " << b[i];
			}
		}
		fout << endl;
	}
}

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

float* Jakobi(float** a, float* f, int& k, int k_max, float e, int n) {
	float* x = new float[n];
	for (int i = 0; i < n; i++) {
		x[i] = f[i] / a[i][i];
	}

	for (k = 0; k < k_max; k++) {
		bool endFlag = true;
		float* newX = new float[n];

		for (int i = 0; i < n; i++) {
			newX[i] = f[i];
			for (int j = 0; j < i; j++) {
				newX[i] -= x[j] * a[i][j];
			}
			for (int j = i + 1; j < n; j++) {
				newX[i] -= x[j] * a[i][j];
			}
			newX[i] /= a[i][i];
			if (abs(newX[i] - x[i]) > e) {
				endFlag = false;
			}
		}
		for (int i = 0; i < n; i++) {
			x[i] = newX[i];
		}
		delete[] newX;

		if(endFlag){
			break;
		}
	}

	return x;
}

float* Relax(float** a, float* f, int& k, int k_max, float e, int n, float w) {
	float* x = new float[n];
	for (int i = 0; i < n; i++) {
		x[i] = f[i] / a[i][i];
	}

	for (k = 0; k < k_max; k++) {
		bool endFlag = true;
		float* newX = new float[n];

		for (int i = 0; i < n; i++) {
			newX[i] = f[i];
			for (int j = 0; j < i; j++) {
				newX[i] -= newX[j] * a[i][j];
			}
			for (int j = i + 1; j < n; j++) {
				newX[i] -= x[j] * a[i][j];
			}
			newX[i] /= a[i][i];
			newX[i] *= w;
			newX[i] += (1 - w) * x[i];
			if (abs(newX[i] - x[i]) > e) {
				endFlag = false;
			}
		}
		for (int i = 0; i < n; i++) {
			x[i] = newX[i];
		}
		delete[] newX;

		if (endFlag) {
			break;
		}
	}

	return x;
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

void printMessage(int k, int k_max, float* x, int n) {
	if (k < k_max) {
		fout << "Решение найдено на итерации k = " << k;
	}
	else {
		fout << "Требуемая точность не была достигнута за " << k_max << " итераций." << endl;
		fout << "Приближенное решение на итерации k_max = " << k_max;
	}
	printAnswer(x, n);
}

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int n = 5, k_max = 1000;
	float m = 21, e = 1.0 / 1000;

	float** a = new float*[n]; // распределяем и заполняем A
	for (int i = 0; i < n; i++) {
		a[i] = new float[n];
	}
	generateMatrix(a, n);

	float* x_ideal = new float[n]; // распределяем и заполняем эталонный вектор x
	for (int i = 0; i < n; i++) {
		x_ideal[i] = m + i;
	}
	fout << "Эталонный вектор значений: " << endl;
	printAnswer(x_ideal, n);

	float* f = new float[n]; // распределяем f
	getB(f, x_ideal, a, n); // рассчитываем f
	fout << "Расширенная матрица системы: " << endl;
	printSystem(a, f, n);
	
	float** a1 = copyMatrix(a, n); // создаем копию A
	int k1 = 0;
	float* x1 = Jakobi(a1, f, k1, k_max, e, n); // рассчет методом Якоби
	printMessage(k1, k_max, x1, n);

	float** a2 = copyMatrix(a, n); // создаем копию A
	int k2 = 0;
	float* x2 = Relax(a2, f, k2, k_max, e, n, 0.5); // рассчет методом релаксации для w = 0.5
	printMessage(k2, k_max, x2, n);

	float** a3 = copyMatrix(a, n); // создаем копию A
	int k3 = 0;
	float* x3 = Relax(a3, f, k3, k_max, e, n, 1.0); // рассчет методом релаксации для w = 1.0
	printMessage(k3, k_max, x3, n);

	float** a4 = copyMatrix(a, n); // создаем копию A
	int k4 = 0;
	float* x4 = Relax(a4, f, k4, k_max, e, n, 1.5); // рассчет методом релаксации для w = 1.5
	printMessage(k4, k_max, x4, n);

	fout << endl << "Погрешность метода Якоби: E = " << getError(x1, x_ideal, n) << "%" << endl;
	fout << endl << "Погрешность метода релаксации (w = 0.5): E = " << getError(x2, x_ideal, n) << "%" << endl;
	fout << endl << "Погрешность метода релаксации (w = 1.0): E = " << getError(x3, x_ideal, n) << "%" << endl;
	fout << endl << "Погрешность метода релаксации (w = 1.5): E = " << getError(x4, x_ideal, n) << "%" << endl << endl;

	return 0;
}


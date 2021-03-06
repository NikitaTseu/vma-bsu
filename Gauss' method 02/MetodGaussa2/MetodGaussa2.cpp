#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

void generateMatrix(float** a, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			a[i][j] = rand() % 100;
			if (rand() % 2 == 0) {
				a[i][j] *= -1;
			}
		}
	}
	return;
}

void printAnswer(float* x, int n) {
	cout << "x = (" << x[0] << "," << endl;
	for (int i = 1; i < n - 1; i++) {
		cout << "     " << setprecision(5) << fixed << x[i] << "," << endl;
		//cout << "     " << x[i] << "," << endl;
	}
	cout << "     " << x[n - 1] << ")" << endl;
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
				cout << " ";
			}
			cout << setprecision(2) << fixed << a[i][j] << " ";
			//cout << default << a[i][j] << " ";
			if (j + 1 == n) {
				cout << "| " << b[i];
			}
		}
		cout << endl;
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

float* Gauss1(float** a, float* b, int n) {
	float* x = new float[n];

	// Прямой ход метода Гаусса
	// k - идем по переменным
	// i - идем по строкам, зануляя переменные
	for (int k = 0; k < n - 1; k++) {
		for (int i = k + 1; i < n; i++) {
			float l = a[i][k] / a[k][k];
			a[i][k] = 0;
			b[i] = b[i] - b[k] * l;
			for (int j = k + 1; j < n; j++) {
				a[i][j] = a[i][j] - l * a[k][j];
			}
		}
		if (k == 0) {
			cout << endl << " Расширенная матрица после первого шага алгоритма: " << endl;
			printSystem(a, b, n);
			cout << endl;
		}
	}

	// Обратный ход метода Гаусса
	for (int i = n - 1; i >= 0; i--) {
		x[i] = b[i];
		for (int j = n - 1; j > i; j--) {
			x[i] -= a[i][j] * x[j];
		}
		x[i] /= a[i][i];
	}

	return x;
}

float* Gauss2(float** a, float* b, int n) {
	float* x = new float[n];

	// Прямой ход метода Гаусса
	// k - идем по переменным
	// i - идем по строкам, зануляя переменные
	for (int k = 0; k < n - 1; k++) {

		int imax = k;

		for (int i = k + 1; i < n; i++) {
			if (abs(a[i][k]) > abs(a[imax][k])) {
				imax = i;
			}
		}

		for (int j = k; j < n; j++) {
			swap(a[imax][j], a[k][j]);
		}
		swap(b[imax], b[k]);

		if (k == 0) {
			cout << endl << " Расширенная матрица после выбора ведущего элемента:" << endl;
			printSystem(a, b, n);
			cout << endl;
		}

		for (int i = k + 1; i < n; i++) {
			float l = a[i][k] / a[k][k];
			a[i][k] = 0;
			b[i] = b[i] - b[k] * l;
			for (int j = k + 1; j < n; j++) {
				a[i][j] = a[i][j] - l * a[k][j];
			}
		}

		if (k == 0) {
			cout << endl << " Расширенная матрица после " << k+1 << " шага алгоритма: " << endl;
			printSystem(a, b, n);
			cout << endl;
		}
	}

	// Обратный ход метода Гаусса
	for (int i = n - 1; i >= 0; i--) {
		x[i] = b[i];
		for (int j = n - 1; j > i; j--) {
			x[i] -= a[i][j] * x[j];
		}
		x[i] /= a[i][i];
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

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int n = 15;
	float m = 21;

	float** a = new float*[n]; // распределяем A
	for (int i = 0; i < n; i++) {
		a[i] = new float[n];
	}

	float* x_ideal = new float[n]; // распределяем и заполняем эталонный вектор x
	for (int i = 0; i < n; i++) {
		x_ideal[i] = m + i;
	}
	cout << "Эталонный вектор значений: " << endl;
	printAnswer(x_ideal, n);
	cout << endl;

	generateMatrix(a, n); // заполняем A

	float* b = new float[n]; // распределяем B
	getB(b, x_ideal, a, n); // рассчитываем B
	cout << "Расширенная матрица системы: " << endl << endl;
	printSystem(a, b, n);
	cout << endl;

	float** atmp = copyMatrix(a, n); // создаем копию A
	float* x1 = Gauss1(atmp, b, n); // рассчет методом Гаусса без выбора ведущего элемента
	cout << "Решение без выбора ведущего элемента: " << endl;
	printAnswer(x1, n);
	cout << endl;

	atmp = copyMatrix(a, n);
	getB(b, x_ideal, atmp, n);
	cout << "Расширенная матрица системы: " << endl << endl;
	printSystem(atmp, b, n);
	cout << endl;

	float* x2 = Gauss2(atmp, b, n);// рассчет методом Гаусса с выбором ведущего элемента
	cout << "Решение с выбором ведущего элемента: " << endl;
	printAnswer(x2, n);
	cout << endl; 

	cout << endl << "Погрешность без выбора ведущего элемента: E = " << getError(x1, x_ideal, n) << "%" << endl;
	cout << "Погрешность с выбором ведущего элемента:  E = " << getError(x2, x_ideal, n) << "%" << endl << endl;

	for (int i = 0; i < n; i++) {
		delete[] a[i];
		delete[] atmp[i];
	}
	delete[] a;
	delete[] atmp;
	delete[] b;
	delete[] x1;
	delete[] x2;
	return 0;
}


#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

void generateFirst(float** a, int n, int k) {

	// Генерируем первый элемент
	a[0][0] = 0;
	for (int j = 1; j < n; j++) {
		a[0][0] -= a[0][j];
	}
	float p = 1;
	for (int i = 0; i < k; i++) {
		p /= 10;
	}
	a[0][0] += p;
}

void generateMatrix(float** a, int n, int k) {
	// Генерируем недиагональные элементы
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j) {
				a[i][j] = -(rand() % 5);
			}
		}
	}

	// Генерируем диагональные элементы
	for (int i = 1; i < n; i++) {
		int sum = 0;
		for (int j = 0; j < n; j++) {
			if (i != j) {
				sum += a[i][j];
			}
		}
		a[i][i] = -(sum);
	}


	generateFirst(a, n, k);
	return;
}

void printAnswer(float* x, int n) {
	cout << endl << "Решение системы:" << endl << "x = (" << x[0] << "," << endl;
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

float* Gauss(float** a, float* b, int n) {
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
	int n = 15, k = 0;
	float m = 21;

	float** a = new float*[n]; // распределяем и заполняем A
	for (int i = 0; i < n; i++) {
		a[i] = new float [n];
	}
	generateMatrix(a, n, k);

	float* x_ideal = new float[n]; // распределяем и заполняем эталонный вектор x
	for (int i = 0; i < n; i++) {
		x_ideal[i] = m + i;
	}
	cout << "Эталонный вектор значений: " << endl;
	printAnswer(x_ideal, n);

	float* b = new float[n]; // распределяем B
	getB(b, x_ideal, a, n); // рассчитываем B
	cout << "Расширенная матрица системы при k=0: " << endl;
	printSystem(a, b, n);

	float** atmp = copyMatrix(a, n); // создаем копию A
	float* x1 = Gauss(atmp, b, n); // первый рассчет методом Гаусса для k = 0
	printAnswer(x1, n);
	
	k = 2;
	atmp = copyMatrix(a, n);
	generateFirst(atmp, n, k);
	getB(b, x_ideal, atmp, n);
	cout << "Расширенная матрица системы при k=2: " << endl;
	printSystem(atmp, b, n);

	float* x2 = Gauss(atmp, b, n);
	printAnswer(x2, n); // второй рассчет методом Гаусса для k = 2

	cout << endl << "Погрешность при k=0: E = " << getError(x1, x_ideal, n) << "%" << endl;
	cout << "Погрешность при k=2: E = " << getError(x2, x_ideal, n) << "%" << endl << endl;

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


#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

//создание первого элемента матрицы
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

//заполнение матрицы (за исключением первого элемента)
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

//вывод решения системы
void printAnswer(float* x, int n) {
	for (int i = 0; i < n - 1; i++) {
		cout << "     " << setprecision(5) << fixed << x[i] << "," << endl;
	}
	cout << "     " << x[n - 1] << ")" << endl;
}

//рассчет столбца b по известному решению системы
void getB(float* b, float* x, float** a, int n) {
	for (int i = 0; i < n; i++) {
		float sum = 0;
		for (int j = 0; j < n; j++) {
			sum += (a[i][j] * x[j]);
		}
		b[i] = sum;
	}
}

//sgn(x)
int sgn(float a) {
	if (a < 0) {
		return -1;
	}
	else {
		return 1;
	}
}

//вывод системы
void printSystem(float** a, float* b, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (a[i][j] < 10 && a[i][j] >= 0) {
				cout << " ";
			}
			cout << setprecision(2) << fixed << a[i][j] << " ";
			if (j + 1 == n) {
				cout << "| " << b[i];
			}
		}
		cout << endl;
	}
	cout << endl;
}

//копирование Матрицы
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

//метод Гаусса
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

//рассчет евклидовой нормы вектора
float getENorm(float* x, int n) {
	float norm = 0;
	for (int i = 0; i < n; i++) {
		norm += x[i] * x[i];
	}
	return sqrt(norm);
}

//рассчет нормы вектора при вычислении погрешности
float getNorm(float* x, int n) {
	float norm = abs(x[0]);
	for (int i = 0; i < n; i++) {
		if (abs(x[i]) > norm) {
			norm = abs(x[i]);
		}
	}
	return norm;
}

//вычисление погрешности
float getError(float* x, float* x_ideal, int n) {
	float* xtmp = new float[n];
	for (int i = 0; i < n; i++) {
		xtmp[i] = x_ideal[i] - x[i];
	}
	return (getNorm(xtmp, n) / getNorm(x_ideal, n) * 100);
}

//метод Хаусхолдера
float* Hausholder(float** a, float* b, int n) {
	float* x = new float[n]; // здесь хранится решение системы
	float sNorm, zNorm;

	for (int k = 0; k < n - 1; k++) {
		//рассчет вектора w
		float* s = new float[n - k];
		float* z = new float[n - k];

		for (int i = k; i < n; i++) {
			s[i - k] = a[i][k];
		}
		sNorm = getENorm(s, n - k);

		z[0] = s[0] + sgn(a[k][k]) * sNorm;
		for (int i = k + 1; i < n; i++) {
			z[i - k] = s[i - k];
		}
		zNorm = getENorm(z, n - k);
		for (int i = k; i < n; i++) {
			z[i - k] /= zNorm;
		}
		//теперь в векторе z лежат значения вектора w для текущего шага алгоритма

		float m = 0;//скалярное произведение w и a
		for (int i = k; i < n; i++) {
			m += a[i][k] * z[i - k];
		}
		a[k][k] = a[k][k] - 2 * m * z[0];
		for (int i = k + 1; i < n; i++) { //зануляем столбец под диагональю
			a[i][k] = 0;
		}

		for (int j = k + 1; j < n; j++) { //рассчитываем обновленные значения в остальных столбцах
			m = 0;
			for (int i = k; i < n; i++) {
				m += a[i][j] * z[i - k];
			}
			for (int i = k; i < n; i++) {
				a[i][j] = a[i][j] - 2 * m * z[i - k];
			}
		}
		m = 0;
		for (int i = k; i < n; i++) {
			m += b[i] * z[i - k];
		}
		for (int i = k; i < n; i++) { //рассчитываем обновленные значения вектора b

			b[i] = b[i] - 2 * m * z[i - k];
		}

		if (k == 0) {
			cout << "Расширенная матрица системы после 1 шага метода Хаусхолдера:" << endl;
			printSystem(a, b, n);
		}

		delete[] z;
		delete[] s;
	}

	// Обратный ход метода отражений
	for (int i = n - 1; i >= 0; i--) {
		x[i] = b[i];
		for (int j = n - 1; j > i; j--) {
			x[i] -= a[i][j] * x[j];
		}
		x[i] /= a[i][i];
	}

	return x;
}

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int n = 12, k = 2;
	float m = 21;

	float** a = new float*[n]; // распределяем и заполняем A
	for (int i = 0; i < n; i++) {
		a[i] = new float[n];
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
	cout << "Расширенная матрица системы при k=2: " << endl;
	printSystem(a, b, n);

	float** atmp = copyMatrix(a, n); // создаем копию A
	float* x1 = Gauss(atmp, b, n); // рассчет методом Гаусса
	cout << "Решение методом Гаусса: " << endl;
	printAnswer(x1, n);

	getB(b, x_ideal, a, n); // рассчитываем B
	float* x2 = Hausholder(a, b, n); // рассчет методом Хаусхолдера
	cout << "Решение методом Хаусхолдера: " << endl;
	printAnswer(x2, n);

	//рассчитываем погрешности
	cout << endl << "Погрешность при рассчете методом Гаусса: E = " << getError(x1, x_ideal, n) << "%" << endl;
	cout << "Погрешность при рассчете методом Хаусхолдера: E = " << getError(x2, x_ideal, n) << "%" << endl << endl;
	return 0;
}

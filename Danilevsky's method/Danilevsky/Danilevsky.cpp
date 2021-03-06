#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

ofstream fout("out.txt");

void generateMatrix(float** a, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			a[i][j] = rand() % 51;
			if (rand() % 2 == 0) {
				a[i][j] *= -1;
			}
		}
	}
	return;
}

void printMatrix(float** a, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (abs(a[i][j]) < 10) {
				fout << " ";
			}
			if (a[i][j] >= 0) {
				fout << " ";
			}
			fout << setprecision(1) << fixed << a[i][j] << " ";
		}
		fout << endl;
	}
	fout << endl;
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

bool isZero(float x) {
	return x < 0.00000001;
}

float** nextStep(float** A, float** M, float** M_obr, int n) {
	float** tmpA1 = new float*[n];
	float** tmpA2 = new float*[n];
	for (int i = 0; i < n; i++) {
		tmpA1[i] = new float[n];
		tmpA2[i] = new float[n];
	}

	float tmpElem = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			tmpElem = 0;
			for (int p = 0; p < n; p++) {
				tmpElem += M_obr[i][p] * A[p][j];
			}
			tmpA1[i][j] = tmpElem;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			tmpElem = 0;
			for (int p = 0; p < n; p++) {			
				tmpElem += tmpA1[i][p] * M[p][j];
			}
			tmpA2[i][j] = tmpElem;
		}
	}

	return tmpA2;
}

float*** Danilevskiy(float** a, int n) {
	float*** ans = new float**[n];
	for (int i = 0; i < n; i++) {
		ans[i] = new float*[n];
		for (int j = 0; j < n; j++) {
			ans[i][j] = new float[n];
		}
	}
	ans[n-1] = a;

	float** M_obr = new float*[n];
	for (int i = 0; i < n; i++) {
		M_obr[i] = new float[n];
	}

	// метод Данилевского
	for (int k = n - 2; k >= 0; k--) {

		// формирование матрицы M для очередной итерации
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (i == j) {
					ans[k][i][j] = 1;
				}
				else {
					ans[k][i][j] = 0;
				}
			}
		}
		for (int j = 0; j < n; j++) {
			if (j != k) {
				ans[k][k][j] = -(ans[n - 1][k + 1][j]);
			}
			ans[k][k][j] /= ans[n - 1][k + 1][k];
		}

		// формирование матрицы M^(-1) для очередной итерации
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (i == j) {
					M_obr[i][j] = 1;
				}
				else {
					M_obr[i][j] = 0;
				}
			}
		}
		for (int j = 0; j < n; j++) {
			M_obr[k][j] = ans[n - 1][k + 1][j];
		}

		// перемножение трёх матриц
		ans[n-1] = nextStep(ans[n - 1], ans[k], M_obr, n);

		// приводим очередную строку к форме Фробениуса
		for (int j = 0; j < n; j++) {
			if (j != k) {
				ans[n - 1][k + 1][j] = 0;
			}
			else {
				ans[n - 1][k + 1][j] = 1;
			}
		}
	}

	return ans;
}

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int n = 4;

	float** a = new float*[n]; // распределяем A
	for (int i = 0; i < n; i++) {
		a[i] = new float[n];
	}
	generateMatrix(a, n); // заполняем A
	float** a1 = copyMatrix(a, n); // создаем копию A

	fout << "Исходная матрица: " << endl;
	printMatrix(a, n);

	float*** ans = Danilevskiy(a, n);

	fout << "Каноническая форма Фробениуса: " << endl;
	printMatrix(ans[n - 1], n);

	fout << " p1 = " << ans[n - 1][0][0] << endl;
	float SpA = 0;
	for (int i = 0; i < n; i++) {
		SpA += a[i][i];
	}
	fout << "SpA = " << SpA << endl << endl;

	for (int i = 0; i < n - 1; i++) {
		fout << "Матрица M" << i << ":" << endl;
		printMatrix(ans[n - 2 - i], n);
	}

	return 0;
}



#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <vector>

using namespace std;

ofstream fout("out.txt");

void generateMatrix(double** a, int n) {
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

void printMatrix(double** a, int n) {
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

double** copyMatrix(double** a, int n) {
	double** b = new double*[n];
	for (int i = 0; i < n; i++) {
		b[i] = new double[n];
		for (int j = 0; j < n; j++) {
			b[i][j] = a[i][j];
		}
	}
	return b;
}

bool isZero(double x) {
	return x < 0.00000001;
}

double** nextStep(double** A, double** M, double** M_obr, int n) {
	double** tmpA1 = new double*[n];
	double** tmpA2 = new double*[n];
	for (int i = 0; i < n; i++) {
		tmpA1[i] = new double[n];
		tmpA2[i] = new double[n];
	}

	double tmpElem = 0;

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

double*** Danilevskiy(double** a, int n) {
	double*** ans = new double**[n];
	for (int i = 0; i < n; i++) {
		ans[i] = new double*[n];
		for (int j = 0; j < n; j++) {
			ans[i][j] = new double[n];
		}
	}
	ans[n - 1] = a;

	double** M_obr = new double*[n];
	for (int i = 0; i < n; i++) {
		M_obr[i] = new double[n];
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
		ans[n - 1] = nextStep(ans[n - 1], ans[k], M_obr, n);

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

double C(double* a, int n) {
	double m = a[1];
	for (int i = 1; i < n; i++) {
		if (a[i] > m) {
			m = a[i];
		}
	}
	return m;
}

class Interval {
public:
	double left, right;
	Interval(double l, double r) {
		left = l;
		right = r;
	}
};

class Polynomial {
public:
	double* a;
	int deg;

public:
	Polynomial(int _deg) {
		deg = _deg;
		a = new double[deg + 1];
		for (int i = 0; i < deg + 1; i++) {
			a[i] = 0;
		}
	}

	Polynomial(double* mas, int _deg) {
		deg = _deg;
		a = new double[deg + 1];
		for (int i = 0; i < deg + 1; i++) {
			a[i] = mas[i];
		}
	}

	Polynomial derivative() {
		if (deg == 0) {
			return Polynomial(0);
		}
		else {
			double* b = new double[deg];
			for (int i = 1; i < deg + 1; i++) {
				b[i - 1] = a[i] * i;
			}
			return Polynomial(b, deg - 1);
		}
	}

	double calculate(double x) {
		double ans = 0;
		for (int i = 0; i < deg + 1; i++) {
			double t = 1;
			for (int j = 0; j < i; j++) {
				t *= x;
			}
			ans += a[i] * t;
		}
		return ans;
	}
	
	vector<Interval> separateRoots(vector<double> g) {
		double l, r;
		r = 1 + C(a, deg + 1) / abs(a[deg]);
		l = (-1.0)*r;

		vector<Interval> v;
		sort(g.begin(), g.end());
		if (g.size() == 0) {
			v.push_back(Interval(l, r));
		}
		else {
			vector<double> x = g;
			v.push_back(Interval(l, x[0]));
			for (int i = 0; i < x.size() - 1; i++) {
				v.push_back(Interval(x[i], x[i + 1]));
			}
			v.push_back(Interval(x[x.size() - 1], r));
		}
		return v;
	}
	
	friend ostream& operator << (ostream &s, const Polynomial &p);
};

ostream& operator << (ostream &s, const Polynomial &p)
{
	for (int i = p.deg; i > 0; i--) {
		if (p.a[i] < 0) {
			s << "- " << abs(p.a[i]) << "*x^" << i << " ";
		}
		else {
			s << "+ " << p.a[i] << "*x^" << i << " ";
		}
	}
	if (p.a[0] < 0) {
		s << "- " << abs(p.a[0]);
	}
	else {
		s << "+ " << p.a[0];
	}
	return s;
}

ostream& operator << (ostream &s, const Interval &i)
{
	s << "[" << i.left << "; " << i.right << "]";
	return s;
}

vector<double> solveQuadratic(Polynomial p){
	vector<double> ans;
	double D = p.a[1] * p.a[1] - 4 * p.a[0] * p.a[2];
	if (D > 0) {
		ans.push_back(((-1.0)*p.a[1] + sqrt(D)) / (2 * p.a[2]));
		ans.push_back(((-1.0)*p.a[1] - sqrt(D)) / (2 * p.a[2]));
	}
	return ans;
}

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));
	int n = 4;

	double** a = new double*[n]; // распределяем A
	for (int i = 0; i < n; i++) {
		a[i] = new double[n];
	}
	generateMatrix(a, n); // заполняем A
	double** a1 = copyMatrix(a, n); // создаем копию A

	cout << "Исходная матрица: " << endl;
	printMatrix(a, n);

	double*** ans = Danilevskiy(a, n);

	cout << "Каноническая форма Фробениуса: " << endl;
	printMatrix(ans[n - 1], n);

	double* tmp = new double[n + 1];
	tmp[0] = ans[n - 1][0][3];
	tmp[1] = ans[n - 1][0][2];
	tmp[2] = ans[n - 1][0][1];
	tmp[3] = ans[n - 1][0][0];
	tmp[4] = 1;

	Polynomial p = Polynomial(tmp, n);
	/*p.a[0] = -161832.0;
	p.a[1] = 30017.0;
	p.a[2] = 712.0;
	p.a[3] = -50.0;
	p.a[4] = 1.0;*/
	cout << "Собственный многочлен: "<< p << endl << endl;

	Polynomial p1 = p.derivative();
	cout << "Первая производная: " << p1 << endl << endl;
	Polynomial p2 = p1.derivative();
	cout << "Вторая производная: " << p2 << endl << endl;

	vector<double> v2; //массив корней второй производной
	v2 = solveQuadratic(p2);
	vector<double> new_v2;
	for (int i = 0; i < v2.size(); i++) {
		if (p1.calculate(v2[i] + 0.1) * p1.calculate(v2[i] - 0.1) < 0) {
			new_v2.push_back(v2[i]);
		}
	}
	v2 = new_v2;

	cout << "Отделение корней первой производной:" << endl;
	vector<Interval> intervals1 = p1.separateRoots(v2); //отделение корней первой производной
	for (int i = 0; i < intervals1.size(); i++) {
		cout << intervals1[i] << endl;
	}
	cout << endl;

	vector<double> v1_1; //массив корней первой производной по Ньютону
	for (int k = 0; k < intervals1.size(); k++) {
		double x = (intervals1[k].right + intervals1[k].left) / 2;
		for (int i = 0; i < 50; i++) {
			x -= p1.calculate(x) / p2.calculate(x);
		}
		v1_1.push_back(x);
	}

	cout << "Корни первой производной по методу Ньютона:" << endl;
	for (int i = 0; i < intervals1.size(); i++) {
		cout << setprecision(4) << fixed << v1_1[i] << endl;
	}
	cout << endl;

	//vector<double> v1_2; //массив корней первой производной по МПИ
	//for (int k = 0; k < 3; k++) {
	//	double x = (intervals1[k].right + intervals1[k].left) / 2;
	//	for (int i = 0; i < 50; i++) {
	//		x = (p1.a[3] / p1.a[1])*x*x*x + (p1.a[2] / p1.a[1])*x*x + (p1.a[0] / p1.a[1]);
	//		x *= (-1.0);
	//	}
	//	v1_2.push_back(x);
	//}

	vector<double> v1_2; //массив корней первой производной по МПИ
	for (int k = 0; k < intervals1.size(); k++) {
		double x = (intervals1[k].right + intervals1[k].left) / 2;
		for (int i = 0; i < 50; i++) {
			x = sqrt(((-1.0)*p1.a[1]*x - p1.a[0]) / (p1.a[3] * x + p1.a[2]));
		}
		v1_2.push_back(x);
	}

	cout << "Корни первой производной по МПИ:" << endl;
	for (int i = 0; i < intervals1.size(); i++) {
		cout << setprecision(4) << fixed << v1_2[i] << endl;
	}
	cout << endl;


	//////////////////////////////////////////////////////////////////

	cout << "Отделение корней мночлена:" << endl;
	vector<Interval> intervals0 = p.separateRoots(v1_1);
	for (int i = 0; i < intervals0.size(); i++) {
		cout << intervals0[i] << endl;
	}
	cout << endl;

	vector<double> v0_1; //массив корней первой производной по Ньютону
	for (int k = 0; k < intervals0.size(); k++) {
		double x = (intervals0[k].right + intervals0[k].left) / 2;
		for (int i = 0; i < 50; i++) {
			x -= p.calculate(x) / p1.calculate(x);
		}
		v0_1.push_back(x);
	}

	cout << "Корни многочлена по методу Ньютона:" << endl;
	for (int i = 0; i < intervals0.size(); i++) {
		cout << setprecision(4) << fixed << v0_1[i] << endl;
	}
	cout << endl;

	vector<double> v0_2; //массив корней первой производной по методу дихотомии
	for (int k = 0; k < intervals0.size(); k++) {
		double r = intervals0[k].right;
		double l = intervals0[k].left;
		for (int i = 0; i < 50; i++) {
			double x = (r + l) / 2;
			if (p.calculate(x)*p.calculate(r) < 0) {
				l = x;
			}
			else {
				r = x;
			}
		}
		v0_2.push_back((r+l)/2);
	}

	cout << "Корни многочлена по методу дихотомии:" << endl;
	for (int i = 0; i < intervals0.size(); i++) {
		cout << setprecision(4) << fixed << v0_2[i] << endl;
	}
	cout << endl;

	return 0;
}
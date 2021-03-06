#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct s_i {
	int *s;
	int i;
};

int Count(string s, int t) {
	struct cout {
		char key;
		int n;
	};
	struct cout *arr = new struct cout[t];
	for (int i = 0; i < t; i++) {
		arr[i].n = 0;
		for (int j = 0; j < t; j++)
		{
			if (s[i] == s[j]) {
				arr[i].key = s[i];
				arr[i].n++;
			}
		}
	}
	int max_count = 0;
	for (int i = 0; i < t; i++) {
		if (arr[i].n > max_count) {
			max_count = arr[i].n;
		}
	}
	return max_count;
}

int Score(int *s, string *dna, int l, int t) {
	string *alignment_matrix = new string[t];
	for (int i = 0; i < t; i++) {
		alignment_matrix[i] = dna[i].substr(s[i], l);//составляем матрицу substr - возвращает подстроку с s[i] позиции, l - количество символов
	}
	string *t_matrix = new string[l];
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < t; j++) {
			t_matrix[i] += alignment_matrix[j].substr(i, 1); //транспонированная матрица
		}

	}
	int score = 0;
	for (int i = 0; i < l; i++) {
		score = score + Count(t_matrix[i], t);
	}
	return score;
}

int* Next_Leaf(int* a, int l, int k) {
	for (int i = l - 1; i >= 0; i--) {
		if (a[i] < k - 1) {
			a[i]++;
			return a;
		}
		a[i] = 0;
	}
	return a;
}

void All_Leaves(int l, int k) {
	int *s = new int[l];
	for (int i = 0; i < l; i++) {
		s[i] = 0;
	}
	for (;;) {
		for (int i = 0; i < l; i++) {
			cout << s[i];
		}
		cout << endl;
		s = Next_Leaf(s, l, k);
		int n;
		for (n = 0; n < l && s[n] == 0; n++) {
			;
		}
		if (n == l) return;

	}
}

string* Motifs(int* s, string* dna, int t, int l) {
	string* best_motifs = new string[t];
	for (int i = 0; i < t; i++) {
		best_motifs[i] = dna[i].substr(s[i], l);
	}
	return best_motifs;
}

struct s_i NextVertex(int* a, int i, int l, int k) {
	struct s_i a_i;
	a_i.s = new int[l];
	a_i.i = i;
	for (int h = 0; h < l; h++) {
		a_i.s[h] = a[h];
	}
	if (a_i.i < l) {
		a_i.s[a_i.i+1] = 0;
		a_i.i++;
		return a_i;
	}
	else {
		for (int j = l-1; j >= 0; j--) {
			if (a_i.s[j] < k-1) {
				a_i.s[j] = a_i.s[j] + 1;
				a_i.i = j;
				return a_i;
			}
		}
	}
	a_i.i = -1;
	return a_i;
}

struct s_i ByPass(int* a, int i, int l, int k) {
	struct s_i a_i;
	a_i.s = new int[l];
	a_i.i = i;
	for (int h = 0; h < l; h++) {
		a_i.s[h] = a[h];
	}
	for (int j = l-1; j >= 0; j--) {
		if (a_i.s[j] < k - 1) {
			a_i.s[j] = a_i.s[j] + 1;
			a_i.i = j;
			return a_i;
		}
	}
	a_i.i = -1;
	return a_i;
}

string* BrunchAndBoundMotifSearch(string *dna, int t, int n, int l) {
	string *best_motifs = new string[t];
	int *best_s = new int[t];
	struct s_i si;
	si.s = new int[t];
	si.i = 0;
	for (int i = 0; i < t; i++) {
		si.s[i] = 0;
	}
	int best_score = 0;
	int optimistic_score;
	while (si.i > -1) {
		if (si.i < t) {
			optimistic_score = Score(si.s, dna, l, si.i) + (t - si.i)*l;
			if (optimistic_score < best_score) {
				si = ByPass(si.s, si.i, t, n - l + 1);
								
			}
			else {
				si = NextVertex(si.s, si.i, t, n - l + 1); 
			
			}
		}
		else {
			if (Score(si.s, dna, l, si.i) >= best_score) {
				best_score = Score(si.s, dna, l, si.i);
				for (int i = 0; i < t; i++) {
					best_s[i] = si.s[i];
				}
				best_motifs = Motifs(si.s, dna, t, l);
			}
			si = NextVertex(si.s, si.i, t, n - l + 1);
		}
	}
	return best_motifs;
}


int main() {
	ifstream fin("input.txt");

	if (!fin.is_open())
		cout << "Файл не может быть открыт!\n";
	else
	{
		int l;//размер шаблона
		int t;//число последовательностей 

		fin >> l;

		fin >> t;

		string *arr_str = new string[t];

		for (int i = 0; i < t; i++) {
			fin >> arr_str[i];
		}
		int n = arr_str[0].length();

		string* best_motifs = new string[t];

		best_motifs = BrunchAndBoundMotifSearch(arr_str, t, n, l);


		ofstream fout("output.txt");

		for (int i = 0; i < t; i++) {
			fout << best_motifs[i] << endl;
		}


		fin.close();
		fout.close();

	}
	return 0;
}


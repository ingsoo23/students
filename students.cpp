#include <iostream>
#include <fstream>
#include <string.h>
#include <cstring>
#include <string>
#include <stdlib.h>
#include "bptree.h"
using namespace std;

void insert_DB(ofstream& fout, Student student) {
	student.toPrint(fout);
}

int main() {
	int N;
	int k;
	string tmp;

	ifstream fin;
	fin.open("sampleData.csv");
	ofstream fout1, fout2;
	fout1.open("Student.DB");
	fout2.open("Students_score.idx");

	getline(fin, tmp);
//	N = stoi(tmp);

	for (int j = 0; j < 10; j++)
		insert_DB(fout1, to_student(fin));
    cin >> k;

	fin.close();
	fout1.close();
    fout2.close();
	return 0;
}

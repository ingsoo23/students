#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "students.h"
#include "dynamic_hash.h"
#include "DB.h"

#pragma pack(1)

using namespace std;

Block::Block()
{
	Record_Count = 0;
	Bit_Num = 0;
}

Students to_student(ifstream& fin) {
	int i = 0, pos = 0;
	string tmp;
	Students student;
	getline(fin, tmp);
	pos = tmp.find(",", i);
	strcpy(student.name, tmp.substr(i, pos - i).c_str());
	i = pos + 1;
	pos = tmp.find(",", i);
	student.studentID = stoi(tmp.substr(i, pos - i));
	i = pos + 1;
	pos = tmp.find(",", i);
	student.score = stof(tmp.substr(i, pos - i));
	i = pos + 1;
	student.advisorID = stoi(tmp.substr(i));

	return student;
}

int main() {
	int N;
	unsigned int tmpID;
	string tmp;
	Students student_tmp;
	_DB testDB;

	ifstream fin;
	fin.open("sampleData.csv");

	getline(fin, tmp);
	N = stoi(tmp);

	testDB.Open();

	for (int i = 0; i < N; i++) {
		student_tmp = to_student(fin);
		testDB.Insert(student_tmp);
	}

	testDB.Print();

	cout << "ID search! Enter ID : ";
	cin >> tmpID;

	testDB.ID_Search(tmpID);

	fin.close();
	testDB.Close();

	return 0;
}
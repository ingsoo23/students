#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef struct {
	char name[20];
	unsigned studentID;
	float score;
	unsigned advisorID;
} students;

void insert_DB(ofstream& fout, students student) {
	fout << student.name << ", " << student.studentID << ", " << student.score << ", " << student.advisorID << endl;
}

students to_student(ifstream& fin) {
	int i = 0, pos = 0;
	string tmp;
	students student;
	getline(fin, tmp);
	pos = tmp.find(",", i);
	strncpy_s(student.name, tmp.substr(i, pos - i).c_str(), _TRUNCATE);
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
	string tmp;

	ifstream fin;
	fin.open("sampleData.csv");
	ofstream fout;
	fout.open("Student.DB");

	getline(fin, tmp);
	N = stoi(tmp);

	for (int j = 0; j < N; j++)
		insert_DB(fout, to_student(fin));

	fin.close();
	fout.close();

	return 0;
}
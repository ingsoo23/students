#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "bptree.h"
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
	//unsigned int tmpID;
	int k =0 ;
	string tmp;
	Students* students;
	_DB testDB;

	BPNode* root = new BPNode();

	ifstream fin;
	fin.open("sampleData.csv");

	ofstream fout;
	fout.open("Students_score.idx");

	getline(fin, tmp);
	N = stoi(tmp);

	//unsigned int IDtmp[N];
	testDB.Open();
	students = new Students[N];
	for (int i = 0; i < N; i++) {
		students[i] = to_student(fin);
		testDB.Insert(students[i]);
		//IDtmp[i] = student_tmp.studentID;
		// tree.Insert(student_tmp);
	}

	testDB.Print();

	cout << "Enter the leaf number" << endl;
	cin >> k;

	for (int i = 0; i < N; i++) {
		//cout << testDB.BlockNum(students[i].studentID) << endl;
		root->Insert(students[i],testDB.BlockNum(students[i].studentID));
	}
	// root->Print(fout);
	// root->Print(k);
	fin.close();
	testDB.Close();
	fout.close();
//	N = stoi(tmp);
//  Student* students;
//  for (int i = 0; i<N; i++){
//  insert_DB(); // treanfer the excel data 'fin' into Students class
//  Hashfunction(students[i]); // choose BLOCK where this student data stored
//  }
//  fout1 <<Hash result
//  BPNode* root;
//  for(int i =0;i<N; i++){
//  root.insert(student[i]);
//  }
//  fout2 << BPTree;
//  cout << K-th leaf node;

    // cin >> k;
	// tree.Print(k);
	delete [] students;
	return 0;
}
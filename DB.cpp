#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include "students.h"
#include "dynamic_hash.h"
#include "DB.h"

#pragma pack(1)

using namespace std;

template <typename T>
std::string to_string(T value) {
	std::ostringstream os;
	os << value;
	return os.str();
}

void _DB::Open()
{
	DBFile.open("Student.DB", ios::in | ios::out | ios::binary);
	if (!DBFile)
	{
		DBFile.clear();
		DBFile.open("Student.DB", ios::in | ios::out | ios::binary | ios::trunc);
		DBFile.seekp(0, ios::beg);
		DBFile.write((char*)&DB_Buffer, sizeof(Block));
	}
	H = new Dynamic_Hash("Student.Hash");
}

void _DB::Close()
{
	DBFile.close();
}

bool _DB::Insert(Students student)
{
	bool Is_Insert = false;
	///// Dynamic Hash�� ����Ͽ� Database�� �����Ѵ�.
	int DB_File_Offset = H->Get_Hash_Offset(to_string(student.studentID));                // �ش� ID Record�� �����ؾ� �� Block Offset�� Return.

	DBFile.seekg(DB_File_Offset, ios::beg);                          // �ش� Block�� �о� �´�.      
	DBFile.read((char*)&DB_Buffer, sizeof(Block));

	// Insert�Ǵ� Record�� ID�� ���� Record�� �̹� �����ϴ� ��츦 Check. 
	for (int i = 0; i<DB_Buffer.Record_Count; i++)
		if (DB_Buffer.Record[i].studentID == student.studentID)
			return false;

	if (DB_Buffer.Record_Count < IN_BLOCK_MAX)                        // Buffer Block�� �� �������� ���.
	{
		// Record Insertion.
		strcpy(DB_Buffer.Record[DB_Buffer.Record_Count].name, student.name);

		DB_Buffer.Record[DB_Buffer.Record_Count].studentID = student.studentID;
		DB_Buffer.Record[DB_Buffer.Record_Count].score = student.score;
		DB_Buffer.Record[DB_Buffer.Record_Count].advisorID = student.advisorID;

		DB_Buffer.Record_Count++;
		// Modified�� Buffer�� Database�� ���ش�.
		DBFile.seekp(DB_File_Offset, ios::beg);
		DBFile.write((char*)&DB_Buffer, sizeof(Block));
		Is_Insert = true;
	}
	else                                                             // Buffer Block�� ���� ���.
	{
		H->Block_Full(to_string(student.studentID), DB_Buffer.Bit_Num, DBFile);
		Insert(student);
	}
	return true;
}

void _DB::Print()
{
	H->Print_Hash_Table();
}

/*
void _DB::ID_Search(unsigned int ID)
{
	long DB_File_Offset = H->Get_Hash_Offset(to_string(ID));               // �ش� ID Record�� �����ϴ� Block Offset�� Return.

	DBFile.seekg(DB_File_Offset, ios::beg);                          // �ش� Block�� �о� �´�.      
	DBFile.read((char*)&DB_Buffer, sizeof(Block));

	int i;
	for (i = 0; i<DB_Buffer.Record_Count; i++)
		if (DB_Buffer.Record[i].studentID == ID)          // Case : ID�� �ش��ϴ� Record�� �����ϴ� ���.		
		{
			cout << "Key : " << DB_File_Offset << ", Name : " << DB_Buffer.Record[i].name << ", StudentID : " << DB_Buffer.Record[i].studentID << ", Score : " << DB_Buffer.Record[i].score << ", AdvisorID : " << DB_Buffer.Record[i].advisorID << endl;
			cout << "Record count : " << DB_Buffer.Record_Count << endl;
			return;
		}

	cout << "No Record Look Up." << endl;
}
*/

int _DB::BlockNum(unsigned int ID)
{
	for (int i = 0; i < BLOCKSIZE / sizeof(long); i++) {
		if (H->Hash_Table.Table_Block_Offset[i] == H->Get_Hash_Offset(to_string(ID)))
			return i;
	}
}

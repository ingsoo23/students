#include <iostream>
#include <fstream>
#include <string>
#include "students.h"
#include "dynamic_hash.h"

#pragma pack(1)

using namespace std;

HashTable::HashTable()
{
	for (int i = 0; i < (BLOCKSIZE / sizeof(long)); i++)                   // 1024
		Table_Block_Offset[i] = -1;
}

// Hash_File내 [0..4]까지는 Table_Bit_Num이 쓰여지고, [4..4100]은 Hash_Table이 쓰여진다.
Dynamic_Hash::Dynamic_Hash(char* Hash_File_Name)
{
	Hash_File.open(Hash_File_Name, ios::binary | ios::in | ios::out);
	if (!Hash_File)                                                   // Hash_File이 존재하지 않을 경우.
	{
		Hash_File.clear();
		Hash_File.open(Hash_File_Name, ios::binary | ios::in | ios::out | ios::trunc);
		Table_Bit_Num = 0;
		Hash_File.write((char*)&Table_Bit_Num, sizeof(Table_Bit_Num));
		Hash_Table.Table_Block_Offset[0] = 0;                        // 첫 번째 Block은 DBFile내 Offset 0에 저장되어있다.
		Hash_File.write((char*)&Hash_Table, sizeof(Hash_Table));
	}
	else
	{
		Hash_File.read((char*)&Table_Bit_Num, sizeof(Table_Bit_Num));// Hash_File에 저장된 Table_Bit_Num을 읽어드린다.
		Hash_File.read((char*)&Hash_Table, sizeof(Hash_Table));      // Hash_File에 저장된 Hash Table을 읽어드린다.
	}
}

Dynamic_Hash::~Dynamic_Hash()
{
	Hash_File.seekp(0, ios::beg);
	Hash_File.write((char*)&Table_Bit_Num, sizeof(Table_Bit_Num));
	Hash_File.write((char*)&Hash_Table, sizeof(Hash_Table));
	Hash_File.close();
}

unsigned int Dynamic_Hash::_Hash(string str)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;

	for (unsigned int i = 0; i < str.length(); i++)
	{
		hash = (hash * seed) + str[i];
	}

	hash = hash % 1024;

	unsigned int eax, ebx, ecx;
	eax = hash;
	ebx = 0;
	ecx = 32;
_while:
	if ((eax >> 31) & 1 == 1) {
		eax = eax << 1;
		goto _ADD;
	}
	eax = eax << 1;
	ebx = ebx >> 1;
	ecx--;
	if (ecx != 0)
		goto _while;

	goto EXIT;
_ADD:
	ebx = ebx >> 1;
	ebx += 1024 * 1024 * 1024 * 2;
	ecx--;
	if (ecx != 0)
		goto _while;
EXIT:
	hash = ebx;

	return hash;
}

long Dynamic_Hash::Get_Hash_Offset(string s_ID)
{
	unsigned int _Hash_Key = _Hash(s_ID);

	// Table_Bit_Num 수만큼의 상위 Bits만 Valid하므로 Shift.
	if (Table_Bit_Num == 0)
	{
		_Hash_Key = 0;
	}
	else
	{
		_Hash_Key = _Hash_Key >> (32 - Table_Bit_Num);
	}
	return Hash_Table.Table_Block_Offset[_Hash_Key];
}

void Dynamic_Hash::Block_Full(string s_ID, int Block_Bit_Num, fstream& DB_File)
{
	unsigned int _Hash_Key = _Hash(s_ID);
	if (Table_Bit_Num != 0)
		_Hash_Key = _Hash_Key >> (32 - Table_Bit_Num);
	else
		_Hash_Key = 0;

	if (Block_Bit_Num == Table_Bit_Num)                               // Hash_Table을 확장해야하는 경우.
	{
		Extend_Table(_Hash_Key, DB_File);
		return;
	}
	else if (Block_Bit_Num < Table_Bit_Num)                           // Hash_Table의 확장이 필요없는 경우.             
	{
		Block* Buffer = new Block;                                 // Temp Buffer;
		DB_File.seekp(0, ios::end);
		long New_Block_Offset = DB_File.tellp();                   // 새로운 Block의 OFfset.
		Block* New_Block = new Block;                              // 새로운 Block.
		Block* Old_Block = new Block;                              // 기존의 Block.

		DB_File.seekg(Hash_Table.Table_Block_Offset[(int)_Hash_Key], ios::beg);
		DB_File.read((char*)Buffer, sizeof(Block));
		Old_Block->Bit_Num = Buffer->Bit_Num + 1;
		New_Block->Bit_Num = Buffer->Bit_Num + 1;

		int How_Many = Table_Bit_Num - Block_Bit_Num;
		int start = -1, end = -1;
		int i;
		for (i = 0; i<1024; i++)
		{
			if (start == -1 && Hash_Table.Table_Block_Offset[i] == Hash_Table.Table_Block_Offset[(int)_Hash_Key])
				start = i;
			if (start != -1 && Hash_Table.Table_Block_Offset[i] != Hash_Table.Table_Block_Offset[(int)_Hash_Key])
			{
				end = i - 1;
				break;
			}
		}

		// Hash_Table내 Offset 설정.
		for (i = start; i <= (start + (end - start) / 2); i++)
			Hash_Table.Table_Block_Offset[i] = Hash_Table.Table_Block_Offset[_Hash_Key];

		for (i = (start + (end - start) / 2) + 1; i <= end; i++)
			Hash_Table.Table_Block_Offset[i] = New_Block_Offset;

		// OverFlow가 발생한 Block을 읽어드려서, Record를 재분배.

		int Low_Count = 0;
		int High_Count = 0;
		unsigned int m_Hash_Key;
		char m_c_ID[30];
		string m_s_ID;

		for (int j = 0; j < 127; j++)
		{
			sprintf(m_c_ID, "%d", Buffer->Record[j].studentID);
			m_s_ID = m_c_ID;
			m_Hash_Key = _Hash(m_s_ID);
			m_Hash_Key = m_Hash_Key >> (32 - Table_Bit_Num);
			if ((int)m_Hash_Key < (start + (end - start) / 2) + 1)
				Old_Block->Record[Old_Block->Record_Count++] = Buffer->Record[j];
			else
			{
				New_Block->Record[New_Block->Record_Count++] = Buffer->Record[j];
			}
		}

		DB_File.seekp(Hash_Table.Table_Block_Offset[start], ios::beg);
		DB_File.write((char*)Old_Block, sizeof(Block));
		DB_File.seekp(New_Block_Offset, ios::beg);
		DB_File.write((char*)New_Block, sizeof(Block));

		Hash_File.seekp(0, ios::beg);
		Hash_File.write((char*)&Table_Bit_Num, sizeof(Table_Bit_Num));
		Hash_File.write((char*)&Hash_Table, sizeof(Hash_Table));

		delete Buffer;
		delete New_Block;
		delete Old_Block;
	}

}

void Dynamic_Hash::Extend_Table(unsigned int _Hash_Key, fstream& DB_File)
{
	Block* Buffer = new Block;                                     // Temp Buffer;

	DB_File.seekp(0, ios::end);
	long New_Block_Offset = DB_File.tellp();                          // 새로운 Block의 OFfset.
	Block* New_Block = new Block;                                    // 새로운 Block.
	Block* Old_Block = new Block;
	// Table Bit Num을 통하여 Table에 Valid한 Offset을 가진 것의 갯수를 구한다.
	unsigned int Table_Block_Num = 2;
	if (Table_Bit_Num != 0)
		Table_Block_Num = Table_Block_Num << (Table_Bit_Num - 1);
	else
		Table_Block_Num = 1;
	for (int i = Table_Block_Num - 1; i >= 0; i--)                          // 현재 Hash_Table의 모든 Valid한 Block에 대하여 Loop.
	{
		if (i == (int)_Hash_Key)                                   // Case : OverFlow가 발생한 Block
		{
			Hash_Table.Table_Block_Offset[2 * i] = Hash_Table.Table_Block_Offset[i];
			Hash_Table.Table_Block_Offset[2 * i + 1] = New_Block_Offset;

			// OverFlow가 발생한 Block을 읽어드려서, Record를 재분배.
			DB_File.seekg(Hash_Table.Table_Block_Offset[i], ios::beg);
			DB_File.read((char*)Buffer, sizeof(Block));

			Table_Bit_Num++;
			Old_Block->Bit_Num = Table_Bit_Num;
			New_Block->Bit_Num = Table_Bit_Num;

			int Low_Count = 0;
			int High_Count = 0;
			unsigned int m_Hash_Key;
			char c_ID[30];
			string s_ID;

			for (unsigned int j = 0; j < 127; j++)
			{
				sprintf(c_ID, "%d", Buffer->Record[j].studentID);
				s_ID = c_ID;
				m_Hash_Key = _Hash(s_ID);
				m_Hash_Key = m_Hash_Key >> (32 - Table_Bit_Num);
				if (m_Hash_Key == 2 * i)
					Old_Block->Record[Old_Block->Record_Count++] = Buffer->Record[j];
				else
				{
					New_Block->Record[New_Block->Record_Count++] = Buffer->Record[j];
				}
			}

			DB_File.seekp(Hash_Table.Table_Block_Offset[2 * i], ios::beg);
			DB_File.write((char*)Old_Block, sizeof(Block));
			DB_File.seekp(New_Block_Offset, ios::beg);
			DB_File.write((char*)New_Block, sizeof(Block));
		}
		else                                                         // Case : OverFlow가 발생한 Block 외 Block.
		{
			Hash_Table.Table_Block_Offset[2 * i] = Hash_Table.Table_Block_Offset[i];
			Hash_Table.Table_Block_Offset[2 * i + 1] = Hash_Table.Table_Block_Offset[i];
		}
	}

	Hash_File.seekp(0, ios::beg);
	Hash_File.write((char*)&Table_Bit_Num, sizeof(Table_Bit_Num));
	Hash_File.write((char*)&Hash_Table, sizeof(Hash_Table));

	delete Buffer;
	delete New_Block;
	delete Old_Block;
}

void Dynamic_Hash::Print_Hash_Table() {
	for (int i = 0; i < BLOCKSIZE / sizeof(long); i++) {
		if (Hash_Table.Table_Block_Offset[i] == -1)
			break;
		cout << "Offset : ";
		Bin(i);
		cout << ", Table Bit Num : " << Table_Bit_Num << ", Key : " << Hash_Table.Table_Block_Offset[i] << endl;
	}
}

void Dynamic_Hash::Bin(int x) {
	int tmp;

	for (int i = Table_Bit_Num; i > 0; i--) {
		tmp = x >> (i - 1);
		cout << (tmp & 1);
	}
}
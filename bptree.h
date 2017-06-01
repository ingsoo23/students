#ifndef BPTREE_H_INCLUDED
#define BPTREE_H_INCLUDED
using namespace std;
#include "student.h"


class BPNode {
private:
	BPNode* nextleaf;
	BPNode** childnode;
	BPNode* parentnode;
	Student* students;
	bool leaf;
	int in;
	int n;
public:
	BPNode() {
		nextleaf = NULL;
		childnode = NULL;
		parentnode = NULL;
		students = NULL;
		leaf = true;
		n = (BLOCKSIZE - (sizeof(nextleaf) + sizeof(childnode) + sizeof(parentnode) + sizeof(leaf) + sizeof(in) + sizeof(n))) / (sizeof(students));
		in = 0;
	}
	void Sort() {
		Student* temp;
		BPNode* childtem;
		int i;
		for (i = 0; i<in - 1; i++) {
			if (this->students[in - 1].score < this->students[i].score) {
				break;
			}
		}
		temp = this->students[in - 1];
		for (int j = in; j>i + 1; j--) {
			students[j - 1] = students[j - 2];
		}
		students[i] = temp;
		if (!this->isLeaf()) {
			BPNode* tmp = new BPNode();
			tmp->leaf = false;
			tmp->childnode[in] = new BPNode();
			tmp->childnode[in]->leaf = tmp->childnode[0]->leaf;
			childtem = tmp->childnode[in];
			for (int j = in; j > i; j--) {
				tmp->childnode[j] = tmp->childnode[j - 1];
			}
			tmp->childnode[i] = childtem;
		}
		/*  else{
		int tempBNum = this->BNum[in-1];
		for(int j = in; j>i+1;j--)
		BNum[j-1] = BNum[j-2];
		BNum[i] = tempBNum;
		}
		*/
	}
	BPNode* Copy() {
		BPNode* b;
		b->parentnode = this->parentnode;
		b->leaf = this->leaf;
		b->n = this->n;
		return b;
	}
	bool isOver() {
		return (n < in);
	}
	void Split() {
		BPNode* tmp;
		if (this->parentnode == NULL) { //When this node is a root.
			BPNode* new1 = new BPNode();
			BPNode* new2 = new BPNode();
			for (int k = this->in / 2 + 1; k<in; k++) {
				new2->students[k] = this->students[k];
				new2->in++;
				this->students[k] = 0;
			}
			for (int j = 0; j< in / 2 + 1; j++) {
				new1->students[j] = this->students[j];
				new1->in++;
				this->students[j] = 0;
			}
			new1->nextleaf = new2;
			this->students[0] = new1->students[new1->in - 1];
			this->childnode[0] = new1;
			this->childnode[1] = new2;
			this->leaf = false;
			this->in = 1;
			new1->parentnode = this;
			new2->parentnode = this;
		}
		else { //When this node is not a root.
			BPNode* new1 = this->Copy();
			for (int k = this->in / 2 + 1; k<this->in; k++) {
				new1->students[k] = this->students[k];
				new1->in++;
				this->students[k] = 0;
			}
			this->in = in / 2;
			if (new1->isLeaf()) {
				new1->nextleaf = this->nextleaf;
				this->nextleaf = new1;
			}
			//insert parent¿¡ scores[in/2]
			//childnode Á¤¸®

		}


	}
	bool isEmpty() {
		return (in == 0);
	}
	bool isLeaf() {
		return leaf;
	}
	void Insert(Student s) {
		int i = 0;
		BPNode* tmp;
		tmp = this;
		if (tmp->isEmpty()) {
			tmp->students[0] = s;
		}
		else {
			while (!tmp->isLeaf()) {
				for (i = 0; i< tmp->in; i++) {
					if (s.getscore()< tmp->scores[0]) {
						break;
					}
					else if (s.getscore() > tmp->scores[i] && s.getscore() < tmp->scores[i + 1]) {
						i++;
						break;
					}
					else if (s.getscore() == tmp->scores[i]) {
						break;
					}
					else
						continue;
				}
				tmp = tmp->childnode[i];
			}
			tmp->students[tmp->in] = s;
			tmp->Sort();
		}
		if (tmp->isOver()) {
			tmp->Split();
		}
		/*
		else{
		if(tmp->isLeaf()){
		tmp->scores[tmp->in]=s.getscore();
		tmp->Sort();
		}
		else{
		while(!tmp->isLeaf()){
		for(i = 0; i< tmp->in; i++){
		if(s.getscore()< tmp->scores[0]){
		break;
		}
		else if(s.getscore() > tmp->scores[i] && s.getscore() < tmp->scores[i+1]){
		i++;
		break;
		}
		else if(s.getscore() == tmp->scores[i]){
		break;
		}
		else
		continue;
		}
		tmp = tmp->childnode[i];
		}
		tmp->score[tmp->in] = s.getscore();
		tmp->Sort();
		}}*/

		tmp->in++;
	}
	void LeafPrint(ofstream& of) {
		BPNode* tmp;
		tmp = this;
		while (!tmp->isLeaf()) {
			tmp = tmp->childnode[0];
		}
		while (tmp != NULL) {
			for (int i = 0; i<tmp->in; i++) {
				of << tmp->scores[i] << " ";
			}
			tmp = tmp->nextleaf;
			of << endl;
		}
	}
	void Print(int k) {
		BPNode* a = this;
		while (!a->isLeaf()) {
			a = a->childnode[0];
		}
		int i = k;
		while (i != 1) {
			a = a->nextleaf;
			if (i <0 || i == 0) {
				cout << k << " is too much big number" << endl;
				break;
			}
		}
		for (int j = 0; j<a->in; j++) {
			cout << a->scores[j] << " ";
		}
		cout << endl;
	}
};




#endif // BPTREE_H_INCLUDED
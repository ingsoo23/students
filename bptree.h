#ifndef BPTREE_H_INCLUDED
#define BPTREE_H_INCLUDED
using namespace std;
#include "students.h"
#define IN_NODE_MAX ((BLOCKSIZE - sizeof(Students)) / (sizeof(Students)+sizeof(int)+sizeof(int*)))

class BPNode{
private:
    BPNode* nextleaf;
    BPNode* childnode[IN_NODE_MAX+1];
	BPNode* parentnode;
    Students students[IN_NODE_MAX];
	int BNum[IN_NODE_MAX];
    bool leaf;
    int in;
	char blank[BLOCKSIZE - IN_NODE_MAX*(sizeof(Students) + sizeof(int) + sizeof(int*))];
public:
    BPNode(){
        nextleaf = NULL;
		parentnode = NULL;
		for(int i = 0; i < IN_NODE_MAX+1 ; i++){
			childnode[i] = NULL;
		}
        leaf = true;
        in = 0;
    }
    bool isOver(){
        return (IN_NODE_MAX < in);
    }
    void Sort(){
        Students temp;
		int BNtemp;
		int a = this->in;
        BPNode* childtem = new BPNode();
        int i, j;
		temp = Copy(this->students[a-1]);
		BNtemp = BNum[a-1];
        for (i =0; i<a-1; i++){
            if(temp.score < this->students[i].score){
                break;
            }
        }
        for(j=a;j>i+1;j--){
            students[j-1] = Copy(students[j-2]);
			BNum[j-1] = BNum[j-2];
        }
        students[i] = Copy(temp);
		BNum[i] = BNtemp;
        if(!this->isLeaf()){
            childtem = this->childnode[a];
        //    tmp->leaf = false;
        //    tmp->childnode[in] = BPNode();
        //    tmp->childnode[in]->leaf = tmp->childnode[0]->leaf;
         //   childtem = tmp->childnode[in];
            for(j = a; j > i; j--){
				this->childnode[j] = this->childnode[j-1];
            }
			
            this->childnode[i] = childtem;
        }
      /*  else{
            int tempBNum = this->BNum[in-1];
            for(int j = in; j>i+1;j--)
                BNum[j-1] = BNum[j-2];
            BNum[i] = tempBNum;
        }
        */
    }
    BPNode* Copy(){
        BPNode* b = new BPNode();
		b->parentnode = this->parentnode;
        b->leaf = this->leaf;
        return b;
    }
    void Split(){
		//Students* mid;
		//int* mid;
		int k =0;
		int j =0;
		int s;
        BPNode* parenttmp;
        BPNode* childtem;
        if(this->parentnode == NULL){ //When this node is a root.
			
            BPNode* new1 = new BPNode();
            BPNode* new2 = new BPNode();
			s =0;
            for(k = IN_NODE_MAX/2+1; k<in; k++){
				
                new2->students[s] = Copy(this->students[k]);
				
				new2->BNum[s] = this->BNum[k];
                new2->in++;
				s++;
            }
            for(j = 0; j< IN_NODE_MAX/2+1 ; j++){
                new1->students[j] = Copy(this->students[j]);
				new1->BNum[j] = this->BNum[j];
                new1->in++;
            }
			
            new1->nextleaf = new2;
            this->students[0] = Copy(new1->students[new1->in-1]);
			this->BNum[0] = new1->BNum[0];
            this->childnode[0] = new1;
            this->childnode[1] = new2;
            this->leaf = false;
            this->in = 1;
            new1->parentnode = this;
            new2->parentnode = this;
        }
        else{ //When this node is not a root.
			s =0;
            BPNode* new1 = new BPNode();
			new1 = this->Copy();
			int k;
			//childtem = new BPNode();
			//childtem = 
            for(k = IN_NODE_MAX/2+2; k<IN_NODE_MAX; k++){
                new1->students[s] = Copy(this->students[k]);
				new1->BNum[s] = this->BNum[k];
                new1->in++;
				s++;
                //this->students[k] =0;
            }
            this->in = IN_NODE_MAX/2 +1;
            if(new1->isLeaf()){
                new1->nextleaf = this->nextleaf;
                this->nextleaf = new1;
            }
			parenttmp = this->parentnode;
			parenttmp->students[parenttmp->in] = Copy(new1->students[new1->in]);
			parenttmp->BNum[parenttmp->in] = new1->BNum[new1->in];
			parenttmp->in++;
			parenttmp->childnode[parenttmp->in] = new1;
			parenttmp->Sort();
			if(parenttmp->isOver())
				parenttmp->Split();
            //insert parent�� scores[in/2]
            //childnode ����

        }


    }
    bool isEmpty(){
        return (in == 0);}
    bool isLeaf(){
        return leaf;}
    void Insert(Students stu, int Bnum){
		int t= 0;
        int i = 0;
        BPNode* tmp = new BPNode();
        tmp = this;

        if(tmp->in == 0){
			tmp->students[0] = Copy(stu);
			tmp->BNum[0] = Bnum;
        }
        else{
            while(!tmp->isLeaf()){
				//cout << tmp->childnode[0]->students[0].score;
                for(i = 0; i< tmp->in; i++){
					
					//cout << tmp->in;
                    if(stu.score < tmp->students[0].score){
                        break;
                    }
                    else if(stu.score > tmp->students[i].score && stu.score < tmp->students[i].score){
                        i++;
                        break;
                    }//?
                    else if(stu.score == tmp->students[i].score){
                        break;
                    }
                    else
                        continue;
                }
                tmp = tmp->childnode[i];
            }
            tmp->students[tmp->in] = Copy(stu);
			//tmp->BNum[tmp->in] = Bnum;
            tmp->Sort();
        }
        tmp->in++;
        if (tmp->isOver()){
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

    }
    void Print(fstream& of){
		int s = 0;
		int i =0;
        BPNode* tmp = this;
        while(!tmp->isLeaf()){
            tmp = tmp->childnode[0];
        }
		/*
        while(tmp!= NULL){
			of << "LEAF NODE No." << s+1 << endl;
            for(i = 0; i<tmp->in; i++){
				of << tmp->students[i].score << ", " << tmp->students[i].studentID << "," << tmp->BNum[i] <<endl;
            }
            tmp = tmp->nextleaf;
			of << "-----------------------------------------------------------------------" << endl;
			s++;
        }
		*/
		of.open("Students_score.idx", ios::in | ios::out | ios::binary);
		if(!of)
			of.open("Students_score.idx", ios::in | ios::out | ios::binary | ios::trunc);
		while (tmp != NULL) {
			of.write((char*)tmp, sizeof(BPNode));
			tmp = tmp->nextleaf;
		}
		of.close();
    }

    void Print(int k){
		int j =0;
        BPNode* tmp = this;
        while(!tmp->isLeaf()){
            tmp = tmp->childnode[0];
        }
        int i = 0;
		for( i =0;i<k;i++){
			tmp = tmp->nextleaf;
		}
		if(tmp!= NULL){
			for(j =0; j< tmp->in ;j++){
				cout << tmp->students[j].score << ", " << tmp->students[j].studentID << "," << tmp->BNum[j] <<endl;
			}
		}
		else
			cout << k << "is too big." << endl;
    }

	Students Copy(Students& a){
		Students b;
		strcpy(b.name, a.name);
		b.studentID = a.studentID;
		b.score = a.score;
		b.advisorID = a.advisorID;
		return b;
	}
};


#endif // BPTREE_H_INCLUDED

#ifndef BPTREE_H_INCLUDED
#define BPTREE_H_INCLUDED
using namespace std;
#include "students.h"
#define IN_NODE_MAX (BLOCKSIZE - (sizeof(bool)+sizeof(int)+2*sizeof(int*))) / (sizeof(Students)+sizeof(int)+sizeof(int*))

class BPNode{
private:
    BPNode* nextleaf;
    BPNode* childnode[IN_NODE_MAX+1];
	BPNode* parentnode;
    Students students[IN_NODE_MAX];
	int BNum[IN_NODE_MAX];
    bool leaf;
    int in;
	char blank[BLOCKSIZE - ((sizeof(bool)+sizeof(int)+2*sizeof(int*)) % (sizeof(Students)+sizeof(int)+sizeof(int*)))];
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
        BPNode* childtem;
        int i;
        for (i =0; i<in-1; i++){
            if(this->students[in-1].score < this->students[i].score){
                break;
            }
        }
        for(int j=in;j>i+1;j--){
            students[j-1] = Copy(students[j-2]);
			BNum[j-1] = BNum[j-2];
        }
        students[i] = temp;
		BNum[i] = BNtemp;
        if(!this->isLeaf()){
            childtem = this->childnode[in];
        //    tmp->leaf = false;
        //    tmp->childnode[in] = BPNode();
        //    tmp->childnode[in]->leaf = tmp->childnode[0]->leaf;
         //   childtem = tmp->childnode[in];
            for(int j = in; j > i; j--){
				this->childnode[j]=this->childnode[j-1];
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
        BPNode* parenttmp;
        BPNode* childtem;
        if(this->parentnode == NULL){ //When this node is a root.
            BPNode* new1 = new BPNode();
            BPNode* new2 = new BPNode();
            for(int k = this->in/2+1; k<in; k++){
                new2->students[k] = Copy(this->students[k]);
				new2->BNum[k] = this->BNum[k];
                new2->in++;
				this->BNum[k] = 0;
                //this->students[k] = Copy(Students());
            }
            for(int j = 0; j< in/2+1 ; j++){
                new1->students[j] = Copy(this->students[j]);
				new1->BNum[j] = this->BNum[j];
                new1->in++;
				this->BNum[j] = 0;
                //this->students[j] = 0;
            }
            new1->nextleaf = new2;
            this->students[0] = new1->students[new1->in-1];
            this->childnode[0] = new1;
            this->childnode[1] = new2;
            this->leaf = false;
            this->in = 1;
            new1->parentnode = this;
            new2->parentnode = this;
        }
        else{ //When this node is not a root.
            BPNode* new1 = new BPNode();
			new1 = this->Copy();
			int k;
			//childtem = new BPNode();
			//childtem = 
            for(k = this->in/2+1; k<this->in; k++){
                new1->students[k] = this->students[k];
				new1->BNum[k] = this->BNum[k];
                new1->in++;
                //this->students[k] =0;
            }
            this->in = (in+1)/2;
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
            //insert parent¿¡ scores[in/2]
            //childnode Á¤¸®

        }


    }
    bool isEmpty(){
        return (in == 0);}
    bool isLeaf(){
        return leaf;}
    void Insert(Students stu, int Bnum){
        int i = 0;
        BPNode* tmp;
        tmp = this;
        if(tmp->in == 0){
			cout << stu.studentID << Bnum << endl;
            tmp->students[0] = Copy(stu);
			tmp->BNum[0] = Bnum;
			tmp->in ++;
        }
        else{
            while(!tmp->isLeaf()){
				
                for(i = 0; i< tmp->in; i++){
                    if(stu.score< tmp->students[0].score){
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
            tmp->students[tmp->in] = stu;
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
    void Print(ofstream& of){
        BPNode* tmp = this;
        while(!tmp->isLeaf()){
            tmp = tmp->childnode[0];
        }
        while(tmp!= NULL){
            for(int i = 0; i<tmp->in; i++){
				of << tmp->students[i].score << ", " << tmp->students[i].studentID << "," << tmp->BNum[i] <<endl;
            }
            tmp = tmp->nextleaf;
			of << "-----------------------------------------------------------------------" << endl;
        }
    }
    void Print(int k){
        BPNode* tmp = this;
        while(!tmp->isLeaf()){
            tmp = tmp->childnode[0];
        }
        int i = k;
        while(i != 1){
            tmp = tmp->nextleaf;
			if(tmp == NULL){
                cout << k <<" is too big"<< endl;
                break;
			}
            if( i <0 || i == 0){
                cout << k <<" is too small"<< endl;
                break;
            }
			i--;
        }
		if( i == 1 && tmp!= NULL){
			for(int j =0; j<tmp->in;j++){
				cout << tmp->students[i].score << ", " << tmp->students[i].studentID << "," << tmp->BNum[i] <<endl;
			}
		}
    }
	Students Copy(Students &a){
		Students b;
		strcpy(b.name, a.name);
		b.studentID = a.studentID;
		b.score = a.score;
		b.advisorID = a.advisorID;
		return b;
	}
};


#endif // BPTREE_H_INCLUDED

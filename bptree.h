#ifndef BPTREE_H_INCLUDED
#define BPTREE_H_INCLUDED
using namespace std;
#include "student.h"


class BPNode{
friend class BPTree;
private:
    BPNode* nextleaf;
    BPNode** childnode;
    float* scores;
    bool leaf;
    int in;
    int n;
public:
    BPNode(int i){
        nextleaf = NULL;
        childnode = NULL;
        parentnode = NULL;
        scores = NULL;
        leaf = true;
        n = i;
        in =0;
    }
    void Sort(){
        float temp;
        for (int i =0; i<in; i++){
            for (int j =0; j < in; j++){
                if (scores[i] > scores[j]){
                    temp = scores[i];
                    scores[i] = scores[j];
                    scores[j] = temp;
                }
            }
        }
    }
    bool isOver(){
        return (n < in);
    }
    void Split(int i){
        BPNode* new1 = new BPNode(this->n);
        BPNode* tmp;
        if( i == 0){ //When this node is a root.
            BPNode* new2 = new BPNode(this->n);
            for(int k = this->in/2+1; k<in; k++){
                new2->scores[k] = this->scores[k];
                new2->in++;
                this->scores[k] = 0;
            }
            for(int j = 0; j< in/2+1 ; j++){
                new1->scores[j] = this->scores[j];
                new1->in++;
                this->scores[j] = 0;
            }
            new1->nextleaf = new2;
            this->scores[0] = new1->scores[new1->in-1];
            this->childnode[0] = new1;
            this->childnode[1] = new2;
            this->leaf = false;
            this->in = 1;
            new1->parentnode = this;
            new2->parentnode=this;
        }
        else{ //When this node is not a root.
            for(int k = this->in/2+1; k<in; k++){
                new1->scores[k] = this->scores[k];
                new1->in++;
                this->scores[k] =0;
            }
            new1->leafnode = this->leafnode;

        }


    }
    bool isEmpty(){
        return (in == 0);}
    bool isLeaf(){
        return leaf;}
};

class BPTree{
private:
    BPNode* root;
    int n;
public:
    BPTree(int i){
        n = i;
        this->root = NULL;}
    void Insert(Student s){
        BPNode* tmp;
        tmp = this->root;
        if(tmp->isEmpty()){
            tmp->scores[0] = s.getscore();
        }
        else{
            if(tmp->isLeaf()){
                tmp->scores[tmp->in]=s.getscore();
                if (tmp->isOver()){
                    tmp->Split(0);
                }
            }
            else{
                while()
                tmp = this->root->childnode[0];

            }

        }


        tmp->in++;
    }
    void LeafPrint(&ofstream of){
    }
    void Print(int k){
        BPNode* a;
        a = root;
        while(!a->isLeaf()){
            a = a->childnode[0];
        }
        int i = k;
        while(i != 1){
            a = a->nextleaf;
            if( i <0 || i == 0){
                cout << k <<" is too much big number"<< endl;
                break;
            }
        }
        for(int j =0; j<a->in;j++){
            cout << a->scores[j] <<" ";
        }
        cout<<endl;
    }
};





#endif // BPTREE_H_INCLUDED

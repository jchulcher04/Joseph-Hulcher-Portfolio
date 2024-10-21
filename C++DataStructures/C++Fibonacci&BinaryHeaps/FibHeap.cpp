using namespace std;
#include <iostream>
#include <cstdlib>
#include <cmath>
#import "CircularDynamicArray.cpp" 

template <typename keytype> struct FibHeapNode {
    keytype key;
    bool marked; //for decreasing keys
    FibHeapNode<keytype>* parent; //Parent node
    FibHeapNode<keytype>* right; //right sibling
    FibHeapNode<keytype>* left; //left sibling
    
    FibHeapNode<keytype>* child; //a child
    int rank;
};


template <typename keytype> class FibHeap {
private:
    FibHeapNode<keytype>* min;
    FibHeapNode<keytype>* front;
    FibHeapNode<keytype>* end;
    int size, trees;

public:
    FibHeap(){
        size = 0;
        min = nullptr;
        front = nullptr;
        end = nullptr;
        trees = 0;
    };

    FibHeap(keytype k[], int s, CircularDynamicArray <FibHeapNode<keytype>* >& handle){
        size = 0;
        min = nullptr;
        front = nullptr;
        end = nullptr;
        trees = 0;
        for(int i = 0; i < s; i++){
            handle.addEnd(this->insert(k[i]));
        }
        this->consolidate();
    }
    
    ~FibHeap(){ //checks front before destroying
        if(front == nullptr){ //check if its empty
            return;
        }
        FibHeapNode<keytype>* toDestroy = front;
        do {
            FibHeapNode<keytype>* children = toDestroy->child;
            for(int i = 0; i < toDestroy->rank; i++){ //calls on each nodes children print helper, which returns the pointer to the next one
                children = FibTreeDestroyer(children);
            }
            FibHeapNode<keytype>* nowDestroy = toDestroy;
            toDestroy = toDestroy->right;
            delete nowDestroy;
        } while (toDestroy != front);
    }
    FibHeapNode<keytype>* FibTreeDestroyer(FibHeapNode<keytype>* toDestroy) {
        FibHeapNode<keytype>* children = toDestroy->child;
            for(int i = 0; i < toDestroy->rank; i++){ //calls on each nodes children print helper, which returns the pointer to the next one
                children = FibTreeDestroyer(children);
            }
            FibHeapNode<keytype>* nowDestroy = toDestroy;
            toDestroy = toDestroy->right;
            delete nowDestroy;
            return toDestroy;
    }
    /*
    FibHeap<keytype>& operator=(const FibHeap<keytype>& copyHeap){ //copy assignment operator
        if(size > 0) delete this;
        size = copyHeap.size;
        if(size = 0){ //if tree is empty
            front = nullptr;
            end = nullptr;
            min = nullptr;
            cout << "size is 0";
            return *this;
        }
        trees = copyHeap.trees;
        size = copyHeap.size;
        FibHeapNode<keytype>* toAdd = new FibHeapNode<keytype>; //create first node to be added
        FibHeapNode<keytype>* toCopy = copyHeap.front;
        FibHeapNode<keytype>* added;
        front = toAdd; // assign it as front
        cout << trees << " " << size << endl;
        for(int i = 0; i < trees; i++){
            cout << toCopy->key << endl;
            toAdd->key = toCopy->key; //copy, and go right until it gets back to the original node
            toAdd->marked = toCopy->marked;
            toAdd->rank = toCopy->rank;
            fibTreeCopier(toCopy, toAdd); //copy the sub tree;
            added = toAdd;
            toAdd = new FibHeapNode<keytype>;
            added->right = toAdd;
            toAdd->left = added;
            if(copyHeap.min == toCopy) min = added; //if it is the min, reassing as such.
            toCopy = toCopy->right;
            cout <<"!" << endl;
        } 
        
        end = added;
        delete toAdd;
        front->left = end;
        end->right = front;
        cout << "finish" << std::flush;
        return *this;
    }
    void fibTreeCopier(FibHeapNode<keytype>* toCopy, FibHeapNode<keytype>* toAdd){
        if(toCopy->rank == 0){
            toAdd->child = nullptr;
            return;
        }
        FibHeapNode<keytype>* children = new FibHeapNode<keytype>;
        FibHeapNode<keytype>* childrenToCopy = toAdd->child;
        FibHeapNode<keytype>* childAdded;
        toAdd->child = children;
        
        for(int i = 0; i < toCopy->rank; i++){
            cout << childrenToCopy->key << " " << std::flush;
            children->key = childrenToCopy->key;
            children->marked = childrenToCopy->marked;
            children->rank = childrenToCopy->rank;
            //fibTreeCopier(childrenToCopy, children);
            childAdded = children;
            children = new FibHeapNode<keytype>;
            children->left = childAdded;
            childAdded->right = children;
            toCopy = toCopy->right;
        }
        return;
        childAdded->right = toAdd->child;
        toAdd->child->left = childAdded;
        //delete children;
    }
    */

    FibHeap(const FibHeap<keytype>& toCopy){
        *this = toCopy;
    }

    keytype peekKey(){
        return min->key;
    }

    void consolidate(){
        int D = log2(size) + 1;
        CircularDynamicArray<FibHeapNode<keytype>*>  links(D + 1);       
        
        for(int i = 0; i < D + 1; i++) links[i] = nullptr; //set up list of recorded binomial trees
        FibHeapNode<keytype>* current = front;
        for(int i = 0; i < trees; i++){ //go through all the trees in the tree list
            FibHeapNode<keytype>* next = current->right; 
            int degree = current->rank;
            while(links[degree] != nullptr){ //while there is a heap recorded of size degree, comine it with current and move degree up one
                FibHeapNode<keytype>* toCombine = links[degree];
                if(current->key > toCombine->key) { // switch current and to combine if tocombine's key is less
                    FibHeapNode<keytype>* temp = current;
                    current = toCombine;
                    toCombine = temp;
                }
                linkNodes(current, toCombine); //link tocombine to current
                links[degree] = nullptr; //set the links list and iterate degree
                degree = degree + 1;
            }
            links[degree] = current; //put the final tree in the links list, move onto the next tree
            current = next;
        }
        /*
        for(int i = 0; i < D; i++){
            cout << links[i] << endl; //Testing
        }
        */ 
        front = nullptr;
        trees = 0;
        for(int i = 0; i < D; i++){ //remake the base list
            //cout << "."; test
            if(links[i] != nullptr){
                //cout << "! "; test
                trees++;
                if(front == nullptr){ // if it is the first to insert, put it as the only one
                    front = links[i];
                    front->right = front;
                    front->left = front;
                    end = front;
                } else { //otherwise, insert it at the end of the list
                    end->right = links[i];
                    links[i]->left = end;
                    end = end->right;
                    front->left = end;
                    end->right = front;
                }
            }
        }
        //cout << front << endl;   //testing
    }
    void linkNodes(FibHeapNode<keytype>* smaller, FibHeapNode<keytype>* larger){
        smaller->rank++;
        if(smaller->child == nullptr){ //puts larger as only child of smaller, links larger with itself
            smaller->child = larger;
            larger->parent = smaller;
            larger->left = larger;
            larger->right = larger;
            return;
        }
        larger->right = smaller->child; //insert larger to the left of Smaller's child
        larger->left = smaller->child->left;
        smaller->child->left->right = larger;
        smaller->child->left = larger;
        larger->parent = smaller;
        smaller->child = larger;
    }
    keytype extractMin(){
        if(min == nullptr) return NULL; // check if it is there are nodes
        keytype toReturn = min->key; //get the return value
        min->right->left = min->left; // fix the neighbors for removal
        min->left->right = min->right;
        if(min == end) end = min->left; //if min is the end or front, fix that
        if(min == front) front = min->right;
        trees--; //update size and trees
        size--;
        
        FibHeapNode<keytype>* toInsert = min->child; //now insert all the children
        if(trees == 0){ // the case that the heap is now empty, we first establish a node as the front
            front = toInsert;
            end = toInsert;
        }
        for(int i = 0; i < min->rank; i++){
            FibHeapNode<keytype>* next = toInsert->right;
            toInsert->right = front;
            toInsert->left = end;
            end->right = toInsert;
            front->left = toInsert;
            end = toInsert;
            toInsert->parent = nullptr;
            trees++;
            toInsert = next; 
        }
        
        if(size == 0){ //if no nodes left, fix the pointers
            min = nullptr;
            end = nullptr;
            front = nullptr;
        } else { //else consolidate and find the min for the future
            //this->printKey();
            this->consolidate();
            //cout << "hello";
            //return NULL;
            delete min;
            min = front;
            FibHeapNode<keytype>* toCheck = front->right;
            for(int i = 1; i < trees; i++){
                //cout << toCheck->rank << " ";
                if(toCheck->key < min->key) min = toCheck;
                toCheck = toCheck->right;

            }
        }
        return toReturn; // return the min key
    }


    FibHeapNode<keytype>* insert(keytype k){
        FibHeapNode<keytype>* toInsert = new FibHeapNode<keytype>;
        toInsert->rank = 0;
        toInsert->marked = 0;
        toInsert->key = k;
        toInsert->parent = nullptr;
        toInsert->child = nullptr;
        if(min == nullptr){
            min = toInsert;
            front = toInsert;
            end = toInsert;
            toInsert->right = toInsert;
            toInsert->left = toInsert;
        } else{ 
            toInsert->right = front;
            toInsert->left = end;
            end->right = toInsert;
            front->left = toInsert;
            if(toInsert->key < min->key){
                min = toInsert;
            }
            end = toInsert;
        }
        size++;  
        trees++;
        return toInsert;       
    }

    bool decreaseKey(FibHeapNode<keytype>* h, keytype k){
        if (k > h->key) return 0;
        h->key = k;
        if(h->parent != nullptr && h->parent->key > h->key){
            Cut(h);
        }
        if(k < min->key) min = h;
        return 1;
    }
    void Cut(FibHeapNode<keytype>* toCut){
        trees++;
        if(toCut->parent->rank == 1) toCut->parent->child = nullptr;
        else {
            if(toCut->parent->child == toCut) toCut->parent->child = toCut->right;
            toCut->right->left = toCut->left;
            toCut->left->right = toCut->right;
        }
        end->right = toCut;
        front->left = toCut;
        toCut->left = end;
        toCut->right = front;
        end = toCut;
        if(toCut->parent->marked == 1) Cut(toCut->parent);
        else if(toCut->parent->parent == nullptr) ;
        else toCut->parent->marked = 1;
        toCut->marked = 0;
        toCut->parent->rank--;
        toCut->parent = nullptr;
    }

    void merge(FibHeap<keytype> &H2){
        if(H2.front == nullptr) return;  
        size += H2.size;
        trees += H2.trees;
        if(front == nullptr){
            front = H2.front;
            end = H2.end;
            min = H2.min;
            H2.front = nullptr;

        }     
        if(H2.min < min) min = H2.min;
        H2.front->left = end;
        end->right = H2.front;
        front->left = H2.end;
        end = H2.end;
        end->right = front;
        H2.front = nullptr;
        
    }

    void printKey(){ //work in progress
        //cout << trees; //need to comment out
        if(front == nullptr){ //check if its empty
            //cout << " empty ";
            return;
        }
        FibHeapNode<keytype>* toPrint = front;
        do {
            cout << "Rank " << toPrint->rank << "\n"; //loop goes through all nodes in root list, makes first print
            cout << toPrint->key << " ";
                FibHeapNode<keytype>* children = toPrint->child; 
                for(int i = 0; i < toPrint->rank; i++){ //calls on each nodes children print helper, which returns the pointer to the next one
                    children = printKeyHelper(children);
                }
            cout << endl << endl;
            toPrint = toPrint->right;
        } while (toPrint != front);
    }
    FibHeapNode<keytype>* printKeyHelper(FibHeapNode<keytype>* toPrint){ //does the same thing as the loop, but for the children, recusively
        cout << toPrint->key << " ";
            FibHeapNode<keytype>* children = toPrint->child;
            for(int i= 0; i < toPrint->rank; i++){
                children = printKeyHelper(children);
            }
        return toPrint->right;
    }



};
/*

int main() {
    FibHeap<int> a;
    FibHeap<int> b;
    for(int i = 0; i < 10; i++){
        a.insert(i);
    }
    for(int i = 10; i < 30; i++){
        b.insert(i);
    }
    FibHeapNode<int>* test = b.insert(67);
    test->right = test->right;
    FibHeap<int> x;
    for(int i = 0; i < 100000; i++){
        x.insert(i);
    }
    for(int i = 0; i < 99993; i++) x.extractMin();
    
    x.printKey();
    FibHeapNode<int>* toDecrease = x.insert(10000000);
    x.consolidate();
    x.decreaseKey(toDecrease, 5);
    x.printKey();

    int k[100];
    for(int i = 0; i < 100; i++){
        k[i] = i + 100;
    }
    CircularDynamicArray<FibHeapNode<int>*> toHold; 
    FibHeap<int> y(k, 100, toHold);
    y.printKey();
    y.decreaseKey(toHold[4], 2);
    y.printKey();

    //c = a;
    //b.merge(a);
    //b.merge(c);
    
    //b.consolidate();
    //b.printKey();
    //cout << endl << b.peekKey() << endl;

    //b.consolidate();
    //c.printKey();
    //c = b;
    /*
    for(int i = 0; i < 17; i++){
        cout << b.peekKey() << " " << b.extractMin() << endl;
        b.printKey();
    }
    */
    /*
    for(int i = 0; i < 31; i++){
        cout << c.peekKey() << " " << c.extractMin() << endl;
    }
    c.printKey();
    
    
    return 0;
}
*/
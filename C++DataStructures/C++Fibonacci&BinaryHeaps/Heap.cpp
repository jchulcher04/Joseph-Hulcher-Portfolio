using namespace std;
#include <iostream>
#include <cstdlib>
#import "CircularDynamicArray.cpp" 

template <typename keytype> class Heap { //modified// 
private:
   int size;
   CircularDynamicArray<keytype> theArray;

public:
    Heap(){
        size = 0;
    }
    Heap(keytype k[], int s){
        size = s;
        for(int i = 0; i < s; i++){
            theArray.addEnd(k[i]);
        }
        for(int i = (size-1) / 2; i >= 0; i--){
            sinkDown(i);
        }
    }
    Heap(const Heap<keytype> &baseHeap){
        size = baseHeap.size;
        theArray = baseHeap.theArray;
    }
    Heap<keytype>& operator=(const Heap<keytype>& baseHeap){
        size = baseHeap.size;
        theArray = baseHeap.theArray;
        return *this;
    }
    keytype peekKey(){
        return theArray[0];
    }
    void sinkDown(int place){
        int curr = place;
        while(true){
            if(curr*2+2 < size && theArray[curr*2 + 2] < theArray[curr *2 + 1] && theArray[curr * 2 + 2] < theArray[curr]){
                keytype temp = theArray[curr];
                theArray[curr] = theArray[curr *2 + 2];
                theArray[curr * 2 + 2] = temp;
                curr = curr * 2 + 2;
                continue;
            }
            if(curr * 2 + 1 < size && theArray[curr * 2 + 1] < theArray[curr]){
                keytype temp = theArray[curr];
                theArray[curr] = theArray[curr *2 + 1];
                theArray[curr * 2 + 1] = temp;
                curr = curr * 2 + 1;
                continue;
            }
            break;
        }
    }
    keytype extractMin(){
    if(size == 0 ){
        return NULL;
    }
    int returnValue = theArray[0];
    theArray[0] = theArray[size - 1];
    sinkDown(0);
    size--;
    theArray.delEnd();
    return returnValue;
    }
    
    void insert(keytype k){
        size++;
        theArray.addEnd(k);
        int curr = size -1;
        while(theArray[curr] < theArray[(curr-1)/ 2]){
            keytype parent = theArray[(curr-1)/ 2];
            theArray[(curr-1)/ 2] = theArray[curr];
            theArray[curr] = parent;
            curr = (curr-1) / 2;
        }
    }
    void printKey(){
        theArray.print();
    }
};
/*
int main(){
    Heap<int> h;
    for(int i = 20; i > 0; i--){
        h.insert(i);
    }
    h.printKey();
    cout << h.peekKey() << endl;
    for(int i = 0; i < 20; i++){
        cout << h.extractMin() << " ";
    }
    
    int array[40];
    for(int i = 0; i < 40; i++){
        array[i] = 40 - i;
    }
    
    Heap<int> j(array, 40);
    cout << endl;
    j.printKey();
    cout << j.peekKey() << endl;
    Heap<int> h2(j);
    Heap<int> h3 = j;
    h3 = j;

    for(int i = 0; i < 40; i++){
        cout << j.extractMin() << " ";
    }
    cout << endl;
    for(int i = 0; i < 40; i++){
        cout << h2.extractMin() << " ";
    }
    cout << endl;
    for(int i = 0; i < 40; i++){
        cout << h3.extractMin() << " ";
    }


}*/
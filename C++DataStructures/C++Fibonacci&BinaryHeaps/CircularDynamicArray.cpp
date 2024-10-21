using namespace std;
#include <iostream>
#include <cstdlib>
  


template <typename T> class CircularDynamicArray {
private: //capacityVar is size of array, size is number of elements, front is the number added to the array to get to the indecy of the front, end the same
    T* theArray;
    int size, capacityVar, front, end;
  
public: //functions listed in order shown here
    CircularDynamicArray(); 
    CircularDynamicArray(int s);
    void clear();
    CircularDynamicArray(const CircularDynamicArray<T> &baseArray);
    ~CircularDynamicArray();
    CircularDynamicArray<T>& operator=(const CircularDynamicArray<T>& baseArray);
    

    void addEnd(T v);
    void addFront(T v);
    void delEnd();
    void delFront();

    int length();
    int capacity();
    T& operator[](int i);
    
    void print();

    T QuickSelect(int k);
    T WCSelect(int k);
    //T PivotFinder();

    void stableSort();
    int linearSearch(T e);
    int binSearch(T e);
};

template <typename T> T PivotFinder(T* arr, int length){ //finds the pivot with algorithim 3, breaking it into groups, with the extra in an extra group
    int i, key, j; 
    if(length <= 10){
        for (i = 1 ; i < length; i++)
        {
            key = arr[i]; 
            j = i - 1; 
            while (j >= 0 && arr[j] > key)
            { 
                arr[j + 1] = arr[j]; 
                j = j - 1; 
            } 
            arr[j + 1] = key; 
        }
        T f = arr[0];
        delete [] arr;
        return f;
    }
    for(int a = 0; a < (length) / 5; a++) {
        for (i = 1 + 5*a; i < 5 + 5*a; i++)
        {
            key = arr[i]; 
            j = i - 1; 
            while (j >= a * 5 && arr[j] > key)
            { 
                arr[j + 1] = arr[j]; 
                j = j - 1; 
            } 
            arr[j + 1] = key; 
        }
    }
    T* medians = new T[length / 5 + 1];
    
    for(i = 0; i < length / 5; i++){
        medians[i] = arr[i * 5 + 2];
    }
    if(length % 5 == 0){
        delete [] arr;
        return PivotFinder(medians, length / 5);
    }  
    for (i = (length / 5)*5; i < length; i++)
        {
            key = arr[i]; 
            j = i - 1; 
            while (j >= (length / 5)*5 && arr[j] > key)
            { 
                arr[j + 1] = arr[j]; 
                j = j - 1; 
            } 
            arr[j + 1] = key; 
    }
    medians[length / 5] = arr[(length + 5*(length / 5)) / 2];
    delete [] arr;
    return PivotFinder(medians, length / 5 + 1); 
}
  
template <typename T> CircularDynamicArray<T>::CircularDynamicArray(){ 
    capacityVar = 2;
    size = 0;
    end = 0; 
    front = 0;
    theArray = new T[2];
}
template <typename T> CircularDynamicArray<T>::CircularDynamicArray(int s){ //creates array of size s
    capacityVar = s;
    size = s;
    front = 0;
    end = s;
    theArray = new T[s];
}
template <typename T> void CircularDynamicArray<T>::clear(){
    delete [] theArray;
    capacityVar = 2;
    size = 0;
    end = 0; 
    front = 0;
    theArray = new T[2];
}
template <typename T> CircularDynamicArray<T>::CircularDynamicArray(const CircularDynamicArray<T> &baseArray){
    capacityVar = baseArray.capacityVar;
    size = baseArray.size;
    front = baseArray.front;
    end = baseArray.end;
    theArray = new  T[capacityVar];
    for(int i = 0; i <= size; i++ ){
        theArray[(i + front) % capacityVar] = baseArray.theArray[(i + front) % capacityVar];
    }
}
template <typename T> CircularDynamicArray<T>::~CircularDynamicArray(){
    delete [] theArray;
    
}
template <typename T> CircularDynamicArray<T>& CircularDynamicArray<T>::operator=(const CircularDynamicArray<T>& baseArray){ // takes array and makes a deep copy
    capacityVar = baseArray.capacityVar;
    size = baseArray.size;
    front = baseArray.front;
    end = baseArray.end;
    delete [] theArray;
    theArray = new  T[capacityVar];
    for(int i = 0; i <= size; i++ ){
        theArray[(i + front) % capacityVar] = baseArray.theArray[(i + front) % capacityVar];
    }
    
}

template <typename T> void CircularDynamicArray<T>::addEnd(T v){ // Add element to end, double the size when neccesarry
    size++;
    if(size > capacityVar){
        capacityVar = capacityVar * 2;
        T* newArray = new T[capacityVar];
        //cout << front << " " << end << " " << capacityVar;
        for(int i = 0; i < size; ++i){
            newArray[i] = theArray[(i + front) % (capacityVar / 2)];
        }
        delete [] theArray;
        theArray = newArray;
        front = 0;
        end = size - 1;
    }
    theArray[end] = v;
    end = (end + 1) % (capacityVar);
}
template <typename T> void CircularDynamicArray<T>::addFront(T v){ // Add element to end, double the size when neccesarry, circles around the front
    size++;
    if(size > capacityVar){
        capacityVar = capacityVar * 2;
        T* newArray = new T[capacityVar];
        //cout << front << " " << end << " " << capacityVar;
        for(int i = 0; i < size; ++i){
            //cout << (i + front) % capacityVar;
            newArray[i] = theArray[(i + front) % (capacityVar / 2)];
        }
        delete [] theArray;
        theArray = newArray;
        front = 0;
        end = size - 1;
    }
    if(front == 0){
        front = capacityVar - 1;
    }
    else{
        front = (front - 1);
    }
    theArray[front] = v;
    //cout << front;
}
template <typename T> void CircularDynamicArray<T>::delEnd(){ // Seems to work, but the delFront copies an interger wrong, So I'm unsure
    if(size > 0){
        size--;
        if(size * 4 <= capacityVar){
            capacityVar = capacityVar / 2;
            T* newArray = new T[capacityVar];
            //cout << front << " " << end << " " << capacityVar;
            for(int i = 0; i < size + 1; ++i){
                //cout << (i + front) % capacityVar;
                newArray[i] = theArray[(i + front) % (capacityVar * 2)];
            }
            delete [] theArray;
            theArray = newArray;
            front = 0;
            end = size - 1;
        }
        if(end == 0){
            end = capacityVar - 1;
        }
        else{
            end--;
        }
    }
    //cout << front;
}
template <typename T> void CircularDynamicArray<T>::delFront(){ // Seems to copie and element wrong
    if(size > 0){
        size--;
        if(size * 4 <= capacityVar){
            int tempCap = capacityVar;
            capacityVar = capacityVar / 2;
            T* newArray = new T[capacityVar];
            //cout << front << " " << end << " " << capacityVar;
            for(int i = 0; i < size  + 1; ++i){
                //cout << (i + front) % capacityVar;
                newArray[i] = theArray[(i + front) % (tempCap)];
            }
            delete [] theArray;
            theArray = newArray;
            front = 0;
            end = size - 1;
        }
        front = (front + 1) % capacityVar;
    }
    //cout << front;
}

template <typename T> int CircularDynamicArray<T>::length(){ // returns number of elements in array
    return size;
}
template <typename T> int CircularDynamicArray<T>::capacity(){ // returns array max size
    return capacityVar;
}
template <typename T> T& CircularDynamicArray<T>::operator[](int i){ // returns element at indecy in the circular array
    if(i < 0 || i >= size){
        cout << "error, requested indecy is out of bounds. ";
        return theArray[0];
    }
    return (theArray[(front + i) % capacityVar]);
}

template <typename T> void CircularDynamicArray<T>::print() // prints the array
{
    for (int i = 0; i < size; i++)
        cout << theArray[(i + front) % capacityVar] << " ";
    cout << endl;

}

template <typename T> T CircularDynamicArray<T>::QuickSelect(int k){ // A straight foward implementation of the Quicksort algorithim in the divide and conquor slides
    //cout << "1" << endl;
    T pivot = theArray[((front + (rand() % size)) % capacityVar)];
    //cout << "2" << endl;
    CircularDynamicArray<T> L;
    CircularDynamicArray<T> E;
    CircularDynamicArray<T> G;
    //cout << "3" << endl;
    for(int i = 0; i < size; i++){
        if(theArray[(i + front) % capacityVar] < pivot){
            L.addEnd(theArray[(i + front) % capacityVar]);
        } else if (theArray[(i + front) % capacityVar] == pivot){
            E.addEnd(theArray[(i + front) % capacityVar]);
        } else {
            G.addEnd(theArray[(i + front) % capacityVar]);
        } 
    }
        //L.addEnd(10);
        //cout << L.length() << " " << E.length() << " " << G.length() << " " << size << endl;          //extra information line
        if(k <= L.length()){
            return L.QuickSelect(k);
        } else if (k <= (L.length() + E.length())) {
            return pivot;
        } else {
            return G.QuickSelect(k - L.length() - E.length());
        }
        
    } 
template <typename T> T CircularDynamicArray<T>::WCSelect(int k){ //Essentially the same as QuickSelect, except while size > 25 when it calls quickselect, it uses Pivot finder function to find the pivot
    if(size > 25){
        //cout << "1" << endl;
        T* copyArray = new T[size];
        //cout << "2" << endl;
        for(int i = 0; i < size; i++){
            copyArray[i] = theArray[(i + front) % capacityVar];
        }
        //cout << "3" << endl;
        T pivot = PivotFinder(copyArray, size);
        //delete [] copyArray;
        //cout << "4" << endl;
        CircularDynamicArray<T> L;
        CircularDynamicArray<T> E;
        CircularDynamicArray<T> G;
        for(int i = 0; i < size; i++){
            if(theArray[(i + front) % capacityVar] < pivot){
                L.addEnd(theArray[(i + front) % capacityVar]);
            } else if (theArray[(i + front) % capacityVar] == pivot){
                E.addEnd(theArray[(i + front) % capacityVar]);
            } else {
                G.addEnd(theArray[(i + front) % capacityVar]);
            } 
        }
            //L.addEnd(10);
            //cout << L.length() << " " << E.length() << " " << G.length() << " " << size << endl;          //extra information line
            if(k <= L.length()){
                return L.WCSelect(k);
            } else if (k <= (L.length() + E.length())) {
                return pivot;
            } else {
                return G.WCSelect(k - L.length() - E.length());
            }
        
    }   else {
        return this->QuickSelect(k);
    }

    
}

template <typename T> void CircularDynamicArray<T>::stableSort(){ //Uses merge sort by breaking it into two more circular arrays L and R to be called recursily before merging at the top level, I made the algorithim myself to practice Mergesort implementation
        if(size > 1){
            CircularDynamicArray<T> L;
            for(int i = 0; i < (size / 2); i++){
                L.addEnd(theArray[(front + i) % capacityVar]);
            }
            CircularDynamicArray<T> R;
            for(int i = (size / 2); i < size; i++){
                R.addEnd(theArray[(front + i) % capacityVar]);
            }
            //L.print(); //
            //R.print();
            if(size > 2){
                L.stableSort();
                R.stableSort();
            }
            int LIndex = 0;
            int RIndex = 0;
            
            for(int i = 0; i < size; i++){
                if(LIndex >= L.length() || (RIndex < R.length() && R[RIndex] < L[LIndex])){
                    theArray[(i + front) % capacityVar] = R[RIndex];
                    RIndex++;
                } else if(LIndex < L.length()){
                    theArray[(i + front) % capacityVar] = L[LIndex];
                    LIndex++;
                } else{
                    theArray[(i + front) % capacityVar] = R[RIndex];
                    RIndex++;
                }
            }
        }
        return;
}
template <typename T> int CircularDynamicArray<T>::linearSearch(T e){
    for(int i = 0; i < size; i++){
        if(theArray[(front + i) % capacityVar] == e){
            return i;
        }
    }
    return -1;
}
template <typename T> int CircularDynamicArray<T>::binSearch(T e){ //Does not use reccusion, instead holding indicies Bottom and Top and making them closer steadily, for effeciency and ease of implementation
    int bottom = 0;
    int top = size;
    int pivot;
    while(top - bottom >= 0){
        pivot = (top + bottom) / 2;
        if(e > theArray[(pivot + front) % capacityVar]){
            bottom = pivot + 1;
        } else if (e < theArray[(pivot + front) % capacityVar]) {
            top = pivot - 1;
        } else{
            return pivot;
        }
    }
    if(theArray[(pivot + front) % capacityVar] == e){
        return pivot;
    }
    return -1;
}


/* //bunch of testing stuff
int main()
{
    CircularDynamicArray<float> b(1000);
    for(int i = 0; i < 1000; i++){
        b[i] = (rand() % 10000);
    }
    b.stableSort();
    for(int i = 0; i < 10; i ++){
        cout << b[i + 989] << " ";
    }
    //b.stableSort();
    cout << b.binSearch(9929);
}
    /*
    cout << b.QuickSelect(1) << endl;
    cout << b.WCSelect(1) << endl;
    cout << b.QuickSelect(100000) << endl;
    cout << b.WCSelect(100000) << endl;
    
    b.stableSort();

    cout << b.linearSearch(108451) << endl;
    cout << b.linearSearch(108452) << endl;
    cout << b.binSearch(108451) << endl;
    cout << b.binSearch(108452) << endl;
    cout << endl;
    for(int i = 0; i < 10; i ++){
        cout << b[i] << " ";
    }
    cout << b[99999];
    

}
    /*
    
      cout << endl;
    CircularDynamicArray<float> c = b;
    for(int i = 0; i < 100; i ++){
        cout << c[i] << " ";
    }
    for(int i = 0; i < 99; i ++){
        c.DelEnd();
    }
    
    cout << endl;
    //c.AddFront(1);
    c.print();
    cout << c.QuickSelect(0);
    */


//}
    /*
    CircularDynamicArray<int> b;
    b.addEnd(10);
    b.addEnd(7);
    b.addEnd(6);
    b.addEnd(5);
    b.addEnd(4);
    
    b.AddFront(11);
    b.print();
    /*
    for(int i = 0; i < 200; i++){
        b.AddFront(rand() % 1000);
    }
    
    for(int i = 0; i < 20; i++){
        b.addEnd(0-i);
    }
    b.AddFront(100);
    b.print();
    cout << b[3] << endl;
    CircularDynamicArray<int> c;
    c = b;
    CircularDynamicArray<int> d(b);
    b.DelEnd();
    //b.print();
    b.stableSort();
    b.print();
    /*
    //c.print();
    //d.print();
    cout << b.QuickSelect(5) << " " << c.linearSearch(4) << " " << c.linearSearch(1000) << endl;
    b.stableSort();
    b.print();
    cout << b.binSearch(-17) << " " << b.binSearch(-10) << " " << b.binSearch(2);
    * /
    cout << b.WCSelect(200) << " " << b.QuickSelect(200);


    /*
    for(int i = 0; i < 40; i++){
        b.DelEnd();
    }
    b.print();

    cout << b[2] << " " << b[4] << endl;
    b.clear();
    b.addEnd(1);
    b.print();
    * /

    return 0;
     
}
 */
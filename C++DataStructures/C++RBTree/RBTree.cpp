using namespace std;
#include <iostream>
#include <cstdlib>

template <typename keytype, typename valuetype> struct treeNode {
    valuetype value;
    keytype key;
    treeNode* l;
    treeNode* r;
    treeNode* p;
    bool color; // 0 = black, 1 is red
    int leftNodes;
};

template <typename keytype, typename valuetype> class RBTree {
    private:
        treeNode<keytype, valuetype>* nil; //assign it here to makr life simple
        treeNode<keytype, valuetype>* root;
        int sizer;

    public:
        RBTree() { //  basic controctor, creates the sentinam nil, and assignes the root to it
            nil = new treeNode<keytype, valuetype>; 
            nil->color = 0;
            root = nil;
            sizer = 0;
            nil->leftNodes = 0;
        }
        RBTree(keytype K[], valuetype V[], int s){ // same construction method, then calls insert for all the items
            nil = new treeNode<keytype, valuetype>; 
            nil->color = 0;
            root = nil;
            sizer = 0;
            nil->leftNodes = 0;
            for(int i = 0; i < s; i++){
                insert(K[i], V[i]);
            }
        }
        ~RBTree(){ // calls recursive destroyer on the root, then gets rid of nil
            RBTreeDeleter(root);
            delete nil;
        }
        void RBTreeDeleter(treeNode<keytype, valuetype>* x){ // detroyes the two left and right, then itself, unless its nill
            if(x != nil){
                RBTreeDeleter(x->l);
                RBTreeDeleter(x->r);
                delete x;
            }
            
        }
        RBTree(const RBTree<keytype, valuetype> &toCopy){ //copies the base information, then hanldes the nodes by calling the node copier  
            nil = new treeNode<keytype, valuetype>; 
            nil->color = 0;
            sizer = toCopy.sizer;
            nil->leftNodes = 0;
            root = RBTreeNodeCopier(toCopy.root, toCopy.nil, nil);
        }
        treeNode<keytype, valuetype>* RBTreeNodeCopier(treeNode<keytype, valuetype>* x, treeNode<keytype, valuetype>* xnil, treeNode<keytype, valuetype>* parent){ // returns the node it is copying, and takes the parent node, the copied tree's nil and the node to copy
            if(x != xnil){
                treeNode<keytype, valuetype>* y = new treeNode<keytype, valuetype>;
                y->value = x->value;
                y->key = x->key;
                y->p = parent;
                y->l = RBTreeNodeCopier(x->l, xnil, y); // recursive calls, itself and the parent, and the children of the copied node
                y->r = RBTreeNodeCopier(x->r, xnil, y);
                y->color = x->color;
                y->leftNodes = x->leftNodes;
                return y;
            }
            return nil;
        }
        RBTree<keytype, valuetype>& operator=(const RBTree<keytype, valuetype> &toCopy){ // destroyes old tree, then copies new one
            delete nil;
            nil = new treeNode<keytype, valuetype>; 
            RBTreeDeleter(root);
            sizer = toCopy.sizer;
            nil->color = 0;
            root = RBTreeNodeCopier(toCopy.root, toCopy.nil, nil);
            return *this;
        }

        valuetype * search(keytype k){  //basic search for the key, returns the value if found
            treeNode<keytype, valuetype>* x = root;
            while(x != nil){
                if(k < x->key)
                    x = x->l;
                else if (k > x->key){
                    x = x->r;
                }
                else
                    return &(x->value);
            }
            return NULL;
        }

        void leftRotate(treeNode<keytype, valuetype>* x){ //just implemented the algorithim in the textbooks
            treeNode<keytype, valuetype>* y = x->r;
            y->leftNodes = y->leftNodes + x->leftNodes + 1;
            x->r = y->l;
            if(y->l != nil)
                y->l->p = x;
            y->p = x->p;
            if(x->p == nil){
                root = y;
            }
            else if (x == x->p->l){
                x->p->l = y;
            } else {
                x->p->r = y;
            }
            y->l = x;
            x->p = y;

        }
        void rightRotate(treeNode<keytype, valuetype>* y){ //is the revers algorithim in the textbook, used a graph 
            treeNode<keytype, valuetype>* x = y->l; // x is y's left subtree
            y->leftNodes = y->leftNodes - x->leftNodes -1;
            y->l = x->r; //got x's right to be y's left
            if(x->r != nil)
                x->r->p = y; 
            x->p = y->p;
            if(y->p == nil){
                root = x;
            } else if(y == y->p->l){
                y->p->l = x;
            } else {
                y->p->r = x;
            }
            x->r = y;
            y->p = x;

        }
        void rbInsertFixup(treeNode<keytype, valuetype>* z){ // is the current node
            while(z->p->color == 1){
                if(z->p == z->p->p->l){ //check which node is z's parent
                    treeNode<keytype, valuetype>* y = z->p->p->r;
                    if(y->color == 1){ //case one, transfer red to grandpa
                        z->p->color = 0;
                        y->color  = 0;
                        z->p->p->color = 1;
                        z = z->p->p;
                    }
                    else{ // case two, shift so that you have left left instead of left right
                        if(z == z->p->r){
                            z = z->p;
                            leftRotate(z);
                        }
                        z->p->color = 0; //case three, shift to resolve the double red
                        z->p->p->color = 1;
                        rightRotate(z->p->p);
                    }
                } else{
                    treeNode<keytype, valuetype>* y = z->p->p->l; //these are a reflection on the oppisit side
                    if(y->color == 1){
                        z->p->color = 0;
                        y->color  = 0;
                        z->p->p->color = 1;
                        z = z->p->p;
                    }
                    else{
                        if(z == z->p->l){
                            z = z->p;
                            rightRotate(z);
                        }
                        z->p->color = 0;
                        z->p->p->color = 1;
                        leftRotate(z->p->p);
                    }
                }
            }
            root->color = 0; //set the root black
        }
        void insert(keytype k, valuetype v){ // fun stuff
            //cout << k << endl;
            sizer++;
            treeNode<keytype, valuetype>* x = root; // comparision node
            treeNode<keytype, valuetype>* y = nil; // comparision node's parent
            while(x != nil){ // finds where to insert the node
                y = x;
                if(k < x->key){
                    x->leftNodes++;
                    x = x->l;
                } else x = x->r;
            } // now that we got to the 
            treeNode<keytype, valuetype>* newNode = new treeNode<keytype, valuetype>; // make the node to insert
            newNode->value = v;
            newNode->key = k;
            newNode->p = y;
            if(y == nil) // insert it at the thing
                root = newNode;
            else if (k < y->key){
                y->l = newNode;
            } else {
                y->r = newNode;
            }
            newNode->l = nil;
            newNode->r = nil;
            newNode->color = 1;
            newNode->leftNodes = 0;
            //cout << "a " << endl;
            rbInsertFixup(newNode); // call the color fixer
        }


        int rank(keytype k){ // counts the left nodes as it goes along and adds one for the node itself
            treeNode<keytype, valuetype>* x = root;
            int rank = 1;
            while(x != nil){
                if(k < x->key){
                    x = x->l;
                } else if (k > x->key) {
                    rank = rank + x->leftNodes + 1;
                    x = x->r;
                } else{
                    return rank + x->leftNodes;
                }
            }
            return 0;
        }
        treeNode<keytype, valuetype>* positionFinder(int pos){ //helper function that returns the rank of the functions
            treeNode<keytype, valuetype>* x = root;
            while(x != nil){
                if(pos <= x->leftNodes){
                    x = x->l;
                }
                else if (pos > x->leftNodes + 1) {
                    pos = pos - x->leftNodes - 1;
                    x = x->r;
                } else{
                    return x;
                }
            }
            return nil;
        }
        keytype select(int pos){ // calls helper fucntion, returns key of the node found at the ranke
            treeNode<keytype, valuetype>* x = positionFinder(pos);
            return x->key;
        }
        treeNode<keytype, valuetype>& subTreeMin(treeNode<keytype, valuetype> x){ // i think i dont use this, but im not going to delete it just in case
            while(x->l != nil){
                x = x->l;
            }
            return x;
        }
        keytype *successor(keytype k){  //finds the node, then uses position finder to find the next
            int kRank = rank(k);
            if(kRank == 0){
                return NULL;
            }
            treeNode<keytype, valuetype>* x = positionFinder(kRank + 1);
            if(x == nil){
                return NULL;
            }
            return &(x->key);
        }

        keytype *predecessor(keytype k){ // same as successor, but the previous one
            int kRank = rank(k);
            if(kRank == 0){
                return NULL;
            }
            treeNode<keytype, valuetype>* x = positionFinder(kRank - 1);
            if(x == nil){
                return NULL;
            }
            return &(x->key);
        }

        void RBTrasplant(treeNode<keytype, valuetype>* u, treeNode<keytype, valuetype>* v){  //u is what is being replaced, v replaces that node
            if(u->p == nil){
                root = v;
            } else if (u == u->p->l){
                u->p->l = v;
            } else{
                u->p->r = v;
            }
            v->p = u->p;
        }

        int remove(keytype k){ //z is the node to remove, y is the preddessesr we replace it with, 
            sizer--;
            treeNode<keytype, valuetype>* z = root;
            treeNode<keytype, valuetype>* x;
            while(z != nil){ //go down to the node to be deleted, if its not found, this code ends and zero is returned
                if(z->key > k){
                    z = z->l;
                } else if(k > z->key){
                    z= z->r;
                } else{ //when node is found, delete algorithim activates
                    treeNode<keytype, valuetype>* y;
                    y = z;
                    bool yOriginalColor = y->color;
                    if(z->l == nil){ // if there is no left, replace by right, as there is no predessesor
                        x = z->r;
                        RBTrasplant(z, z->r);
                    }  else{ //we replace by predecessor
                        y = z->l;
                        while(y->r != nil){ //we find the succesor
                            y = y->r;
                        }
                        yOriginalColor = y->color; //store the predecessor's color
                        x = y->l;
                        if(y != z->l){ // replace predecessor by it's left child, there is no futher nodes, beuase of black length limit. 
                            RBTrasplant(y, y->l);
                            y->l = z->l;
                            y->l->p = y;
                        } else { 
                            x->p = y;
                            }
                        RBTrasplant(z, y); //move up the predecessor
                        y->r = z->r;
                        y->r->p = y;
                        y->color = z->color; 
                        y->leftNodes = z->leftNodes;
                        //z = nil; 
                    }
                    treeNode<keytype, valuetype>* xcopy = x; //update the left nodes going back up the tree now that we know which node we deleted, starting there each time we go left.
                    while(xcopy != root){
                        if( xcopy == xcopy->p->l && xcopy->p->leftNodes > 0)
                            xcopy->p->leftNodes--;
                        xcopy = xcopy->p;
                    }
                    
                    if (yOriginalColor == 0) //if the node we got rid of was black, we gots to fix the colors
                        RBDeleteFixup(x);
                    return 1;                   
                }
            }
            return 0;
        }
        void RBDeleteFixup(treeNode<keytype, valuetype>* x){ 
            treeNode<keytype, valuetype>* w;
            while(x != root && x->color == 0){ //we have an extra black, x we got to find a red node to put it on. if its the root, we end, black height decreces by one
                if(x == x->p->l){//x is a left child
                    w = x->p->r;
                    if(w->color == 1){ //if sibling is red, it has two black childre so we rotate the parent to move down the tree and work from there
                        w->color = 0;
                        x->p->color = 1;
                        leftRotate(x->p);
                        w = x->p->r;
                    }
                    if(w->l->color == 0 && w->r->color == 0){ // case two, we have a black sibling with black childre, we move the double black up and make the sibling red
                        w->color = 1;
                        x = x->p;
                    } else { // we have a red in the sibling children, we are going to use that
                        if(w->r->color == 0){ //case 3: we have to rotate the sibling to get it to case 4
                            w->l->color = 0;
                            w->color = 1;
                            rightRotate(w);
                            w = x->p->r;
                        }
                        w->color = x->p->color; // case 4: we move the red from the siblings sub tree over, then color black and spread out the double black, and end the loop
                        x->p->color = 0;
                        w->r->color = 0;
                        leftRotate(x->p);
                        x = root;
                    }
                }
                else { // a reflection where x is the right child
                    w = x->p->l;
                    if(w->color == 1){
                        w->color = 0;
                        x->p->color = 1;
                        rightRotate(x->p);
                        w = x->p->l;
                    }
                    if( w->r->color == 0 && w->l->color == 0){
                        w->color = 1;
                        x = x->p;
                    } else{
                        if(w->l->color == 0){
                            w->r->color= 0;
                            w->color = 1;
                            leftRotate(w);
                            w= x->p->l;
                        }
                        w->color = x->p->color;
                        x->p->color = 0;
                        w->l->color = 0;
                        rightRotate(x->p);
                        x= root;
                    }
                }
            }
            x->color = 0;
        }

        int size(){ //return the size variable
            return sizer;
        }

        void preorder() { // calls the preorder recursive function
            preOrderPrinter(root);
            cout << "\n";
        }
        void preOrderPrinter(treeNode<keytype, valuetype>* z){ //prints this key, then makes the recursive calls if its not nil
            if(z != nil){
                cout << z->key << " ";
                preOrderPrinter(z->l);
                preOrderPrinter(z->r);
            }
        }
        void inorder() {
            inOrderPrinter(root);
            cout << "\n";
        }
        void inOrderPrinter(treeNode<keytype, valuetype>* z){ // same as pre, but for inorder order
            if(z != nil){
                inOrderPrinter(z->l);
                cout << z->key << " ";
                inOrderPrinter(z->r);
            }
        }
        void postorder() {
            postOrderPrinter(root);
            cout << "\n";
        }
        void postOrderPrinter(treeNode<keytype, valuetype>* z){ // same
            if(z != nil){
                postOrderPrinter(z->l);
                postOrderPrinter(z->r);
                cout << z->key << " ";
            }
        }
        void printk(int k){
            printkHelper(k, root);
            cout << endl;
        }
        void printkHelper(int k, treeNode<keytype, valuetype>* z){ //this uses in order logic, but keeps track of the number of nodes to prints, and only prints the ones that have rank less thank K.
            if(z != nil){
                printkHelper(k, z->l);
                if(z->leftNodes < k ){
                    cout << z->key << " ";
                    printkHelper(k - z->leftNodes - 1, z->r);
                }
            }
        }

}; //

// my based tests, became unreadable
/*
int main() {
    RBTree<int, int> t1;
    for(int i = 0; i < 30; i++){
        t1.insert( i, i + 10);
    }
    t1.preorder();
    t1.inorder();
    t1.postorder();
    for(int i = 0; i < 10; i++){
        cout << t1.rank(i) << " ";
    }
    cout << endl;
    for(int i = 0; i < 10; i++){
        cout << t1.select(i + 1) << " ";
    }
    cout << endl;

    int a[10], b[10];
    for(int i = 0; i < 10; i++){
        a[i] = i + 10;
        b[i] = i + 20;
    }

    RBTree<int, int> t2(a, b, 10);
    for(int i = 0; i < 10; i++){
        cout << t2.select(i + 1) << " ";
    }
    cout << endl;
    cout << *(t1.successor(13)) << endl;
    t1.successor(14);
    cout << *(t1.successor(11)) << endl;
    cout << *(t1.successor(7)) << endl;
    cout << *(t1.successor(15)) << endl;

    cout << *(t1.predecessor(13)) << endl;
    t1.predecessor(10);
    cout << *(t1.predecessor(15)) << endl;
    cout << *(t1.predecessor(17)) << endl;
    cout << *(t1.predecessor(19)) << endl;

    RBTree<int, int> t3(t1);
    
    t3.preorder();
    t3.inorder();
    t3.postorder();
    RBTree<int, int> t4;
    t4 = t3;
    t4.preorder();
    t4.inorder();
    t4.postorder();
    for(int i = 0; i < 30; i++){
        cout << *(t4.search(i)) << " ";
    }
    cout << "hi" << endl;
    cout << endl;
    
    for(int i = 10; i < 20; i++){
        t4.remove(i);
    }
    
    t4.remove(6);
    t4.preorder();
    t4.inorder();
    t4.postorder();
    t3.printk(12);t3.printk(1);t3.printk(24);t3.printk(32);
    

}
*/
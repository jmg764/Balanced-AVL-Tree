//
//  main.cpp
//  jmg764_hw11
//
//  Created by Jonathan Glaser on 7/2/18.
//  Copyright © 2018 Jonathan Glaser. All rights reserved.
//

#include <iostream>
#include <stack>
#include <queue>
#include <ctime>


using namespace std;
template <class T>
class BST;

template <class T>
class BSTNode{
    BSTNode<T>* left;
    BSTNode<T>* right;
    BSTNode<T>* parent;
    T data;
public:
    BSTNode(T newdata = T(), BSTNode<T>* newparent = nullptr, BSTNode<T>* newleft = nullptr, BSTNode<T>* newright = nullptr){
        data = newdata; parent = newparent; left = newleft; right = newright;
    }
    friend class BST < T > ;
};

template <class T>
class BST{
    BSTNode<T>* root;
    int countOfNodes;
    BSTNode<T>* recursiveCopy(const BSTNode<T>* rhs);
    void printInOrder(BSTNode<T>* ptr);
public:
    BST() :root(nullptr){}
    BST(const BST<T>& rhs) :root(nullptr){ *this = rhs; }
    ~BST(){ clear(); }
    bool isEmpty(){ return root == nullptr; }
    void remove(const T& val){ remove(find(val)); }
    bool findInTree(const T& val){ return find(val) != nullptr; }
    void clear(){ while (!isEmpty()) remove(root); }
    int size(){ return countOfNodes; }
    
    BST<T>& operator=(const BST<T>& rhs);
    void insert(const T&);
    void remove(BSTNode<T>* ptr);
    BSTNode<T>* find(const T&);
    void printInOrder(){ if (root!=nullptr) printInOrder(root); }
    
    //AVL Tree Functions
    int height(BSTNode<T>* ptr);
    int difference(BSTNode<T>* ptr);
    BSTNode<T>* rrRotation(BSTNode<T>* ptr);
    BSTNode<T>* llRotation(BSTNode<T>* ptr);
    BSTNode<T>* lrRotation(BSTNode<T>* ptr);
    BSTNode<T>* rlRotation(BSTNode<T>* ptr);
    BSTNode<T>* balance(BSTNode<T>* ptr);
    void printLevelOrder();
};

template <class T>
void BST<T>::printLevelOrder(){
    queue<BSTNode<T>*> q;
    q.push(root);
    while(!q.empty()){
        BSTNode<T>* temp = q.front();
        q.pop();
        cout << temp->data << ",";
        if (temp->left != nullptr) {
            q.push(temp->left);
        }
        if (temp->right != nullptr) {
            q.push(temp->right);
        }
    }
}

template <class T>
BSTNode<T>* BST<T>::balance(BSTNode<T> *ptr){
    int balanceFactor = difference(ptr);
    if (balanceFactor > 1) {
        if (difference(ptr->left) > 0) {
            ptr = llRotation(ptr);
        } else {
            ptr = lrRotation(ptr);
        }
    } else if (balanceFactor < -1){
        if (difference(ptr->right) > 0) {
            ptr = rlRotation(ptr);
        } else {
            ptr = rrRotation(ptr);
        }
    }
    return ptr;
}

template <class T>
BSTNode<T>* BST<T>::rlRotation(BSTNode<T> *ptr){
    BSTNode<T> *temp;
    temp = ptr->right;
    ptr->right = llRotation(temp);
    return rrRotation(ptr);
}

template <class T>
BSTNode<T>* BST<T>::lrRotation(BSTNode<T> *ptr){
    BSTNode<T> *temp;
    temp = ptr->left;
    ptr->left = rrRotation(temp);
    return llRotation(ptr);
}

template <class T>
BSTNode<T>* BST<T>::llRotation(BSTNode<T> *ptr){
    BSTNode<T>* temp;
    temp = ptr->left;
    ptr->left = temp->right;
    temp->right = ptr;
    return temp;
}

template <class T>
BSTNode<T>* BST<T>::rrRotation(BSTNode<T> *ptr){
    BSTNode<T>* temp;
    temp = ptr->right;
    ptr->right = temp->left;
    temp->left = ptr;
    return temp;
}

template <class T>
int BST<T>::difference(BSTNode<T>* ptr){
    int diff = 0;
    if (ptr != nullptr) {
        int leftHeight = height(ptr->left);
        int rightHeight = height(ptr->right);
        diff = leftHeight-rightHeight;
    }
    return diff;
}

template<class T>
int BST<T>::height(BSTNode<T>* ptr){
    int h = 0;
    if (ptr != nullptr) {
        int leftHeight = height(ptr->left);
        int rightHeight = height(ptr->right);
        int maxHeight = max(leftHeight, rightHeight);
        h = maxHeight + 1;
    }
    return h;
}

template<class T>
void BST<T>::printInOrder(BSTNode<T>* ptr){
    if (ptr->left != nullptr)
        printInOrder(ptr->left);
    cout << ptr->data<<endl;
    if (ptr->right != nullptr)
        printInOrder(ptr->right);
}

template <class T>
BSTNode<T>* BST<T>::find(const T& val){
    BSTNode<T>* temp = root;
    while (temp != nullptr && temp->data != val){
        if (val < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }
    return temp;
}

template <class T>
void BST<T>::remove(BSTNode<T>* ptr){
    if (ptr == nullptr) //someone asked me to remove a value that isnt in the tree... okay, done!
        return;
    if (countOfNodes == 1 && ptr == root){ //this is the last node
        countOfNodes--;
        root = nullptr;
        delete root;
        
    }
    else if (ptr->left == nullptr && ptr->right == nullptr){ //no children, delete and update parent
        BSTNode<T>* parent = ptr->parent;
        if (parent != nullptr){ //update the parent's child pointer
            if (ptr == parent->left)
                parent->left = nullptr;
            else
                parent->right = nullptr;
        }
        ptr = nullptr;
        delete ptr;
        countOfNodes--;
    }
    else if (ptr->left == nullptr) { //node has a right child
        BSTNode<T>* temp = ptr->right;
        BSTNode<T>* parent = ptr->parent;
        if (parent != nullptr){
            if (ptr == parent->left)
                parent->left = temp;
            else
                parent->right = temp;
        }
        else
            root = temp;
        temp->parent = parent;
        ptr = nullptr;
        delete ptr;
        countOfNodes--;
    }
    else if (ptr->right == nullptr) { //node has a left child
        BSTNode<T>* temp = ptr->left;
        BSTNode<T>* parent = ptr->parent;
        if (parent != nullptr){
            if (ptr == parent->right)
                parent->right = temp;
            else
                parent->left = temp;
        }
        else
            root = temp;
        temp->parent = parent;
        ptr = nullptr;
        delete ptr;
        countOfNodes--;
    }
    else{ //the node has two children!!! - Bad
        //Replace the data with the min of the right subtree
        BSTNode<T>* temp = ptr->right;
        while (temp->left != nullptr)
            temp = temp->left;
        ptr->data = temp->data;
        remove(temp); //Recursion at its finest....ahhh!
    }
}

template <class T>
void BST<T>::insert(const T& val){
    countOfNodes++;
    if (root == nullptr){//New node is the first on the tree
        root = new BSTNode<T>(val);
        return;
    }
    else{
        BSTNode<T>* prev=root;
        BSTNode<T>* temp=root;
        while (temp != nullptr){
            prev = temp;
            if (val < temp->data)
                temp = temp->left;
            else
                temp = temp->right;
        }
        //prev is a pointer to the node on which we will insert
        if (val < prev->data){ //val will be a left child of prev
            prev->left = new BSTNode<T>(val, prev);
            root = balance(root);
        }
        else
            prev->right = new BSTNode<T>(val, prev);
            root = balance(root);
    }
}

template <class T>
BSTNode<T>* BST<T>::recursiveCopy(const BSTNode<T>* rhs){
    if (rhs == nullptr)
        return nullptr;
    BSTNode<T>* temp = new BSTNode<T>(rhs->data);
    temp->left = recursiveCopy(rhs->left);
    temp->right = recursiveCopy(rhs->right);
    if (temp->left!=nullptr)
        temp->left->parent = temp;
    if (temp->right)
        temp->right->parent = temp;
    return temp;
}


template <class T>
BST<T>& BST<T>::operator=(const BST<T>& rhs){
    if (this == &rhs)
        return *this;
    clear();
    root = recursiveCopy(rhs.root);
    countOfNodes = rhs.countOfNodes;
    return *this;
}

int main(){
    BST <int> t1;
    srand(time(NULL));
    
    for (int i = 0; i < 100; i++){
        t1.insert(rand() % 1000);
    }
    
    BST<int> t2 = t1;
    
    for (int i = 0; i < 10; i++){
        t2.remove(rand() % 1000);
    }
    t2.printLevelOrder();
    
    return 0;
}




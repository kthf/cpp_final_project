#include "AVLTreeBase.h"

template <typename T>
AVLTreeBase<T>::AVLTreeBase(){
    root = nullptr;
    _size = 0;
}

template <typename T>
AVLTreeBase<T>::~AVLTreeBase(){
    clear();
}

template <typename T>
void AVLTreeBase<T>::clear(){
    std::vector<AVLTreeNode<T>*> stack;
    
    if (root != nullptr)
        stack.push_back(root);
    
    while (!stack.empty()){
        AVLTreeNode<T> *node = stack.back();
        stack.pop_back();
        
        if (node->left != nullptr)
            stack.push_back(node->left);
        
        if (node->right != nullptr)
            stack.push_back(node->right);
        
        _size--;
        delete node;
    }
    
    root = nullptr;
}

template <typename T>
void AVLTreeBase<T>::insert(T value){
    AVLTreeNode<T> **indirect = &root;  // to generalize insertion
    std::vector<AVLTreeNode<T>**> path;  // to update height values
    
    while (*indirect != nullptr){
        path.push_back(indirect);
        
        if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }
    
    *indirect = new AVLTreeNode<T>(value);
    path.push_back(indirect);
    
    balance(path);
    _size++;
}

template <typename T>
void AVLTreeBase<T>::erase(T value){
    AVLTreeNode<T> **indirect = &root;  // to generalize insertion
    std::vector<AVLTreeNode<T>**> path;  // to update height values
    
    while (*indirect != nullptr and (*indirect)->value != value){
        path.push_back(indirect);
        
        if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }
    
    if (*indirect == nullptr)  // the value does not exist in tree
        return;  // may be raising an Exception is more elegant
    
    else
        path.push_back(indirect);
    
    std::size_t index = path.size();
    
    if ((*indirect)->left == nullptr and (*indirect)->right == nullptr){  // the node is leaf
        delete *indirect;  // just delete node
        *indirect = nullptr;
        path.pop_back();  // pop the deleted node from path
    }
    
    else if ((*indirect)->right == nullptr){  // only left child exists
        AVLTreeNode<T> *toRemove = *indirect;
        
        (*indirect) = (*indirect)->left;
        delete toRemove;
        
        path.pop_back();
    }
    
    else{  // right child exists
        AVLTreeNode<T> **successor = &((*indirect)->right);
        
        while ((*successor)->left != nullptr){
            path.push_back(successor);
            successor = &((*successor)->left);
        }
        
        if (*successor == (*indirect)->right){
            (*successor)->left = (*indirect)->left;
            
            AVLTreeNode<T> *toRemove = *indirect;
            *indirect = *successor;
            delete toRemove;
        }
        
        else{
            AVLTreeNode<T> *tmp = *path.back(), *suc = *successor;
            
            tmp->left = (*successor)->right;
            suc->left = (*indirect)->left;
            suc->right = (*indirect)->right;
            
            delete *indirect;
            *indirect = suc;
            path[index] = &(suc->right);
        }
    }
    
    balance(path);
    _size--;
}


template <typename T>
bool AVLTreeBase<T>::empty() const{
    return _size == 0;
}

template <typename T>
int AVLTreeBase<T>::size() const{
    return _size;
}

template <typename T>
int AVLTreeBase<T>::find(T value) const{
    AVLTreeNode<T> *direct = root;
    int idx = 0;
    
    while (direct != nullptr and direct->value != value){
        if (direct->value > value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    if (direct == nullptr)
        return -1;
    
    else
        return idx + (direct->left ? direct->left->count : 0);
}

template <typename T>
int AVLTreeBase<T>::upper_bound(T value) const{
    AVLTreeNode<T> *direct = root;
    int idx = 0;
    
    while (direct != nullptr){
        if (direct->value > value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    return idx;
}

template <typename T>
int AVLTreeBase<T>::lower_bound(T value) const{
    AVLTreeNode<T> *direct = root;
    int idx = 0;
    
    while (direct != nullptr){
        if (direct->value >= value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    return idx;
}

template <typename T>
const T& AVLTreeBase<T>::find_min() const{
    AVLTreeNode<T> *cur = root;
    
    while (cur->left != nullptr)
        cur = cur->left;
    
    return cur->value;
}

template <typename T>
const T& AVLTreeBase<T>::find_max() const{
    AVLTreeNode<T> *cur = root;
    
    while (cur->right != nullptr)
        cur = cur->right;
    
    return cur->value;
}

template <typename T>
const T& AVLTreeBase<T>::operator[](std::size_t idx) const{
    AVLTreeNode<T> *cur = root;
    int left = cur->left != nullptr ? cur->left->count : 0;
    
    while (left != idx){
        if (left < idx){
            idx -= left + 1;
            
            cur = cur->right;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
        
        else{
            cur = cur->left;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
    }
    
    return cur->value;
}

template <typename T>
void AVLTreeBase<T>::display(){
    printf("\n");
    if (root != nullptr)
        display(root);
    else
        printf("Empty");
    printf("\n");
}

template <typename T>
void AVLTreeBase<T>::display(AVLTreeNode<T> *cur, int depth, int state){  // state: 1 -> left, 2 -> right , 0 -> root
    if (cur->left)
        display(cur->left, depth + 1, 1);
    
    for (int i=0; i < depth; i++)
        printf("     ");
    
    if (state == 1) // left
        printf("????????????");
    else if (state == 2)  // right
        printf("????????????");
    
    std::cout << "[" << cur->value << "] - (" << cur->count << ", " << cur->height << ")" << std::endl;
    
    if (cur->right)
        display(cur->right, depth + 1, 2);
}

template class AVLTreeBase<int>;
template class AVLTreeBase<short>;
template class AVLTreeBase<long>;
template class AVLTreeBase<long long>;
template class AVLTreeBase<std::string>;

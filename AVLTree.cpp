#include "AVLTree.h"

template <class T>
void AVLTree<T>::balance(
    std::vector<AVLTreeNode<T> **> path) { // starting from root
  std::reverse(path.begin(), path.end());

  for (auto indirect : path) {
    (*indirect)->updateValues();

    if ((*indirect)->balanceFactor() >= 2 and
        (*indirect)->left->balanceFactor() >= 0) { // left - left
      *indirect = (*indirect)->right_rotate();
    } else if ((*indirect)->balanceFactor() >= 2) { // left - right
      (*indirect)->left = (*indirect)->left->left_rotate();
      *indirect = (*indirect)->right_rotate();
    }

    else if ((*indirect)->balanceFactor() <= -2 and
             (*indirect)->right->balanceFactor() <= 0) { // right - right
      *indirect = (*indirect)->left_rotate();
    } else if ((*indirect)->balanceFactor() <= -2) { // right - left
      (*indirect)->right = ((*indirect)->right)->right_rotate();
      *indirect = (*indirect)->left_rotate();
    }
  }
}

template class AVLTree<int>;
template class AVLTree<short>;
template class AVLTree<long>;
template class AVLTree<long long>;
template class AVLTree<std::string>;

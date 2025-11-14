#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;

// You may add any prototypes of helper functions here
int getHeight(Node* root);

bool equalPaths(Node* root)
{
  // Add your code below

  // base case
  if (root == nullptr) {
    return true;
  }

  // helper function 
  int height = getHeight(root);
  if (height == -1) {
    return false;
  } else {
    return true;
  }
}

int getHeight(Node* root) {
  // base case
  if (root == nullptr) {
    return 0;
  }

  // if it's a leaf
  if (root->left == nullptr && root->right == nullptr) {
    return 1;
  }
  
  // compute left height, right height
  int leftHeight = getHeight(root->left);
  int rightHeight = getHeight(root->right);
  // if the tree is unequal (based on the if statements below)
  if (leftHeight == -1 || rightHeight == -1) {
    return -1;
  }

  // if both children exists
  if (root->left != nullptr && root->right != nullptr) {
    if (leftHeight != rightHeight) {
      return -1;
    } else {
      return leftHeight + 1;
    }
  }

  // if only one child exists
  if (root->left != nullptr) {
    return leftHeight + 1;
  } else {
    return rightHeight + 1;
  }
}
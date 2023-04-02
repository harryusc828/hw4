#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int height(Node * root); 

int height(Node* root){
    if (root == nullptr){
        return 0;
    }
    if(height(root -> left) > height(root -> right) ){
        return height(root -> left) + 1;
    }
    else {
        return height(root -> right) + 1; 
    }

}


bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr){
        return true; 
        // nothing, vacuously true
    }
    else if (root -> left == nullptr && root -> right == nullptr){
        return true;
        //the leaf itself has to be true
    }
    else if (root -> left == nullptr){
        return equalPaths(root -> right); 
        // left is nothing so only care about right tree
    }
    else if (root -> right == nullptr){
        return equalPaths(root -> left); 
        //right is nothing so only care about left tree
    }
    return equalPaths(root -> left) && equalPaths(root -> right) && height(root -> left) == height(root -> right); 
    // both subtrees have content so if they are both of the same height and they are equalPaths, then we return true, otherwise false
    
}


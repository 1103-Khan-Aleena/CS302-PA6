//ALEENA KHAN
//PURPOSE: generates 100 random numbers from 1-200 and inserts them into BST and dispays the trees height traversal orders
//DATE: 7/9/2025
#include "binarysearchtree.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void display(int& item) {
    cout << item << " ";
}

int main() {
    BinarySearchTree<int> numberTree;
    srand(time(0));

    int numbersInserted = 0;
    while (numbersInserted < 100) {
        int randomNum = rand() % 200 + 1; //%200 gives 1-199 MUST add 1!!
        if (numberTree.add(randomNum)) { 
            numbersInserted++;
        }
    }

    // Display tree information
    cout << "Tree height: " << numberTree.getHeight() << endl;
    
    cout << "Inorder traversal: "<<endl;
    numberTree.inorderTraverse(display);
    cout << endl;
    
    cout << "Preorder traversal: "<<endl;
    numberTree.preorderTraverse(display);
    cout << endl;
    
    cout << "Postorder traversal: "<<endl;
    numberTree.postorderTraverse(display);
    cout << endl;

    return 0;
}
//ALEENA KHAN
//PURPOSE:provides a base binary tree class with the basic functionality that BST uses for its specific rules and functions. 
//DATE: 7/9/2025
#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include "tree.h"
#include "linkedtreenode.h"
#include <iostream>
using namespace std;

template<class ItemType>
class BinaryTree : public TreeInterface<ItemType> {
protected:
    LinkedTreeNode<ItemType>* rootPtr;
    int nodeCount;

    int getHeightHelper(LinkedTreeNode<ItemType>* subTreePtr) const {
       if (subTreePtr == nullptr){
        return 0; // empty tree has a height of 0!!!
    }
    else{
        //get the height of the left subtree
        int leftHeight = getHeightHelper(subTreePtr->getLeftChildPtr());
        int rightHeight = getHeightHelper(subTreePtr->getRightChildPtr());
        int tallerChild;
        if (leftHeight > rightHeight){
            tallerChild = leftHeight;
        }
        else{
            tallerChild=rightHeight;
        }
        return 1+tallerChild;
    }
 }

    int getNumberOfNodesHelper(LinkedTreeNode<ItemType>* subTreePtr) const {
        if (subTreePtr == nullptr){ //if we reach an empty spot
            return 0;
        }
       
        int leftCount = getNumberOfNodesHelper(subTreePtr->getLeftChildPtr());
        int rightCount = getNumberOfNodesHelper(subTreePtr->getRightChildPtr());
        return  1+ leftCount + rightCount;
    }

    LinkedTreeNode<ItemType>* balancedAdd(LinkedTreeNode<ItemType>* subTreePtr, LinkedTreeNode<ItemType>* newNodePtr) {
        if (subTreePtr == nullptr){ 
        return newNodePtr;
        }
        else{
        LinkedTreeNode<ItemType>* leftPtr = subTreePtr->getLeftChildPtr();
        LinkedTreeNode<ItemType>* rightPtr = subTreePtr->getRightChildPtr();
        
            if (getHeightHelper(leftPtr) > getHeightHelper(rightPtr)) {
                rightPtr = balancedAdd(rightPtr, newNodePtr);
                subTreePtr->setRightChildPtr(rightPtr);
            }
            else{
                leftPtr = balancedAdd(leftPtr, newNodePtr);
                subTreePtr -> setLeftChildPtr(leftPtr);
            }
            return subTreePtr;
        }
    }
    
    LinkedTreeNode<ItemType>* removeValue(LinkedTreeNode<ItemType>* subTreePtr, const ItemType& target, bool& success) {
        if (subTreePtr == nullptr) {
            success = false;
            return nullptr;
        }
        if (subTreePtr->getItem() == target) {
            success = true;
            return moveValuesUpTree(subTreePtr);
        }
        //look at the left subtree
        LinkedTreeNode<ItemType>* newLeft = removeValue(subTreePtr->getLeftChildPtr(), target, success);
        subTreePtr->setLeftChildPtr(newLeft);
        //if its not in the left look at the right
        if (!success) {
            LinkedTreeNode<ItemType>* newRight = removeValue(subTreePtr->getRightChildPtr(), target, success);
            subTreePtr->setRightChildPtr(newRight);
        }
        return subTreePtr;
    }

    LinkedTreeNode<ItemType>* removeLeftmostNode(LinkedTreeNode<ItemType>* nodePtr, ItemType& inorderSuccessor) {
        if (nodePtr->getLeftChildPtr() == nullptr) {
            // Found the leftmost node
            inorderSuccessor = nodePtr->getItem();
            return removeNode(nodePtr);
        }
        else {
            // Recurse left
            LinkedTreeNode<ItemType>* tempPtr = removeLeftmostNode(nodePtr->getLeftChildPtr(), inorderSuccessor);
            nodePtr->setLeftChildPtr(tempPtr);
            return nodePtr;
        }
    }

     LinkedTreeNode<ItemType>* removeNode(LinkedTreeNode<ItemType>* nodePtr) {
        if (nodePtr->isLeaf()) {
            delete nodePtr;
            return nullptr;
        }
        else if (nodePtr->getLeftChildPtr() == nullptr) {
            // Only right child
            LinkedTreeNode<ItemType>* nodeToConnectPtr = nodePtr->getRightChildPtr();
            delete nodePtr;
            return nodeToConnectPtr;
        }
        else if (nodePtr->getRightChildPtr() == nullptr) {
            // Only left child
            LinkedTreeNode<ItemType>* nodeToConnectPtr = nodePtr->getLeftChildPtr();
            delete nodePtr;
            return nodeToConnectPtr;
        }
        else {
            // Node has two children
            return nullptr; 
        }
    }



    //copites the values in the tree and replaces until a leaf is seen and then removed!!!
    LinkedTreeNode<ItemType>* moveValuesUpTree(LinkedTreeNode<ItemType>* subTreePtr) {
        //1st case if the node is a leaf
        if (subTreePtr->isLeaf()) {
            delete subTreePtr;
            return nullptr;
        }
        // if it only has a left child
        else if (subTreePtr->getRightChildPtr() == nullptr) {
            //copt left value up
            ItemType leftValue = subTreePtr -> getLeftChildPtr()->getItem();
            subTreePtr->setItem(leftValue);
            //recursion!! move the values up the tree
            LinkedTreeNode<ItemType>* newLeft = moveValuesUpTree(subTreePtr->getLeftChildPtr());
            subTreePtr->setLeftChildPtr(newLeft);
            return subTreePtr;
        }
        // has a right child
        else if (subTreePtr->getLeftChildPtr()== nullptr){
            //copy the right value up
            ItemType rightValue = subTreePtr->getRightChildPtr()->getItem();
            subTreePtr->setItem(rightValue);
            //recursiona again
            LinkedTreeNode<ItemType>* newRight = moveValuesUpTree(subTreePtr->getRightChildPtr());
            subTreePtr->setRightChildPtr(newRight);
            return subTreePtr;
        }
        //if the node has TWO children
        else {
            ItemType inorderSuccessor;
            LinkedTreeNode<ItemType>* tempPtr = removeLeftmostNode(subTreePtr->getRightChildPtr(), inorderSuccessor);
            subTreePtr->setRightChildPtr(tempPtr);
            subTreePtr->setItem(inorderSuccessor);
            return subTreePtr;
        }
    }

    void destroyTree(LinkedTreeNode<ItemType>* subTreePtr) {
        if (subTreePtr != nullptr) {
            destroyTree(subTreePtr->getLeftChildPtr());
            destroyTree(subTreePtr->getRightChildPtr());
            delete subTreePtr;
        }
    }

    bool containsHelper(LinkedTreeNode<ItemType>* subTreePtr, const ItemType& target) const {
        if (subTreePtr == nullptr) return false;
        if (subTreePtr->getItem() == target) return true;
        return containsHelper(subTreePtr->getLeftChildPtr(), target) || containsHelper(subTreePtr->getRightChildPtr(), target);
    }

public:
   
    BinaryTree(){
     rootPtr=nullptr;
      nodeCount=0;
    }

    virtual ~BinaryTree() {
        clear();
    }

    bool isEmpty() const override {
        return rootPtr == nullptr;
    }

    int getHeight() const override {
        return getHeightHelper(rootPtr);
    }

    int getNumberOfNodes() const override {
    return getNumberOfNodesHelper(rootPtr); 
    }

    ItemType getRootData() const override {
        if (isEmpty()){
            cout<<"Your tree is empty!"<<endl;
            return ItemType();
        }
        return rootPtr->getItem();
    }

    bool add(const ItemType& newData) override {
        LinkedTreeNode<ItemType>* newNodePtr = new LinkedTreeNode<ItemType>(newData);
        rootPtr = balancedAdd(rootPtr, newNodePtr);
        nodeCount++;
        return true;
    }

    bool remove(const ItemType& target) override {
        bool success = false;
        rootPtr = removeValue(rootPtr, target, success);
        if (success) {
            nodeCount--;
        }
        return success;
    }

    void clear() override {
        destroyTree(rootPtr);
        rootPtr = nullptr;
        nodeCount = 0;
    }

    bool contains(const ItemType& anEntry) const override {
        return containsHelper(rootPtr, anEntry);
    }
};

#endif 
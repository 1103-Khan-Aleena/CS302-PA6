//ALEENA KHAN
//PURPOSE:implements BST with insertion, deletion, etc that help create BST actions
//DATE: 7/9/2025
#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H
#include "binarytree.h"
#include <iostream>
using namespace std;

template<class ItemType>
class BinarySearchTree : public BinaryTree<ItemType> {
protected:
    //insert node in the right spot, left if smaller right if bigger
    LinkedTreeNode<ItemType>* insertInorder(LinkedTreeNode<ItemType>* subTreePtr, LinkedTreeNode<ItemType>* newNodePtr) {
        if (subTreePtr == nullptr) {
            return newNodePtr;
        }
        //go left if new node val is smaller than current
        else if (newNodePtr->getItem() < subTreePtr->getItem()) {
            LinkedTreeNode<ItemType>* tempPtr = insertInorder(subTreePtr->getLeftChildPtr(), newNodePtr);
            subTreePtr->setLeftChildPtr(tempPtr);
        }
        //if not go right 
        else {
            LinkedTreeNode<ItemType>* tempPtr = insertInorder(subTreePtr->getRightChildPtr(), newNodePtr);
            subTreePtr->setRightChildPtr(tempPtr);
        }
        return subTreePtr;
    }
    //help locate and remove node with a specificv val
    LinkedTreeNode<ItemType>* removeValue(LinkedTreeNode<ItemType>* subTreePtr, const ItemType target, bool& success) {
        if (subTreePtr == nullptr) {
            success = false;
            return nullptr;
        }
        
        if (subTreePtr->getItem() == target) {
            subTreePtr = removeNode(subTreePtr);
            success = true;
        }
        //keep looking left
        else if (subTreePtr->getItem() > target) {
            LinkedTreeNode<ItemType>* tempPtr = removeValue(subTreePtr->getLeftChildPtr(), target, success);
            subTreePtr->setLeftChildPtr(tempPtr);
        }
        //at the end look for the right if still not done
        else {
            LinkedTreeNode<ItemType>* tempPtr = removeValue(subTreePtr->getRightChildPtr(), target, success);
            subTreePtr->setRightChildPtr(tempPtr);
        }
        return subTreePtr;
    }

    LinkedTreeNode<ItemType>* removeNode(LinkedTreeNode<ItemType>* nodePtr) {
        //if node is leaf
        if (nodePtr->isLeaf()) {
            delete nodePtr;
            return nullptr;
        }
        else if (nodePtr->getLeftChildPtr() == nullptr) {
            //only has a right child the left is empty
            LinkedTreeNode<ItemType>* nodeToConnectPtr = nodePtr->getRightChildPtr();
            delete nodePtr;
            return nodeToConnectPtr; //connects the part to teh right child
        }
        //only has a left child, the right is empty
        else if (nodePtr->getRightChildPtr() == nullptr) {
            LinkedTreeNode<ItemType>* nodeToConnectPtr = nodePtr->getLeftChildPtr();
            delete nodePtr;
            return nodeToConnectPtr; //connecting to the left child
        }
        else {
            ItemType inorderSuccessor;
            LinkedTreeNode<ItemType>* tempPtr = removeLeftmostNode(nodePtr->getRightChildPtr(), inorderSuccessor);
            nodePtr->setRightChildPtr(tempPtr);
            nodePtr->setItem(inorderSuccessor);
            return nodePtr;
        }
    }
    
    LinkedTreeNode<ItemType>* removeLeftmostNode(LinkedTreeNode<ItemType>* nodePtr, ItemType& inorderSuccessor) {
        if (nodePtr->getLeftChildPtr() == nullptr) {
            inorderSuccessor = nodePtr->getItem();
            return removeNode(nodePtr);
        }
        else {
            LinkedTreeNode<ItemType>* tempPtr = removeLeftmostNode(nodePtr->getLeftChildPtr(), inorderSuccessor);
            nodePtr->setLeftChildPtr(tempPtr);
            return nodePtr;
        }
    }

    bool findNode(LinkedTreeNode<ItemType>* treePtr, const ItemType& target) const {
        if (treePtr == nullptr) {
            return false;
        }
        else if (treePtr->getItem() == target) {
            return true;
        }
        else if (treePtr->getItem() > target) {
            return findNode(treePtr->getLeftChildPtr(), target);
        }
        else {
            return findNode(treePtr->getRightChildPtr(), target);
        }
    }
    //left then root then right!!!
    void inorder(void visit(ItemType&), LinkedTreeNode<ItemType>* treePtr) const {
        if (treePtr == nullptr) {
                return;
        }
            inorder(visit, treePtr->getLeftChildPtr()); //left subtree
            ItemType currentItem = treePtr->getItem();//current node
            visit(currentItem);
            inorder(visit, treePtr->getRightChildPtr());
        }

    // root then left then right
    void preorder(void visit(ItemType&), LinkedTreeNode<ItemType>* treePtr) const {
        if (treePtr == nullptr) {
            return;
        }
            ItemType currentItem = treePtr->getItem();
            visit(currentItem);
            preorder(visit, treePtr->getLeftChildPtr());
            preorder(visit, treePtr->getRightChildPtr());
        }
    
        //left then right thennnn root
    void postorder(void visit(ItemType&), LinkedTreeNode<ItemType>* treePtr) const {
        if (treePtr == nullptr) {
            return;
        }
            postorder(visit, treePtr->getLeftChildPtr());
            postorder(visit, treePtr->getRightChildPtr());
            ItemType currentItem = treePtr->getItem();
            visit(currentItem);
        }
    

public:
    BinarySearchTree() : BinaryTree<ItemType>() {}
    
    bool add(const ItemType& newData) override {
        if (this->contains(newData)) {
            return false;
        }
        LinkedTreeNode<ItemType>* newNode = new LinkedTreeNode<ItemType>(newData);
        this->rootPtr = insertInorder(this->rootPtr, newNode);
        this->nodeCount++;
       
        return true;
    }
    
    bool remove(const ItemType& data) override {
        bool success = false;
        this->rootPtr = removeValue(this->rootPtr, data, success);
        if (success) {
            this->nodeCount--;
        }
        return success;
    }

    bool contains(const ItemType& anEntry) const override {
        return findNode(this->rootPtr, anEntry);
    }

    void inorderTraverse(void visit(ItemType&)) const {
        inorder(visit, this->rootPtr);
    }

    void preorderTraverse(void visit(ItemType&)) const {
        preorder(visit, this->rootPtr);
    }

    void postorderTraverse(void visit(ItemType&)) const {
        postorder(visit, this->rootPtr);
    }
};

#endif
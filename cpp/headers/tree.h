#ifndef TREE_H
#define TREE_H

#include "array.h"

struct TreeNode
{
    int data;
    TreeNode* left;
    TreeNode* right;
};

struct Tree
{
    TreeNode* root;
    int size;
};

void TreeInit(Tree& t);
void TreeFree(Tree& t);

void TreeInsert(Tree& t, int value);
bool TreeInsertBST(Tree& t, int value);          // лаб. 2: вставка в дерево поиска, false - ключ уже есть
void TreeRightEdge(Tree& t, Array& out);         // лаб. 2: правый край дерева
bool TreeBuild(Tree& t, Array& tokens, int from);   // "#" - пустое место
bool TreeFind(Tree& t, int value, int& level);
bool TreeIsComplete(Tree& t);

void TreeInOrder(Tree& t, Array& out);
void TreePreOrder(Tree& t, Array& out);
void TreePostOrder(Tree& t, Array& out);
void TreeLevelOrder(Tree& t, Array& out);
void TreeToArray(Tree& t, Array& out);   // обход в ширину с "#" на пустых местах
void TreePrint(Tree& t);

#endif

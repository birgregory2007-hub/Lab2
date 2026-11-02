#include <iostream>
#include "../headers/tree.h"
#include "../headers/utils.h"

void TreeInit(Tree& t)
{
    t.root = 0;
    t.size = 0;
}

static TreeNode* NewNode(int value)
{
    TreeNode* n = new TreeNode;
    n->data = value;
    n->left = 0;
    n->right = 0;
    return n;
}

static void FreeNode(TreeNode* n)
{
    if (n == 0)
        return;
    FreeNode(n->left);
    FreeNode(n->right);
    delete n;
}

void TreeFree(Tree& t)
{
    FreeNode(t.root);
    TreeInit(t);
}

void TreeInsert(Tree& t, int value)
{
    TreeNode* n = NewNode(value);
    if (t.root == 0)
    {
        t.root = n;
        t.size = 1;
        return;
    }
    TreeNode** queue = new TreeNode*[t.size + 1];
    int head = 0;
    int tail = 0;
    queue[tail++] = t.root;
    while (head < tail)
    {
        TreeNode* cur = queue[head++];
        if (cur->left == 0)
        {
            cur->left = n;
            break;
        }
        queue[tail++] = cur->left;
        if (cur->right == 0)
        {
            cur->right = n;
            break;
        }
        queue[tail++] = cur->right;
    }
    delete[] queue;
    t.size++;
}

bool TreeBuild(Tree& t, Array& tokens, int from)
{
    TreeFree(t);
    int count = tokens.size - from;
    if (count <= 0)
        return true;

    int value;
    if (!ToInt(tokens.data[from], value))
        return false;
    t.root = NewNode(value);
    t.size = 1;

    TreeNode** queue = new TreeNode*[count + 1];
    int head = 0;
    int tail = 0;
    queue[tail++] = t.root;
    int pos = from + 1;
    bool ok = true;
    while (head < tail && pos < tokens.size && ok)
    {
        TreeNode* cur = queue[head++];
        for (int side = 0; side < 2 && pos < tokens.size; side++)
        {
            string word = tokens.data[pos];
            pos++;
            if (word == "#")
                continue;
            if (!ToInt(word, value))
            {
                ok = false;
                break;
            }
            TreeNode* n = NewNode(value);
            if (side == 0)
                cur->left = n;
            else
                cur->right = n;
            queue[tail++] = n;
            t.size++;
        }
    }
    delete[] queue;
    if (pos < tokens.size)
        ok = false;
    if (!ok)
    {
        TreeFree(t);
        return false;
    }
    return true;
}

bool TreeFind(Tree& t, int value, int& level)
{
    if (t.root == 0)
        return false;
    TreeNode** queue = new TreeNode*[t.size + 1];
    int* levels = new int[t.size + 1];
    int head = 0;
    int tail = 0;
    queue[tail] = t.root;
    levels[tail] = 0;
    tail++;
    bool found = false;
    while (head < tail && !found)
    {
        TreeNode* cur = queue[head];
        int curLevel = levels[head];
        head++;
        if (cur->data == value)
        {
            found = true;
            level = curLevel;
        }
        if (cur->left != 0)
        {
            queue[tail] = cur->left;
            levels[tail] = curLevel + 1;
            tail++;
        }
        if (cur->right != 0)
        {
            queue[tail] = cur->right;
            levels[tail] = curLevel + 1;
            tail++;
        }
    }
    delete[] queue;
    delete[] levels;
    return found;
}

bool TreeIsComplete(Tree& t)
{
    if (t.root == 0)
        return true;
    TreeNode** queue = new TreeNode*[2 * t.size + 2];
    int head = 0;
    int tail = 0;
    queue[tail++] = t.root;
    bool seenEmpty = false;
    bool result = true;
    while (head < tail && result)
    {
        TreeNode* cur = queue[head++];
        if (cur == 0)
        {
            seenEmpty = true;
        }
        else
        {
            if (seenEmpty)
                result = false;
            queue[tail++] = cur->left;
            queue[tail++] = cur->right;
        }
    }
    delete[] queue;
    return result;
}

static void InOrder(TreeNode* n, Array& out)
{
    if (n == 0)
        return;
    InOrder(n->left, out);
    ArrayPush(out, to_string(n->data));
    InOrder(n->right, out);
}

static void PreOrder(TreeNode* n, Array& out)
{
    if (n == 0)
        return;
    ArrayPush(out, to_string(n->data));
    PreOrder(n->left, out);
    PreOrder(n->right, out);
}

static void PostOrder(TreeNode* n, Array& out)
{
    if (n == 0)
        return;
    PostOrder(n->left, out);
    PostOrder(n->right, out);
    ArrayPush(out, to_string(n->data));
}

void TreeInOrder(Tree& t, Array& out)
{
    InOrder(t.root, out);
}

void TreePreOrder(Tree& t, Array& out)
{
    PreOrder(t.root, out);
}

void TreePostOrder(Tree& t, Array& out)
{
    PostOrder(t.root, out);
}

void TreeLevelOrder(Tree& t, Array& out)
{
    if (t.root == 0)
        return;
    TreeNode** queue = new TreeNode*[t.size + 1];
    int head = 0;
    int tail = 0;
    queue[tail++] = t.root;
    while (head < tail)
    {
        TreeNode* cur = queue[head++];
        ArrayPush(out, to_string(cur->data));
        if (cur->left != 0)
            queue[tail++] = cur->left;
        if (cur->right != 0)
            queue[tail++] = cur->right;
    }
    delete[] queue;
}

void TreeToArray(Tree& t, Array& out)
{
    if (t.root == 0)
        return;
    TreeNode** queue = new TreeNode*[2 * t.size + 2];
    int head = 0;
    int tail = 0;
    queue[tail++] = t.root;
    int seen = 0;
    while (head < tail && seen < t.size)
    {
        TreeNode* cur = queue[head++];
        if (cur == 0)
        {
            ArrayPush(out, "#");
        }
        else
        {
            ArrayPush(out, to_string(cur->data));
            seen++;
            queue[tail++] = cur->left;
            queue[tail++] = cur->right;
        }
    }
    delete[] queue;
}

static void PrintNode(TreeNode* n, int depth)
{
    if (n == 0)
        return;
    PrintNode(n->right, depth + 1);
    cout << "-> ";
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << n->data << endl;
    PrintNode(n->left, depth + 1);
}

void TreePrint(Tree& t)
{
    if (t.root == 0)
    {
        cout << "-> (empty)" << endl;
        return;
    }
    PrintNode(t.root, 0);
}

bool TreeInsertBST(Tree& t, int value)
{
    TreeNode** link = &t.root;
    while (*link != 0)
    {
        if (value == (*link)->data)
            return false;
        if (value < (*link)->data)
            link = &(*link)->left;
        else
            link = &(*link)->right;
    }
    *link = NewNode(value);
    t.size++;
    return true;
}

void TreeRightEdge(Tree& t, Array& out)
{
    if (t.root == 0)
        return;
    TreeNode** queue = new TreeNode*[t.size + 1];
    int head = 0;
    int tail = 0;
    queue[tail++] = t.root;
    while (head < tail)
    {
        int count = tail - head;
        for (int i = 0; i < count; i++)
        {
            TreeNode* cur = queue[head++];
            if (i == count - 1)
                ArrayPush(out, to_string(cur->data));
            if (cur->left != 0)
                queue[tail++] = cur->left;
            if (cur->right != 0)
                queue[tail++] = cur->right;
        }
    }
    delete[] queue;
}

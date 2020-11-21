/**
 * @file RBTree.c
 * @author  Inbal Lavi <inbal.lavi1@mail.huji.ac.il>
 * @version 1.0
 * @date 3 June 2020
 *
 * @brief Red Black Tree generic code
 *
 * @section LICENSE
 * is free and should be used only for good. we do not support the dark side.
 *
 * @section DESCRIPTION
 * a generic code of a RBTree
 * allows you to create and delete a tree, insert and remove data.
 * also lets you preform actions on the tree in an easy way.
 */
// ------------------------------ includes ------------------------------
#include <stdlib.h>
#include "Structs.h"
#include "RBTree.h"

// -------------------------- const definitions -------------------------
/**
 * @brief return value for functions
 */
typedef enum FunctionReturn
{
    FAIL,
    SUCCESS
} FunctionReturn;

/**
 * @brief boolean values
 */
typedef enum bool
{
    FALSE,
    TRUE
} bool;

/**
 * @brief used for checking where to go and if a Node is a left or right child
 */
typedef enum LeftOrRightChild
{
    ROOT,
    RIGHT,
    LEFT = -1
} LeftOrRightChild;

/**
 * @brief used for kids of a Node
 */
enum Kids
{
    ZERO_KIDS,
    ONE_KID,
    TWO_KIDS
};

/**
 * @brief used for empty tree size
 */
#define EMPTY 0

/**
 * @brief checks if functions failed
 */
#define CHECK_FAIL \
if (failOrNah == FAIL) \
{ \
    return FAIL; \
} \


// -------------------------- func declarations -------------------------
// ------------- general -------------
/**
 * @brief gets a number from the compFunc and tells you where to go
 * @param num - compFunc return value
 * @return LEFT, RIGHT or ROOT - if there's no where to go
 */
int whereToGo(int num);

/**
 * @brief gets a pointer to Node and return its brother
 * @param node - pointer to a Node
 * @return pointer to its brother
 */
Node *setBrother(Node *node);

/**
 * @brief checks if a Node is a right child, a left child or the root
 * @param node - pointer to a Node
 * @return LEFT, RIGHT or ROOT
 */
int isRightLeftChildOrRoot(const Node *node);

/**
 * @brief rotates the tree left for a node
 * @param tree - pointer to the tree (to change the root if necessary)
 * @param node - pointer to the node to rotate on
 */
void turnLeft(RBTree *tree, Node *node);

/**
 * @brief rotates the tree right for a node
 * @param tree - pointer to the tree (to change the root if necessary)
 * @param node - pointer to the node to rotate on
 */
void turnRight(RBTree *tree, Node *node);

// -------------- insert --------------
/**
 * @brief inserts a Node to a tree in a regular BST way
 * @param tree - pointer to the tree
 * @param newNode - pointer to Node to insert
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertRegular(RBTree *tree, Node *newNode);

/**
 * @brief RBTree fixing algorithm after insert
 * @param tree - pointer to the tree
 * @param node - new node to be fixed (if needed)
 */
void fixingAlg(RBTree *tree, Node *node);

/**
 * @brief if the new node has red dad and red uncle fixing function
 * @param new - node to fix
 */
void redDadRedUncle(Node *new);

/**
 * @brief if the new node has red dad and red uncle fixing function
 * @param tree - tree to fix
 * @param new - node to fix
 */
void redDadBlackUncle(RBTree *tree, Node *new);

// -------------- delete --------------
/**
 * @brief finds to node to delete
 * @param tree - tree to delete from
 * @param data - item to be deleted
 * @return - pointer to the node to be deleted or NULL if not in tree
 */
Node *findNode(RBTree *tree, const void *data);

/**
 * @brief finds a node successor
 * @param node - to find successor for
 * @return pointer to the successor
 */
Node *successor(const Node *node);

/**
 * @brief swaps two nodes' data
 * @param a @param b two nodes
 */
void swapData(Node *a, Node *b);

/**
 * @brief swaps two nodes' color
 * @param a @param b two nodes
 */
void swapColor(Node *a, Node *b);

/**
 * @brief checks how many kids a node hase
 * @param node - the node to check
 * @return 0, 1 or 2
 */
int howManyKIds(const Node *node);

/**
 * @brief set M's node child
 * @param M - the node to find a child for
 * @return pointer to M's only child or NULL if has not children
 */
Node *setC(Node *M);

/**
 * @brief put the node @C instead of node @M in the tree
 */
void putCInM(Node *C, Node *M);

/**
 * @brief check if a node has two black kids (TRUE also if a node's a leaf)
 * @param node - to check
 * @return TRUE or FALSE
 */
bool hasTwoBlackKids(Node *node);

/**
 * @brief receives a node's brother S, and checks if the close nephew is red and the far on is black
 * @param S - a node's brother
 * @param side - the node's side in the tree
 * @return TRUE or FALSE
 */
bool redScBlackSf(const Node *S, LeftOrRightChild side);

/**
 * @brief if M's black and its child is black fixing algorithm
 * @param tree - the tree to fix
 * @param M - the node to delete
 */
void blackMAndC(RBTree *tree, Node *M);

/**
 * @brief tells you which child C (S's brother) is
 * @param S - C's brother
 * @return if C is a left child or right child
 */
LeftOrRightChild setCSide(Node *S);

/**
 * @brief turn Sc black and S red, ratate S to the opposite side of C
 * @param tree - the tree to fix
 * @param S - C's brother (DB)
 */
void blackSAndSfRedSc(RBTree *tree, Node *S);

/**
 * @brief switch S's and P's color. rotate P toward C. turn Sf black.
 * @param tree - the tree to fix
 * @param S - C's brother (DB)
 */
void blackSRedSf(RBTree *tree, Node *S);

/**
 * @brief deletes (frees) a node and its data
 * @param M - the node to delete
 * @param func - freeing data func
 */
void deleteNode(Node **M, FreeFunc func);

// ------------- for each -------------
/**
 * @brief a function to help preform an action on every node in the tree
 * @param node - to do the func on (starts from root and calles itself recursively)
 * @param func - the function to preform
 * @param args - an argument for the func
 * @return 0 on failure, other on success
 */
int forEachHelper(const Node *node, forEachFunc func, void *args);

// --------------- free ---------------
/**
 * @brief frees all the nodes (and their data) recursively
 * @param node - the node to free, starting from the root
 * @param func - function to free the data
 */
void freeHelper(Node **node, FreeFunc func);

// ------------------------------ functions -----------------------------
// -------------- general --------------
int whereToGo(const int num)
{
    if (num < 0)
    {
        return LEFT;
    }
    if (num > 0)
    {
        return RIGHT;
    }
    return ROOT;
}

int isRightLeftChildOrRoot(const Node *node)
{
    Node *p = node->parent;
    if (p == NULL)
    {
        return ROOT;
    }
    if (node == p->left)
    {
        return LEFT;
    }
    return RIGHT;
}

void turnLeft(RBTree *tree, Node *node)
{
    Node *x = node;
    Node *y = x->right;
    Node *p = x->parent;

    LeftOrRightChild side = isRightLeftChildOrRoot(x);

    x->parent = y;
    x->right = y->left;
    if (y->left != NULL)
    {
        y->left->parent = x;
    }
    y->left = x;
    y->parent = p;

    switch (side)
    {
        case LEFT:
            p->left = y;
            break;
        case RIGHT:
            p->right = y;
            break;
        default:
            tree->root = y;
            break;
    }
}

void turnRight(RBTree *tree, Node *node)
{
    Node *x = node;
    Node *y = x->left;
    Node *p = x->parent;

    LeftOrRightChild side = isRightLeftChildOrRoot(x);

    x->parent = y;
    x->left = y->right;
    if (y->right != NULL)
    {
        y->right->parent = x;
    }
    y->right = x;
    y->parent = p;

    switch (side)
    {
        case LEFT:
            p->left = y;
            break;
        case RIGHT:
            p->right = y;
            break;
        default:
            tree->root = y;
            break;
    }
}

Node *setBrother(Node *node)
{
    if (node == node->parent->left)
    {
        return node->parent->right;
    }
    return node->parent->left;
}

// --------------- create ---------------
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *tree = (RBTree *) malloc(sizeof(RBTree));
    if (tree == NULL)
    {
        return NULL;
    }

    tree->root = NULL;
    tree->compFunc = compFunc, tree->freeFunc = freeFunc;
    tree->size = EMPTY;

    return tree;
}

// --------------- insert ---------------
int insertToRBTree(RBTree *tree, void *data)
{
    if (data == NULL || tree == NULL)
    {
        return FAIL;
    }
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return FAIL;
    }
    newNode->left = NULL, newNode->right = NULL, newNode->parent = NULL;
    newNode->color = RED;
    newNode->data = data;

    FunctionReturn failOrNah = insertRegular(tree, newNode);
    if (failOrNah == FAIL)
    {
        free(newNode);
        return FAIL;
    }

    fixingAlg(tree, newNode);
    return SUCCESS;
}

int insertRegular(RBTree *tree, Node *newNode)
{
    Node *treeNode = tree->root;
    if (treeNode == NULL)
    {
        tree->root = newNode;
        tree->size++;
        return SUCCESS;
    }

    while (TRUE)
    {
        int next = whereToGo(tree->compFunc(newNode->data, treeNode->data));
        bool turned = FALSE;
        switch (next)
        {
            case RIGHT:
                if (treeNode->right != NULL)
                {
                    treeNode = treeNode->right;
                }
                else
                {
                    newNode->parent = treeNode;
                    treeNode->right = newNode;
                    turned = TRUE;
                }
                break;
            case LEFT:
                if (treeNode->left != NULL)
                {
                    treeNode = treeNode->left;
                }
                else
                {
                    newNode->parent = treeNode;
                    treeNode->left = newNode;
                    turned = TRUE;
                }
                break;
            default:
                return FAIL;
        }
        if (turned)
        {
            break;
        }
    }
    tree->size++;
    return SUCCESS;
}

void fixingAlg(RBTree *tree, Node *node)
{
    Node *toFix = node;
    Node *dad;
    Node *uncle;
    while (TRUE)
    {
        // case 1 - if node is root
        if (toFix == tree->root)
        {
            toFix->color = BLACK;
            return;
        }

        // case 2 - parent is black
        if (toFix->parent->color == BLACK)
        {
            return;
        }

        dad = toFix->parent;
        uncle = setBrother(dad);

        // case 4 - black uncle
        if (uncle == NULL || uncle->color == BLACK)
        {
            redDadBlackUncle(tree, toFix);
            return;
        }

        // case 3 - red uncle
        if (uncle->color == RED)
        {
            redDadRedUncle(toFix);
            toFix = toFix->parent->parent;
        }
    }
}

void redDadRedUncle(Node *new)
{
    Node *dad = new->parent;
    Node *grandad = dad->parent;
    Node *uncle = setBrother(dad);
    dad->color = BLACK, uncle->color = BLACK;
    grandad->color = RED;
}

void redDadBlackUncle(RBTree *tree, Node *new)
{
    Node *dad = new->parent;
    Node *grandad = dad->parent;
    bool turned = FALSE;

    // check if triangle
    if (grandad->left != NULL)
    {
        if (new == grandad->left->right)
        {
            turnLeft(tree, dad);
            new = dad;
            dad = new->parent;
            turned = TRUE;
        }
    }
    if (grandad->right != NULL && !turned)
    {
        if (new == grandad->right->left)
        {
            turnRight(tree, dad);
            new = dad;
            dad = new->parent;
        }
    }
    turned = FALSE;

    // fixing line
    if (grandad->left != NULL)
    {
        if (new == grandad->left->left)
        {
            turnRight(tree, grandad);
            turned = TRUE;
        }
    }
    if (grandad->right != NULL && !turned)
    {
        if (new == grandad->right->right)
        {
            turnLeft(tree, grandad);
        }
    }
    dad->color = BLACK;
    grandad->color = RED;
}

// --------------- delete ---------------
int deleteFromRBTree(RBTree *tree, void *data)
{
    if (data == NULL || tree == NULL)
    {
        return FAIL;
    }

    Node *M = findNode(tree, data);
    // not in tree
    if (M == NULL)
    {
        return FAIL;
    }

    // is M: leaf / has 1 child / has 2 kids
    int kids = howManyKIds(M);
    if (kids == TWO_KIDS)
    {
        Node *MSuccessor = successor(M);
        swapData(M, MSuccessor);
        M = MSuccessor;
    }

    Node *C = setC(M);
    // case 1
    if (M->color == RED)
    {
        putCInM(C, M);
        deleteNode(&M, tree->freeFunc);
        tree->size--;
        return SUCCESS;
    }

    // case 2
    if (C != NULL && C->color == RED)
    {
        // check if m was root
        if (M == tree->root)
        {
            tree->root = C;
        }
        putCInM(C, M);
        deleteNode(&M, tree->freeFunc);
        C->color = BLACK;
        tree->size--;
        return SUCCESS;
    }

    // case 3
    blackMAndC(tree, M);

    tree->size--;
    return SUCCESS;
}

void blackMAndC(RBTree *tree, Node *M)
{
    Node *C = setC(M);
    Node *P = M->parent;

    // a - is root
    if (M == tree->root)
    {
        tree->root = NULL;
        deleteNode(&M, tree->freeFunc);
        return;
    }

    Node *S = setBrother(M);
    putCInM(C, M);
    deleteNode(&M, tree->freeFunc);
    LeftOrRightChild side;

    while (TRUE)
    {
        side = setCSide(S);

        if (S->color == BLACK)
        {
            // b - S and its two sons are black
            if (hasTwoBlackKids(S))
            {
                S->color = RED;
                // i - P is red
                if (P->color == RED)
                {
                    P->color = BLACK;
                    return;
                }
                // ii - P is black
                C = P;
                if (C == tree->root)
                {
                    return;
                }
                P = C->parent;
                S = setBrother(C);
                continue;
            }

            // d - S and Sf are black, Sc is red
            else if (redScBlackSf(S, side))
            {
                blackSAndSfRedSc(tree, S);
                S = S->parent;
                continue;
            }

            // e - S is black, Sf is red
            if (!redScBlackSf(S, side))
            {
                blackSRedSf(tree, S);
                return;
            }
        }

        // c - S is red
        else
        {
            swapColor(S, P);
            switch (side)
            {
                case RIGHT:
                    S = S->right;
                    turnRight(tree, P);
                    break;
                case LEFT:
                    S = S->left;
                    turnLeft(tree, P);
                    break;
                default:
                    break;
            }
        }
    }
}

bool redScBlackSf(const Node *S, LeftOrRightChild side)
{
    switch (side)
    {
        case RIGHT:
            if (S->right == NULL)
            {
                return FALSE;
            }
            if (S->right->color == RED)
            {
                if (S->left == NULL || S->left->color == BLACK)
                {
                    return TRUE;
                }
            }
            return FALSE;
        case LEFT:
            if (S->left == NULL)
            {
                return FALSE;
            }
            if (S->left->color == RED)
            {
                if (S->right == NULL || S->right->color == BLACK)
                {
                    return TRUE;
                }
            }
            return FALSE;
        default:
            return FALSE;
    }
}

bool hasTwoBlackKids(Node *node)
{
    int kids = howManyKIds(node);
    switch (kids)
    {
        case ZERO_KIDS:
            return TRUE;
        case ONE_KID:
            return FALSE;
        default:
            if (node->left->color == BLACK && node->right->color == BLACK)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
    }
}

int howManyKIds(const Node *node)
{
    // check if leaf
    if (node->left == NULL && node->right == NULL)
    {
        return ZERO_KIDS;
    }
    if (node->left != NULL && node->right != NULL)
    {
        return TWO_KIDS;
    }
    return ONE_KID;
}

Node *findNode(RBTree *tree, const void *data)
{
    Node *treeNode = tree->root;
    while (treeNode != NULL)
    {
        int next = whereToGo(tree->compFunc(data, treeNode->data));
        switch (next)
        {
            case RIGHT:
                treeNode = treeNode->right;
                break;
            case LEFT:
                treeNode = treeNode->left;
                break;
            default:
                return treeNode;
        }
    }
    return NULL;
}

Node *successor(const Node *node)
{
    Node *successor = node->right;
    while (successor->left != NULL)
    {
        successor = successor->left;
    }
    return successor;
}

void swapData(Node *a, Node *b)
{
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
}

void swapColor(Node *a, Node *b)
{
    Color temp = a->color;
    a->color = b->color;
    b->color = temp;
}

Node *setC(Node *M)
{
    if (M->right != NULL)
    {
        return M->right;
    }
    return M->left;
}

LeftOrRightChild setCSide(Node *S)
{
    LeftOrRightChild side = isRightLeftChildOrRoot(S);
    if (side == RIGHT)
    {
        return LEFT;
    }
    return RIGHT;
}

void blackSAndSfRedSc(RBTree *tree, Node *S)
{
    LeftOrRightChild side = setCSide(S);
    S->color = RED;
    Node *Sc = NULL;
    switch (side)
    {
        case RIGHT:
            Sc = S->right;
            Sc->color = BLACK;
            turnLeft(tree, S);
            break;
        case LEFT:
            Sc = S->left;
            Sc->color = BLACK;
            turnRight(tree, S);
            break;
        default:
            break;
    }
}

void blackSRedSf(RBTree *tree, Node *S)
{
    LeftOrRightChild side = setCSide(S);
    Node *P = S->parent;
    Node *Sf = NULL;
    swapColor(S, P);
    switch (side)
    {
        case RIGHT:
            Sf = S->left;
            turnRight(tree, P);
            break;
        case LEFT:
            Sf = S->right;
            turnLeft(tree, P);
            break;
        default:
            break;
    }
    Sf->color = BLACK;
}

void putCInM(Node *C, Node *M)
{
    LeftOrRightChild side = isRightLeftChildOrRoot(M);
    // set C as M's parent child
    switch (side)
    {
        case RIGHT:
            M->parent->right = C;
            break;
        case LEFT:
            M->parent->left = C;
            break;
        default:
            break;
    }

    // set C's parent to be M's parent
    if (C != NULL)
    {
        C->parent = M->parent;
    }
}

void deleteNode(Node **M, FreeFunc func)
{
    func((*M)->data);
    free(*M);
    *M = NULL;
}

// --------------- search ---------------
int RBTreeContains(const RBTree *tree, const void *data)
{
    if (data == NULL || tree == NULL)
    {
        return FALSE;
    }
    Node *treeNode = tree->root;

    while (treeNode != NULL)
    {
        int next = whereToGo(tree->compFunc(data, treeNode->data));
        if (next == ROOT)
        {
            return TRUE;
        }
        if (next == RIGHT)
        {
            treeNode = treeNode->right;
        }
        else if (next == LEFT)
        {
            treeNode = treeNode->left;
        }
    }
    return FALSE;
}

// ------------- tree func -------------
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL)
    {
        return FAIL;
    }
    Node *node = tree->root;
    FunctionReturn failOrNah = forEachHelper(node, func, args);
    CHECK_FAIL
    return SUCCESS;
}

int forEachHelper(const Node *node, forEachFunc func, void *args)
{
    if (node == NULL)
    {
        return SUCCESS;
    }
    FunctionReturn failOrNah;
    failOrNah = forEachHelper(node->left, func, args);
    CHECK_FAIL
    failOrNah = func(node->data, args);
    CHECK_FAIL
    failOrNah = forEachHelper(node->right, func, args);
    CHECK_FAIL
    return SUCCESS;
}

// ---------------- free ----------------
void freeRBTree(RBTree **tree)
{
    if (tree == NULL)
    {
        return;
    }
    if (*tree == NULL)
    {
        return;
    }
    freeHelper(&(*tree)->root, (*tree)->freeFunc);
    free(*tree);
    *tree = NULL;
}

void freeHelper(Node **node, FreeFunc func)
{
    if (node == NULL)
    {
        return;
    }
    if (*node == NULL)
    {
        return;
    }
    freeHelper(&((*node)->left), func);
    freeHelper(&((*node)->right), func);
    deleteNode(node, func);
}

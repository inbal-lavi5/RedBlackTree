/**
 * @file Structs.c
 * @author  Inbal Lavi <inbal.lavi1@mail.huji.ac.il>
 * @version 1.0
 * @date 3 June 2020
 *
 * @brief builds structs that use RBTree
 *
 * @section LICENSE
 * is free and should be used only for good. we do not support the dark side.
 *
 * @section DESCRIPTION
 * creates string functions to use RBTree
 * create Vector functions to use RBTree
 */
// ------------------------------ includes ------------------------------
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Structs.h"
#include "RBTree.h"

// -------------------------- const definitions -------------------------
/**
 * @brief return value for functions
 */
enum FunctionExit
{
    FAIL,
    SUCCESS
};

/**
 * values to be returned from campare functions
 */
#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)

// -------------------------- func declarations -------------------------
/**
 * @brief returns the minimum value between @a and @b
 */
int min(int a, int b);

/**
 * @brief calculates a vector norm
 * @param vector - the vector to calculate norm
 * @param len - the vector length
 * @return the norm of the vector
 */
double vectorNorm(const double *vector, int len);

// ------------------------------ functions -----------------------------
int stringCompare(const void *a, const void *b)
{
    return strcmp((char *) a, (char *) b);
}

int concatenate(const void *word, void *pConcatenated)
{
    strcat((char *) pConcatenated, (char *) word);
    strcat((char *) pConcatenated, "\n");
    return SUCCESS;
}

void freeString(void *s)
{
    free(s);
}

int vectorCompare1By1(const void *a, const void *b)
{
    Vector *first = (Vector *) a;
    Vector *second = (Vector *) b;
    double diff;
    int minLen = min(first->len, second->len);
    for (int i = 0; i < minLen; i++)
    {
        diff = first->vector[i] - second->vector[i];
        if (diff < 0)
        {
            return LESS;
        }
        else if (diff > 0)
        {
            return GREATER;
        }
    }
    if (first->len < second->len)
    {
        return LESS;
    }
    else if (first->len > second->len)
    {
        return GREATER;
    }
    else
    {
        return EQUAL;
    }
}

int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}

void freeVector(void *pVector)
{
    Vector *a = (Vector *) pVector;
    free(a->vector);
    free(pVector);
    pVector = NULL;
}

int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL)
    {
        return FAIL;
    }
    Vector *v = (Vector *) pVector;
    Vector *max = (Vector *) pMaxVector;
    if (max->vector == NULL)
    {
        max->vector = (double *) malloc(sizeof(double) * v->len);
        if (max->vector == NULL)
        {
            return FAIL;
        }
        memcpy(max->vector, v->vector, sizeof(double) * v->len);
        max->len = v->len;
        return SUCCESS;
    }
    double vNorm = vectorNorm(v->vector, v->len);
    double maxNorm = vectorNorm(max->vector, max->len);
    if (maxNorm < vNorm)
    {
        if (max->len != v->len)
        {
            double *tmp_ptr = (double *) realloc(max->vector, sizeof(double) * v->len);
            if (tmp_ptr == NULL)
            {
                return FAIL;
            }
            max->vector = tmp_ptr;
        }
        memcpy(max->vector, v->vector, sizeof(double) * v->len);
        max->len = v->len;
    }
    return SUCCESS;
}

double vectorNorm(const double *vector, int len)
{
    double norm = 0.0;
    for (int i = 0; i < len; i++)
    {
        norm += pow(vector[i], 2);
    }
    return sqrt(norm);
}

Vector *findMaxNormVectorInTree(RBTree *tree)
{
    forEachFunc func = copyIfNormIsLarger;
    Vector *maxVector = (Vector *) malloc(sizeof(Vector));
    if (maxVector == NULL)
    {
        return NULL;
    }
    maxVector->len = 0;
    maxVector->vector = NULL;
    int failOrNah = forEachRBTree(tree, func, (void *) maxVector);
    if (failOrNah == FAIL)
    {
        free(maxVector->vector);
        free(maxVector);
        return NULL;
    }
    return maxVector;
}
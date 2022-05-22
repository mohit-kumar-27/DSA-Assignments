#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct wordInfo
{
    char word[21];
    struct llnode *lineNos;
};
struct llnode
{
    int data;
    struct llnode *next;
};
struct node
{
    struct wordInfo Word;
    struct node *left;
    struct node *right;
};

struct node *tree;
struct node *wordsDel;

int findLength(struct llnode *);
struct llnode *insertll(struct llnode *, int);
void displayll(struct llnode *);
struct node *insertDL(struct node *, struct wordInfo);
void create_tree(struct node *);
struct node *insertWord(struct node *, struct wordInfo);
void inorderTraversal(struct node *);
struct node *findMax(struct node *);
struct node *deleteWord(struct node *, struct wordInfo);
struct node *deleteNodes(struct node *, struct node *);
struct node *checkNodes(struct node *, struct node *);

struct node *insertDL(struct node *start, struct wordInfo data)
{
    struct node *ptr, *newnode;
    newnode = (struct node *)malloc(sizeof(struct node));
    newnode->Word = data;
    newnode->right = NULL;
    newnode->left = NULL;
    if (start == NULL)
    {
        start = newnode;
    }
    else
    {
        ptr = start;
        while (ptr->right != NULL)
            ptr = ptr->right;

        ptr->right = newnode;
    }
    return start;
}
struct llnode *insertll(struct llnode *start, int data)
{
    struct llnode *ptr, *newnode;
    newnode = (struct llnode *)malloc(sizeof(struct llnode));
    newnode->data = data;
    newnode->next = NULL;
    if (start == NULL)
    {
        start = newnode;
    }
    else
    {
        ptr = start;
        while (ptr->next != NULL)
            ptr = ptr->next;
        if (ptr->data != data)
            ptr->next = newnode;
    }
    return start;
}
void displayll(struct llnode *start)
{
    struct llnode *ptr = start;
    while (ptr != NULL)
    {
        printf("%d", ptr->data);
        if (ptr->next != NULL)
            printf(",");
        else
            printf("\n");
        ptr = ptr->next;
    }
}
struct node *checkNodes(struct node *root, struct node *wd)
{
    struct node *curr, *pre;
    curr = root;
    while (curr != NULL)
    {
        if (curr->left == NULL)
        {
            if (findLength(curr->Word.lineNos) < 3)
            {
                wd = insertDL(wd, curr->Word);
            }
            curr = curr->right;
        }
        else
        {
            pre = curr->left;
            while (pre->right != NULL && pre->right != curr)
                pre = pre->right;
            if (pre->right == NULL)
            {
                pre->right = curr;
                curr = curr->left;
            }
            else
            {
                pre->right = NULL;
                if (findLength(curr->Word.lineNos) < 3)
                {
                    wd = insertDL(wd, curr->Word);
                }
                curr = curr->right;
            }
        }
    }
    return wd;
}
struct node *deleteNodes(struct node *tree, struct node *wd)
{
    struct node *ptr = wd;
    while (ptr != NULL)
    {

        tree = deleteWord(tree, ptr->Word);
        ptr = ptr->right;
    }
    return tree;
}
struct node *deleteWord(struct node *root, struct wordInfo w)
{
    struct node *t;
    if (root == NULL)
        return root;
    else if (strcmp(w.word, root->Word.word) < 0)
        root->left = deleteWord(root->left, w);
    else if (strcmp(w.word, root->Word.word) > 0)
        root->right = deleteWord(root->right, w);
    else
    {
        if (root->left == NULL)
        {
            struct node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct node *temp = root->left;
            free(root);
            return temp;
        }

        t = findMax(root->left);
        root->Word.lineNos = t->Word.lineNos;

        strcpy(root->Word.word, t->Word.word);
        root->left = deleteWord(root->left, root->Word);
    }

    return root;
}
int findLength(struct llnode *start)
{
    int i = 0;
    struct llnode *ptr = start;
    while (ptr != NULL)
    {
        i++;
        ptr = ptr->next;
    }
    return i;
}
struct node *findMax(struct node *tree)
{
    if (tree == NULL)
        return NULL;
    struct node *t = tree;
    while (t->right != NULL)
        t = t->right;
    return t;
}
void create_tree(struct node *tree)
{
    tree = NULL;
    wordsDel = NULL;
}
struct node *insertWord(struct node *tree, struct wordInfo w)
{
    if (tree == NULL)
    {
        tree = (struct node *)malloc(sizeof(struct node));
        tree->Word = w;
        tree->left = NULL;
        tree->right = NULL;
    }
    else
    {
        if (strcmp(w.word, tree->Word.word) < 0)
            tree->left = insertWord(tree->left, w);
        else if (strcmp(w.word, tree->Word.word) > 0)
            tree->right = insertWord(tree->right, w);
        else
        {
            tree->Word.lineNos = insertll(tree->Word.lineNos, w.lineNos->data);
        }
    }
    return tree;
}
void inorderTraversal(struct node *tree)
{
    if (tree != NULL)
    {
        inorderTraversal(tree->left);
        for (int j = 0; tree->Word.word[j] != '\0'; j++)
            printf("%c", tree->Word.word[j]);
        printf(" ");
        displayll(tree->Word.lineNos);
        inorderTraversal(tree->right);
    }
}
int main(int argc, char **argv)
{
    char ch;

    create_tree(tree);
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    int j = 0;
    int lineNo = 1;
    while ((ch = fgetc(fp)) != EOF)
    {
        char word[100];

        if (ch >= 'a' && ch <= 'z')
        {
            word[j] = ch;
            j++;
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            word[j] = ch + 32;
            j++;
        }
        else if (ch == '-')
        {
        }
        else
        {
            word[j] = '\0';
            int len;
            if (j < 20)
                len = j + 1;
            else
                len = 20;
            j = 0;
            if (len < 4)
            {
            }
            else
            {
                struct wordInfo w;
                strncpy(w.word, word, len);
                w.lineNos = NULL;
                w.lineNos = insertll(w.lineNos, lineNo);
                tree = insertWord(tree, w);
            }
        }
        if (ch == '\n')
        {
            lineNo++;
            j = 0;
        }
    }
    wordsDel = checkNodes(tree, wordsDel);
    tree = deleteNodes(tree, wordsDel);
    inorderTraversal(tree);
    fclose(fp);
    return 0;
}

DSA Assignment 2     
February 28, 2022
Mohit Kumar
SR No.- 04-01-03-10-51-21-1-19825
M.Tech in Artificial Intelligence
Index
----------------------------------------------------------------------------------------------------------------
struct wordInfo is a structure for storing words along with line number information
struct llnode is a structure for nodes corresponding to line numbers
struct node is a structure for nodes of the index binary search tree
----------------------------------------------------------------------------------------------------------------
A description of the functions used for implementing index are as follows:
int findLength(struct llnode *);
This function finds the length of the linked list of line numbers corresponding to a word

struct llnode *insertll(struct llnode *, int);
This function inserts line numbers in the linked list of line numbers 

void displayll(struct llnode *);
This function prints the linked list of line numbers

struct node *insertDL(struct node *, struct wordInfo);
This function inserts words to be deleted in a linked list 

void create_tree(struct node *);
This function initializes the binary search tree and the linked list of words to be deleted

struct node *insertWord(struct node *, struct wordInfo);
This function inserts a word into the binary search tree

void inorderTraversal(struct node *);
This function traverses the binary search tree in inorder mode

struct node *findMax(struct node *);
This function finds the maximum element in a binary search tree

struct node *deleteWord(struct node *, struct wordInfo);
This function deletes a word from the binary search tree

struct node *checkNodes(struct node *, struct node *);
This function traverses the binary search tree and inserts the words with line number count less than 3 into a linked list

struct node *deleteNodes(struct node *, struct node *);
This function traverses the linked list of words to be deleted and deletes them one by one from the binary search tree

------------------------------------------------------------------------------------------------------------------
Example of input and output
.\A3 poem.txt
and 1,6,8,11
that 2,9,13
the 4,7,11

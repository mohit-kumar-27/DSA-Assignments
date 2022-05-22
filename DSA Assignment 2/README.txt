DSA Assignment 2     
January 28, 2022
Mohit Kumar
SR No.- 04-01-03-10-51-21-1-19825
M.Tech in Artificial Intelligence
Large Integer Arithmetic
----------------------------------------------------------------------------------------------------------------
node is a structure datatype for storing a DIGIT from a number system with base 1,000.
list is a structure datatype for representing large unsigned integers with DIGIT values in [0, 999].
----------------------------------------------------------------------------------------------------------------
A description of the functions used for implementing large integer arithmetic are as follows:

void create_list(list *num);
This function initializes a singly linked list num for storing large unsigned integers.

void insert_beg(list *num, int digit);
This function inserts a node with value digit at the beginning of the list num.

void print_number(list *num);
This function traverses the list num and outputs a large unsigned integer to stdout as comma separated DIGITs
in decimal, ordered on decreasing significance and terminated by a $.

void append(list *num, int digit);
This function inserts a node with value digit at the end of the list num.

int get_digit(list *num, int pos);
This function returns the DIGIT at position pos in the list num from the beginning of the list.

int read_line(char *expr, int len);
This function is used to read large integer infix expressions from stdin.

int check_zero(list *num);
This function checks if the integer represented by the list num is 0.

list *add_numbers(list *num1, list *num2);
This function adds the two integers represented by lists num1 and num2 and returns the sum as a list.

list *multiply_numbers(list *num1, list *num2);
This function multiplies the two integers represented by lists num1 and num2 and returns the product as a list.

list *evaluate_infix(char *expr);
This function evaluates the input expression and returns the list representing the integer result.

void delete_list(list *num);
This functions deletes the list num from memory.
------------------------------------------------------------------------------------------------------------------
Examples of input and output
For examples of input format see the README file
Enter = to terminate the program
Enter infix string for evaluation
111,041,411,111,011$ + 222,222$ * 003$ =
0,333,124,233,999,699$
111,041,411,111,011$ + 222,222$ * 222,222$ =
24,675,844,509,293,703,726$
111,041,548,411,111,011$ + 222,222$ * 222,222$ =
24,675,874,971,63,293,703,726$
111,041,548,411,111,011$ + 222,222$ * 222,222$ + 548,987,852,001,000$ =
24,675,875,520,51,145,704,726$
111,041,548,411,111,011$ * 222,222$ * 222,222$ * 548,987,852,001,000$ =
3,10,387,122,183,681,911,615,690,972,861,472,962,124,0$
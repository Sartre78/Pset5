#include <stdio.h>
#include <cs50.h>

typedef struct node
{
    int n;
    struct node* next;
}
node;

bool insert_node (int value, node* list)
{
    // create a new node
    node* new_node = malloc(sizeof(node));
    if (new_node == NULL)
    {
        return false;
    }
    new_node->n = value; 
      
    // create curr and prev pointers
    node* curr_point = list;
    node* prev_point = NULL;
    
    // for an empty list
    if (head == NULL)
    {
        head = new_node;
        new_node->next = NULL;
        return true;
    }
    
    // go through list
    while (curr_point != NULL)
    {
    
        // if value < this node
        if (value < curr_point->n)
        {  
            // insert before
            
            if (prev_point == NULL)
            {
                head = new_node;
            }
            else
            {
                prev_point->next = new_node;
            }
            new_node->next = curr_point;
            
        // if value > this node
        if (value > curr_point->n)
        {
            // go to next node
            prev_point = cur_point;
            curr_point = curr_point->next;
            
            // update pointers
            
        }
            
        // if value == node
        if (value == curr_point->n)
        {
        
            // free new node
            free(new_node);
            
            // return false
            return false;
    }
    
    // must be at end of list
    new_node->next = NULL;
    prev_point = new_node;   
    return true;
}

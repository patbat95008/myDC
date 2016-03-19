// Patrick Russell pcrussel
// $Id: stack.c,v 1.105 2014-03-02 16:05:22-08 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "debug.h"

#define DEFAULT_CAPACITY 16

struct stack {
   size_t capacity;
   size_t size;
   stack_item *data;
};


stack *new_stack (void) {
   stack *this = malloc (sizeof (stack));
   assert (this != NULL);
   this->capacity = DEFAULT_CAPACITY;
   this->size = 0;
   this->data = calloc (this->capacity, sizeof (stack_item));
   assert (this->data != NULL);
   return this;
}

void free_stack (stack *this) {
   assert (empty_stack (this));
   free (this->data);
   free (this);
}

static bool full_stack (stack *this) {
   return this->size == this->capacity;
}

static void realloc_stack (stack *this) {
   size_t old_capacity = this->capacity;
   this->capacity *= 2;
   this->data = realloc (this->data, this->capacity);
   memset (this->data + old_capacity, 0, old_capacity);
   assert (this->data != NULL);
}


//push an item onto the front of the stack
void push_stack (stack *this, stack_item item) {
   if (full_stack (this)) realloc_stack (this);
   size_t location =  this->size;
   this->data[location] = item;
   this->size++;
}

//pops the first item off of the stack
stack_item pop_stack (stack *this) {
   assert (! empty_stack (this));
   size_t top = this->size - 1;
   stack_item return_this = this->data[top];
   this->size--;
   return return_this;
}

//prints out the item located in the given index
stack_item peek_stack (stack *this, size_t index) {
   assert (index < size_stack(this)); 
   return this->data[index];
}

stack_item first_item (stack *this){
   assert (size_stack(this) > 0);
   return this->data[size_stack(this) - 1];
}

bool empty_stack (stack *this) {
   return size_stack (this) == 0;
}

size_t size_stack (stack *this) {
   return this->size;
}

void show_stack (stack *this) {
   fprintf (stderr, "stack@%p->{%lu,%lu,%p}\n",
            this, this->capacity, this->size, this->data);
}


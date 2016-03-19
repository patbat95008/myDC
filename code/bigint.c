// Patrick Russell pcrussel
// $Id: bigint.c,v 1.129 2014-03-03 11:05:25-08 - - $

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};


static void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   return this;
}


bigint *new_string_bigint (char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   return this;
}


//adds the 2 items given, called by add_bigint
static bigint *do_add (bigint *this, bigint *that) {
   bigint *result = NULL;
   size_t count = 0;
   int carry = 0;

   if(this->size > that->size){
      result = new_bigint(this->size + 1);
   }else if(this->size < that->size){
      result = new_bigint(that->size + 1);
   }else{
      result = new_bigint(this->size + 1);
   }

   for (; count < result->capacity - 1; count++){
      int this_digits;
      int that_digits;

      if(this->size < count) this_digits = 0;
      else this_digits = (int)this->digits[count];
      
      if(that->size < count) that_digits = 0;
      else that_digits = (int)that->digits[count];
      int digit = this_digits + that_digits + carry;
      
      result->digits[count] = (char)digit%10;
      carry = digit / 10;
      result->size++;
   }
   if(this->negative || that->negative) result->negative = true;
   return result;
}

//subtracts the 2 given, called by sub_bigint
static bigint *do_sub (bigint *this, bigint *that) {
   bigint *result = new_bigint(this->size);
   size_t count = 0;
   int borrow = 0;

   for(; count < result->capacity - 1; count++){
      int this_digits;
      int that_digits;
      if(this->size <= count) this_digits = 0;
      else this_digits = (int)this->digits[count];
      
      if(that->size <= count) that_digits = 0;
      else that_digits = (int)that->digits[count];

      int digit = this_digits - that_digits - borrow + 10;
      result->digits[count] = (char)digit%10;
      borrow = 1 - digit / 10;
      result->size++;
   }
   //trim_zeros(result);
   return result;
}

void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

//prints the bigint
void print_bigint (bigint *this, FILE *file) { 
   if(this->negative) fprintf(file, "-");   
   int index = (int)this->size - 1;
   for(; index > 0; index--){
      fprintf(file, "%i", this->digits[index]);
   }
   fprintf(file, "%i\n", this->digits[0]);
}

//adds the 2 items given, returns the result
bigint *add_bigint (bigint *this, bigint *that) {
   bigint *return_item = NULL; 
   if (this->negative && !that->negative){
      return_item = do_sub(that, this);
   }else if(!this->negative && that->negative){
      return_item = do_sub(this, that);
   }else{
      return_item = do_add(this, that);
   }
   assert(return_item != NULL);
   return return_item;
}

//subtracts the 2 items given, returns result
bigint *sub_bigint (bigint *this, bigint *that) {
   bigint *return_item = NULL;
   if(this->negative && !that->negative){
      return_item = do_add(this, that);
   }else if(!this->negative && that->negative){
      return_item = do_add(this, that);
   }else{
      return_item = do_sub(this, that);
   }
   assert(return_item != NULL);
   return return_item;
}


//multiplies the 2 items given, returns result
bigint *mul_bigint (bigint *this, bigint *that) {
   int carry = 0, decimal = 0;
   size_t index = 0, j_index = 0, bigger = 0, smaller = 0;
   size_t super_size = this->size + that->size;
   int *product = calloc(super_size, sizeof(int));
   bigint *result = new_bigint(super_size);

   if(this->negative && !that->negative) result->negative = true;
   else if(that->negative && !this->negative) result->negative = true;
   else result->negative = false;

   if(this->size > that->size){
      bigger = this->size;
      smaller = that->size;
   }else if (that->size >= this->size){
      bigger = that->size;
      smaller = this->size;
   }

   for (; index < smaller; index++){
      carry = 0;
      for (; j_index < bigger; j_index++){
         decimal = product[index + j_index] + this->digits[index]
                   * that->digits[j_index] + carry;
         printf("decimal: %i\n", decimal);
         product[index + j_index] = decimal%10;
         carry = decimal/10;
      }
      product[index + smaller] = carry;
   }

   for (index = 0; index < super_size; index++){
      printf("product: %i\n", product[index]);
      result->digits[index] = product[index];
      result->size++;
   }
   trim_zeros(result);

   return result;
   
}

void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}


#include "allocator.h"
#include <stdio.h>
#include <assert.h>

#define BUFFER_SIZE 1024
byte buffer[BUFFER_SIZE];

void test_allocator_init() {
  allocator alloc;
  assert(allocator_init(&alloc, buffer, BUFFER_SIZE) == OK);
  assert(allocator_init(&alloc, buffer, 8) == ERR_NOT_ENOUGH_SIZE);
}

void test_allocator_my_alloc() {
  allocator alloc;
  allocator_init(&alloc, buffer, BUFFER_SIZE);
  assert(my_alloc(&alloc, 1).out_of_memory == false);
  assert(my_alloc(&alloc, 2).out_of_memory == false);
  assert(my_alloc(&alloc, 3).out_of_memory == false);
  allocator_print(&alloc);
}

int main() {
  test_allocator_init();
  test_allocator_my_alloc();
  printf("\nAll tests passed\n");
}

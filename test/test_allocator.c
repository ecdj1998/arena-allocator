#include "allocator.h"
#include <stdio.h>
#include <assert.h>

#define BUFFER_SIZE 1024
byte buffer[BUFFER_SIZE];

void test_allocator_init() {
  allocator alloc;
  allocator_init(&alloc, buffer, BUFFER_SIZE);
}

void test_allocator_my_alloc() {
  allocator alloc;
  allocator_init(&alloc, buffer, 8);
}

int main() {
  test_allocator_init();
  printf("\nAll tests passed\n");
}

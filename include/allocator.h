// allocator.h
#ifndef ALLOCATOR
#define ALLOCATOR
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

typedef unsigned char byte;
typedef unsigned int uint;

#define BLOCK_SIZE 32 // fixed arena block size
#define BYTES_TO_BLOCKS(bytes)                                                 \
  (bytes / BLOCK_SIZE + (bytes % BLOCK_SIZE > 0 ? 1 : 0))

typedef struct arena_chunk {
  size_t length; // in blocks
  struct arena_chunk *next;
  struct arena_chunk *prev;
  bool in_use;
} arena_chunk_header;

typedef struct {
  arena_chunk_header *list; // pointer to the chunk partition
  void *memory;
  size_t capacity; // in chunks
} allocator;

typedef allocator *allocator_t;

typedef struct {
  bool out_of_memory;
  void *ptr;
} my_alloc_return_t;

typedef enum {
  OK = 0,
  ERR_NOT_ENOUGH_SIZE = 1  
} allocator_init_return_t;

/*	Interface	*/
allocator_init_return_t allocator_init(allocator_t alloc, void *mem, size_t size);

my_alloc_return_t my_alloc(allocator_t alloc, size_t bytes);

void my_free(allocator_t alloc, void *ptr);
#endif

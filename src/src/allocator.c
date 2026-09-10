// allocator.c

#include "allocator.h"
#include <stdio.h>

// Here goes a print function that prints a visual representation
// of the chunk list layout
void allocator_print(allocator_t alloc) {
  if (!alloc || !alloc->list) {
    printf("[ Empty Allocator ]\n");
    return;
  }

  printf("=== Allocator Layout (%zu blocks total) ===\n", alloc->capacity);
  arena_chunk_header *curr = alloc->list;
  size_t idx = 0;

  while (curr) {
    printf("[%zu] %s | Blocks: %4zu (%6zu B) | Addr: %p\n",
           idx++,
           curr->in_use ? "USED" : "FREE",
           curr->length,
           curr->length * BLOCK_SIZE,
           (void *)curr);
    curr = curr->next;
  }
  printf("===========================================\n");
}

allocator_init_return_t
allocator_init(allocator_t alloc, void *mem, size_t size) {
  alloc->memory = mem;
  alloc->capacity =
      (size_t)((size - sizeof(arena_chunk_header)) / BLOCK_SIZE);
  *((arena_chunk_header *)mem) =
      (arena_chunk_header){alloc->capacity, NULL, NULL, false};
  alloc->list = (arena_chunk_header *)mem;
}

my_alloc_return_t
my_alloc(allocator_t alloc, size_t bytes) {
  arena_chunk_header *chunk = alloc->list;
  size_t required_blocks = BYTES_TO_BLOCKS(bytes);

  while (chunk != NULL) {
    // Find the first suitable chunk
    if (chunk->in_use && chunk->length <= required_blocks) {
      chunk = chunk->next;
      continue;
    }
    
    // Determine start of new block
    byte *ptr = (byte *)chunk + sizeof(arena_chunk_header);

    // if start of new block is not end of chunk
    if (bytes <= chunk->length * BLOCK_SIZE &&
        bytes >= (chunk->length * BLOCK_SIZE - sizeof(arena_chunk_header))) {
      // create split node
      byte *new_node = ptr + chunk->length * BLOCK_SIZE - bytes;
      size_t leftover_blocks =
          BYTES_TO_BLOCKS((chunk->length - required_blocks) * BLOCK_SIZE -
                          sizeof(arena_chunk_header));
      *((arena_chunk_header *)new_node) = (arena_chunk_header){
          leftover_blocks, chunk, chunk->next, false};
    }

    // Mark chunk as in use
    chunk->in_use = true;
    return (my_alloc_return_t){false, ptr};
  }

  return (my_alloc_return_t){true, NULL};
}

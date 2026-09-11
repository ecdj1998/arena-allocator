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
  if(size < 2 * BLOCK_SIZE) {
	return ERR_NOT_ENOUGH_SIZE;
  }

  // Assign memory region
  alloc->memory = mem;

  // Calculate capacity in blocks
  alloc->capacity = (size_t)(size / BLOCK_SIZE) - 1;

  // Write header at first block
  *((arena_chunk_header *)mem) = (arena_chunk_header){alloc->capacity, NULL, NULL, false};

  // Assign chunk list
  alloc->list = (arena_chunk_header *)mem;

  return OK;
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
    
    // Determine start of data block
    byte *data = (byte *)chunk + BLOCK_SIZE;

    // if there are at least two leftover blocks
    if (chunk->length - required_blocks >= 2) {

      // Calculate new node position
      arena_chunk_header* split_node = (arena_chunk_header*)data + required_blocks * BLOCK_SIZE;

      // Write header
      size_t split_node_blocks = chunk->length - required_blocks - 1;
     *((arena_chunk_header *)split_node) = (arena_chunk_header){split_node_blocks, chunk->next, chunk, false};

      // Discount chunk length
      chunk->length -= split_node_blocks + 1;
      
      // Append split node to list
      arena_chunk_header* next = chunk->next;
      chunk->next = split_node;
      if(next) next->prev = split_node;
    }
    
    // Mark chunk as in use
    chunk->in_use = true;
    return (my_alloc_return_t){false, data};
  }

  return (my_alloc_return_t){true, NULL};
}

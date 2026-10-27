#include <stdlib.h>
#include <string.h>

#include "obstack.h"

void obstack_init(struct obstack *obstack, size_t chunk_size) {
  obstack->chunk_size = chunk_size;
  obstack->chunk = NULL;
  obstack->object_base = NULL;
  obstack->next_free = NULL;
  obstack->chunk_limit = NULL;
  obstack->alignment_mask = 7; // 8-byte alignment
}

static int obstack_chunk_alloc(struct obstack *obstack) {
  // Allokiere neuen Chunk
  size_t chunk_header_size = sizeof(struct _obstack_chunk);
  size_t total_size = chunk_header_size + obstack->chunk_size;
  struct _obstack_chunk *new_chunk = malloc(total_size);
  if (!new_chunk) {
    return 0; // Allokation fehlgeschlagen
  }

  // Initialisiere den neuen Chunk
  new_chunk->limit = (char *)new_chunk + total_size;
  new_chunk->prev = obstack->chunk;
  obstack->chunk = new_chunk;
  obstack->next_free = (char *)new_chunk->contents;
  obstack->chunk_limit = new_chunk->limit;
  obstack->object_base = obstack->next_free;

  return 1; // Erfolgreich
}

void *obstack_alloc(struct obstack *obstack, size_t size) {
  // Prüfe, ob der aktuelle Chunk genug Platz hat
  if (obstack->next_free + size > obstack->chunk_limit) {
    // Nicht genug Platz: Allokiere neuen Chunk
    if (!obstack_chunk_alloc(obstack)) {
      return NULL; // Allokation fehlgeschlagen
    }
  }

  // Allokiere das Objekt im aktuellen Chunk
  void *object = obstack->next_free;
  obstack->next_free += size;

  // Ausrichtung prüfen
  size_t alignment = obstack->alignment_mask + 1;
  if ((size_t)obstack->next_free & obstack->alignment_mask) {
    obstack->next_free = (char *)(((size_t)obstack->next_free + alignment) &
                                  ~obstack->alignment_mask);
  }

  return object;
}

void obstack_free(struct obstack *obstack) {
  struct _obstack_chunk *chunk = obstack->chunk;
  while (chunk != NULL) {
    struct _obstack_chunk *prev = chunk->prev;
    free(chunk);
    chunk = prev;
  }
  obstack->chunk = NULL;
  obstack->next_free = NULL;
  obstack->chunk_limit = NULL;
  obstack->object_base = NULL;
}

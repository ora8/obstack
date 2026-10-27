#pragma once

// Struktur für einen Chunk (Speicherblock)
struct _obstack_chunk {
    char *limit;                  // 1 past end of this chunk
    struct _obstack_chunk *prev;  // address of prior chunk or NULL
    char contents[];              // flexible array for objects
};

// Struktur für den Obstack
struct obstack {
    size_t chunk_size;            // preferred size to allocate chunks in
    struct _obstack_chunk *chunk; // address of current chunk
    char *object_base;            // address of object we are building
    char *next_free;              // where to add next char to current object
    char *chunk_limit;            // address of char after current chunk
    int alignment_mask;           // Mask of alignment for each object
};

void obstack_init(struct obstack *obstack, size_t chunk_size);
void *obstack_alloc(struct obstack *obstack, size_t size);
void obstack_free(struct obstack *obstack);
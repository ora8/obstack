#pragma once

#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Struktur für einen Chunk (Speicherblock)
typedef struct obstack_chunk {
    char *limit;                  // 1 past end of this chunk
    struct obstack_chunk *prev;   // address of prior chunk or NULL
    char contents[];              // Flexibles Array (C99)
} obstack_chunk;

// Allokiert einen neuen Chunk
obstack_chunk *obstack_chunk_new(size_t size);

// Gibt einen Chunk frei
void obstack_chunk_delete(obstack_chunk *chunk);

// Klasse für den obstack
class obstack {
private:
  size_t chunk_size_;    // preferred size to allocate chunks in
  obstack_chunk *chunk_; // address of current chunk
  char *object_base_;    // address of object we are building
  char *next_free_;      // where to add next char to current object
  char *chunk_limit_;    // address of char after current chunk
  size_t alignment_mask_;   // Mask of alignment for each object

  // Funktionen zum Allokieren und Freigeben von Chunks
  using chunk_alloc_func =
      std::function<obstack_chunk *(void *extra_arg, size_t size)>;
  using chunk_free_func =
      std::function<void(void *extra_arg, obstack_chunk *chunk)>;

  chunk_alloc_func chunk_alloc_func_;
  chunk_free_func chunk_free_func_;
  void *extra_arg_;
  bool use_extra_arg_;

  // Allokiert einen neuen Chunk
  bool allocate_new_chunk();

public:
  // Konstruktor
  obstack(
      size_t chunk_size,
      chunk_alloc_func chunk_alloc_func =
          [](void *, size_t size) { return obstack_chunk_new(size); },
      chunk_free_func chunk_free_func =
          [](void *, obstack_chunk *chunk) { 
            obstack_chunk_delete(chunk); },
      void *extra_arg = nullptr)
      : chunk_size_(chunk_size), chunk_(nullptr), object_base_(nullptr),
        next_free_(nullptr), chunk_limit_(nullptr),
        alignment_mask_(sizeof(size_t)  - 1), // 8-byte alignment
        chunk_alloc_func_(chunk_alloc_func), chunk_free_func_(chunk_free_func),
        extra_arg_(extra_arg), use_extra_arg_(extra_arg != nullptr) {}

  ~obstack() { reset(); }

  // Allokiert ein neues Objekt im obstack
  void *alloc(size_t size);

  double *alloc_double(const double value)
  {
    auto r = static_cast<double *>(alloc(sizeof(double)));
    *r = value;
    return r;
  }

  int *alloc_int(const int value)
  {
    auto r = static_cast<int *>(alloc(sizeof(int)));
    *r = value;
    return r;
  }

  char *alloc_str(const char * const value)
  {
    auto r = static_cast<char *>(alloc(strlen(value) + 1));
    strcpy(r, value);
    return r;
  }

  // Zurücksetzen des obstacks (gibt alle Objekte frei)
  void reset();

  // Kopieren und Zuweisen sind nicht erlaubt
  obstack(const obstack &) = delete;
  obstack &operator=(const obstack &) = delete;
};

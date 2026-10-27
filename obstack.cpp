#include <print>

#include "obstack.hpp"

// Allokiert einen neuen Chunk
obstack_chunk *obstack_chunk_new(size_t size) {
    // Allokiere Speicher für den Header + Inhalt
    obstack_chunk *chunk = reinterpret_cast<obstack_chunk *>(new char[sizeof(obstack_chunk) + size]);
    if (!chunk) {
        return NULL;
    }

    // Initialisiere den Chunk
    chunk->limit = (char *)chunk + sizeof(obstack_chunk) + size;
    chunk->prev = NULL;

    return chunk;
}

// Gibt einen Chunk frei
void obstack_chunk_delete(obstack_chunk *chunk) {
    delete [](reinterpret_cast<char *>(chunk));
}

// Allokiert ein neues Objekt im obstack
void *obstack::alloc(size_t size) {
  // Prüfe, ob der aktuelle Chunk genug Platz hat
  if (next_free_ + size > chunk_limit_) {
    if (!allocate_new_chunk()) {
      return nullptr; // Allokation fehlgeschlagen
    }
  }

  // Allokiere das Objekt im aktuellen Chunk
  void *object = next_free_;
  next_free_ += size;

  // Ausrichtung prüfen
  auto alignment = alignment_mask_ + 1;
  
  if (reinterpret_cast<size_t>(next_free_) & alignment_mask_) {
    next_free_ = reinterpret_cast<char *>(reinterpret_cast<size_t>(next_free_ + alignment) & ~alignment_mask_);
  }

  return object;
}

bool obstack::allocate_new_chunk() {
  auto size = chunk_size_ + sizeof(obstack_chunk);
  obstack_chunk *new_chunk = chunk_alloc_func_(extra_arg_, size);
  if (!new_chunk) {
    return false; // Allokation fehlgeschlagen
  }
  new_chunk->prev = chunk_;
  chunk_ = new_chunk;
  next_free_ = chunk_->contents;
  chunk_limit_ = chunk_->limit;
  object_base_ = next_free_;

  return true;
}

// Zurücksetzen des obstacks (gibt alle Objekte frei)
void obstack::reset() {
  obstack_chunk *chunk = chunk_;
  while (chunk != nullptr) {
    obstack_chunk *prev = chunk->prev;
    chunk_free_func_(extra_arg_, chunk);
    chunk = prev;
  }
  chunk_ = nullptr;
  next_free_ = nullptr;
  chunk_limit_ = nullptr;
  object_base_ = nullptr;
}

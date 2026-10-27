#include <cstring>
#include <print>

#include "obstack.hpp"

int main() {
    // Benutzerdefinierte Allokationsfunktion (optional)
    auto custom_alloc = [](void* extra_arg, size_t size) -> obstack_chunk* {
        std::println("Alloziere neuen Chunk mit Größe {}", size);
        return obstack_chunk_new(size);
    };

    // Benutzerdefinierte Freigabefunktion (optional)
    auto custom_free = [](void* extra_arg, obstack_chunk* chunk) {
        std::println("Freigabe eines Chunks");
        obstack_chunk_delete(chunk);
        std::println("Chunk freigegeben");
    };

    // Erstelle einen Obstack mit benutzerdefinierten Funktionen
    obstack obstack1(1024, custom_alloc, custom_free);

    // Allokiere einige Objekte
    auto x = obstack1.alloc_int(42);

    auto str = obstack1.alloc_str("Hello, Obstack!");

    auto y = obstack1.alloc_double( 3.14159);

    // Gib die Werte aus
    std::println("x = {}", *x);
    std::println("str = {}", str);
    std::println("y = {}", *y);

    // Der Obstack wird automatisch zurückgesetzt, wenn er zerstört wird
    // (aufgrund des Destruktors)

    auto f = 1.f;
    std::println("f = {}", f);
    return 0;
}

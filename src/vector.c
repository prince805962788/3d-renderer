#include "vector.h"

vec2_t vec2_add(vec2_t a, vec2_t b) {
    return (vec2_t){a.x + b.x, a.y + b.y};
}

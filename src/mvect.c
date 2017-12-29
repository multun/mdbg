#include "mvect.h"
#include "alloc.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>


void mvect_init(s_mvect *vect, size_t cap)
{
    vect->size = 0;
    vect->capacity = cap;
    vect->data = xmalloc(cap);
}


void mvect_push(s_mvect *vect, size_t size, const void *data)
{
    // TODO: constant time formula
    bool cap_changed = false;
    while (vect->size + size > vect->capacity)
    {
        if (vect->capacity)
            vect->capacity *= 2;
        else
            vect->capacity += 8;
        cap_changed = true;
    }

    if (cap_changed)
        vect->data = xrealloc(vect->data, vect->capacity);

    memcpy(vect->data + vect->size, data, size);
    vect->size += size;
}


void *mvect_pop(s_mvect *vect, size_t size)
{
    // because we return the data we pop, we can't shrink the vector right after
    // TODO: constant time formula
    bool cap_changed = false;
    while (vect->size < vect->capacity / 2)
    {
        vect->capacity /= 2;
        cap_changed = true;
    }

    if (cap_changed)
        vect->data = xrealloc(vect->data, vect->capacity);

    assert(vect->size >= size);
    vect->size -= size;
    return vect->data + vect->size;
}

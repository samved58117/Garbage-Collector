#ifndef GC_H
#define GC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration
typedef struct GCObject GCObject;

// Type for destruction function
typedef void (*DestructorFunc)(GCObject* obj);

// Basic object structure with reference count
struct GCObject {
    size_t refCount;         // Reference count
    size_t size;             // Size of the object
    DestructorFunc destructor; // Function to call before freeing
    char data[];             // Flexible array member for object data
};

// Functions for memory management
GCObject* gc_allocate(size_t size, DestructorFunc destructor);
void gc_incref(GCObject* obj);
void gc_decref(GCObject* obj);
void* gc_get_data(GCObject* obj);

// Debug utility
void gc_print_stats(GCObject* obj);

#endif // GC_H
#include "include/gc.h"

// Global statistics
static size_t total_allocations = 0;
static size_t total_frees = 0;
static size_t live_objects = 0;

// Allocate an object with initial reference count of 1
GCObject* gc_allocate(size_t size, DestructorFunc destructor) {
    GCObject* obj = (GCObject*)malloc(sizeof(GCObject) + size);
    if (!obj) {
        fprintf(stderr, "Failed to allocate memory\n");
        return NULL;
    }
    
    obj->refCount = 1;
    obj->size = size;
    obj->destructor = destructor;
    
    // Initialize data to zero using memset
    memset(obj->data, 0, size);
    
    total_allocations++;
    live_objects++;
    
    return obj;
}

// Increment reference count
void gc_incref(GCObject* obj) {
    if (obj) {
        obj->refCount++;
    }
}

// Decrement reference count and possibly free object
void gc_decref(GCObject* obj) {
    if (!obj) return;
    
    if (obj->refCount <= 0) {
        fprintf(stderr, "Invalid reference count, already zero or negative\n");
        return;
    }
    
    obj->refCount--;
    
    if (obj->refCount == 0) {
        // Call destructor if provided
        if (obj->destructor) {
            obj->destructor(obj);
        }
        
        // Clear memory before freeing (not strictly necessary but can help catch use-after-free bugs)
        memset(obj->data, 0xDD, obj->size);
        free(obj);
        
        total_frees++;
        live_objects--;
    }
}

// Helper function to get the data portion
void* gc_get_data(GCObject* obj) {
    return obj ? obj->data : NULL;
}

// Print object statistics
void gc_print_stats(GCObject* obj) {
    if (!obj) {
        printf("Object is NULL\n");
        return;
    }
    
    printf("Object %p:\n", (void*)obj);
    printf("  Reference count: %zu\n", obj->refCount);
    printf("  Size: %zu bytes\n", obj->size);
    printf("  Has destructor: %s\n", obj->destructor ? "yes" : "no");
    
    printf("\nGlobal statistics:\n");
    printf("  Total allocations: %zu\n", total_allocations);
    printf("  Total frees: %zu\n", total_frees);
    printf("  Live objects: %zu\n", live_objects);
}

// Getter functions for statistics
size_t gc_get_total_allocations(void) {
    return total_allocations;
}

size_t gc_get_total_frees(void) {
    return total_frees;
}

size_t gc_get_live_objects(void) {
    return live_objects;
}
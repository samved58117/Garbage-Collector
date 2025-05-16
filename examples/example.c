#include "include/gc.h"

// Declare external variables
extern size_t total_allocations;
extern size_t total_frees;
extern size_t live_objects;

// Example complex object with references to other objects
typedef struct {
    char* name;
    GCObject* reference1;
    GCObject* reference2;
} TestObject;

// Destructor for string objects
void string_destructor(GCObject* obj) {
    printf("Destroying string: %s\n", (char*)gc_get_data(obj));
}

// Destructor for test objects
void test_object_destructor(GCObject* obj) {
    TestObject* test = (TestObject*)gc_get_data(obj);
    printf("Destroying test object: %s\n", test->name);
    
    // Decrement references to contained objects
    if (test->reference1) {
        gc_decref(test->reference1);
    }
    if (test->reference2) {
        gc_decref(test->reference2);
    }
}

int main() {
    // Create a string object
    GCObject* str1 = gc_allocate(strlen("Hello, World!") + 1, string_destructor);
    strcpy((char*)gc_get_data(str1), "Hello, World!");
    
    // Create another string
    GCObject* str2 = gc_allocate(strlen("Reference counting!") + 1, string_destructor);
    strcpy((char*)gc_get_data(str2), "Reference counting!");
    
    // Create a complex object
    GCObject* obj = gc_allocate(sizeof(TestObject), test_object_destructor);
    TestObject* test = (TestObject*)gc_get_data(obj);
    test->name = "Test Object";
    
    // Establish references
    test->reference1 = str1;
    gc_incref(str1); // Increment reference count for str1
    
    test->reference2 = str2;
    gc_incref(str2); // Increment reference count for str2
    
    // Print object stats
    gc_print_stats(obj);
    gc_print_stats(str1);
    gc_print_stats(str2);
    
    // Release our initial reference to str1, but it won't be freed because obj still references it
    gc_decref(str1);
    printf("After decref on str1:\n");
    gc_print_stats(str1);
    
    // Release our reference to the complex object, which should also release references to str1 and str2
    gc_decref(obj);
    
    // Release our reference to str2, it should now be freed
    gc_decref(str2);
    
    printf("\nFinal global statistics:\n");
    printf("  Total allocations: %zu\n", gc_get_total_allocations());
    printf("  Total frees: %zu\n", gc_get_total_frees());
    printf("  Live objects: %zu\n", gc_get_live_objects());
    
    return 0;
}
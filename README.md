# Garbage Collector

A simple reference-counting garbage collector implementation in C.

## Overview

This library provides a reference counting garbage collection mechanism for C programs. It helps manage memory automatically by tracking references to objects and freeing them when they're no longer in use.

## Features

- Reference counting for memory management
- Custom destructors for different object types
- Memory usage tracking and statistics
- Simple API for memory allocation and reference handling

## Building

To build the library and examples:

```bash
mkdir build
cd build
cmake ..
make
```

To install the library:

```bash
make install
```

## Usage

```c
#include "gc.h"

// Define a destructor for your objects
void my_object_destructor(GCObject* obj) {
    MyObject* data = (MyObject*)gc_get_data(obj);
    // Clean up resources as needed
}

int main() {
    // Allocate a new garbage-collected object
    GCObject* obj = gc_allocate(sizeof(MyObject), my_object_destructor);
    
    // Get the data pointer to work with the object
    MyObject* data = (MyObject*)gc_get_data(obj);
    
    // When creating references between objects, increment the reference count
    obj2->reference = obj1;
    gc_incref(obj1);
    
    // When done with an object, decrement its reference count
    gc_decref(obj);
    
    return 0;
}
```

## API Reference

- `gc_allocate(size_t size, gc_destructor destructor)`: Allocate a new tracked object
- `gc_get_data(GCObject* obj)`: Get the data pointer from an object
- `gc_incref(GCObject* obj)`: Increment the reference count
- `gc_decref(GCObject* obj)`: Decrement the reference count, potentially freeing the object
- `gc_print_stats(GCObject* obj)`: Print statistics about an object
- `gc_get_total_allocations()`: Get the total number of allocations
- `gc_get_total_frees()`: Get the total number of freed objects
- `gc_get_live_objects()`: Get the current number of live objects
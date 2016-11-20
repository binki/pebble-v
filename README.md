[![Build Status](https://travis-ci.org/binki/pebble-v.svg?branch=master)](https://travis-ci.org/binki/pebble-v)

# Synopsis

    #include <pebble-v/v.h>

# Example

## Variables

    // Define the record we want to store as a struct:
    typedef struct {
      int id;
      const char *name;
    } Thing;
    
    // A thing you want to be able to store in the vector.
    // It is up to you how exactly you want to store it,
    // but it is recommended that if you have a seldom-changing
    // list of data that you store it as a value instead of
    // storing pointers.
    Thing thing = {
      .id = 1,
      .name = "Cat",
    };
    
    // Vector state is stored on the stack. However, you should
    // treat the contents of the struct as opaque as only the
    // documented API functions will be preserved as new versions
    // of the library are released. Note that behavior is undefined
    // if different units of code access the same vector instance
    // using different versions of the library.
    V myVector;

## Initialization

    // We decided to store the record itself rather than a
    // pointer to it in the vector. Thus, initialize
    // myVector with that information. Don’t forget to arrange
    // your code in such a way that you ensure you call
    // v_deinit() later. Note that you are responsible for
    // allocating the myVector struct itself. As we are here,
    // declaring it it as a local is an option. You might also
    // declare it as a static or global.
    v_init(&myVector, sizeof(thing));

## Add

    // Add the thing. Copies slice_size bytes (second parameter
    // of v_init()) pointed to by the second parameter into the
    // vector. For our case, the contents of thing are now in
    // the vector.
    v_add(&myVector, &thing);

## Get

    // You can get an item by index. Here we access the name
    // of the first item in the vector.
    const char *name = ((Thing *)v_get(&myVector, 0))->name;

## Compact

    // This library uses a strategy of increasing the capacity
    // of the backing array more than necessary each time a call
    // to v_add() requires more capacity. If you know that the
    // vector’s size will remaing constant for a while, you
    // should compact it to reclaim memory. Do this for fairly
    // static data but do not bother for one-off vectors.
    v_compact(&myVector);

## Find

    // If you have a strcmp()-like comparer for your struct,
    // you can use v_find() to find the first match. To find
    // the position of the thing we inserted earlier, we could
    // use this (see below for an example thingcmp implementation).
    int i = v_find(&myVector, thingcmp, &thing);
    // i is now 0.
    
    // If nothing in the vector matches, -1 is returned:
    thing.id = 2;
    i = v_find(&myVector, thingcmp, &thing);
    // i is now -1.

### Comparers

Example of a cmp-like function for `Thing`:

    static int thingcmp(void *a, void *b) {
      // Leave void * in the function signature to avoid compiler
      // warnings. If optimizations are on, the next lines should
      // compile to no-ops.
      Thing *thingA = a, *thingB = b;
    
      if (a->id != b->id) {
        return a->id - b->id;
      }
      return strcmp(a->name, b->name);
    }

You may also implement a cmp-like function to search for a particular
field. However, mind that the API is documented such that you may not
assume that the sought item is either a or b, so you must mark that
yourself in the record or make a case for this being defined
behavior. However, for something as simple as searching for a
particular record, you do not need to know which of a or b is the
needle or the haystack:

    static int thing_name_cmp(void *a, void *b) {
      Thing *thingA = a, *thingB = b;
    
      return strcmp(a->name, b->name);
    }

## Iteration

    // To iterate, simply use a usual for loop with v_count() as
    // the upper bound and v_get() within the loop body.
    for (int j = 0; j < v_count(&myVector); j++) {
      Thing *thingPtr = v_get(&myVector, j);
      // Do something with thingPtr now…
    }

## Remove

    // You may remove things from the list at any index.
    v_remove(&myVector, 0);

## Deinitialization

    // Do not forget to call v_deinit() on each vector when you
    // are done with it. Note that any resources referenced by
    // members of the vector are your responsibility. You might
    // want to use an iteration pattern to clean up individual
    // entries first.
    v_deinit(&myVector);

# API

Please see [`pebble-v/v.h`](include/v.h) for details.

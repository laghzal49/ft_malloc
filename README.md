# ft_malloc - A C Garbage Collector

A custom thread-safe memory allocator with garbage collection capabilities for C.

## 📖 Overview

`ft_malloc` is a lightweight memory management library and garbage collector designed to simplify dynamic memory allocation in C. It wraps standard `malloc` and `free` calls in a doubly-linked list structure, allowing for:
- **Automatic Tracking**: Every allocation is tracked in a global list.
- **Garbage Collection**: `free_all()` allows you to clean up all allocations at once, acting as a simple but effective garbage collector.
- **Thread Safety**: Built-in mutex locks ensure safe concurrent access from multiple threads.
- **Aligned Memory**: Returns perfectly aligned memory for safe usage with all standard types.

## ✨ Features

- ✅ **Garbage Collector**: Instantly clean up all tracked memory with `free_all()`.
- ✅ **Thread-Safe**: Uses `pthread_mutex_t` to protect the allocation list.
- ✅ **Custom Allocator**: `ft_malloc` and `ft_free` replace standard functions.
- ✅ **Zero External Dependencies**: Only requires standard C libraries and pthreads.

## 🛠️ Compilation

To compile the library with your project, include `ft_malloc.h` and link with `-lpthread`.

```bash
gcc -o my_program main.c ft_malloc.c -lpthread
```

## 🚀 Usage

### Basic Allocation

```c
#include "ft_malloc.h"

int main() {
    void *ptr = ft_malloc(1024);
    if (!ptr) {
        // Handle error
        return 1;
    }

    // Use ptr...

    ft_free(ptr);
    return 0;
}
```

### Garbage Collection

```c
#include "ft_malloc.h"

int main() {
    void *a = ft_malloc(100);
    void *b = ft_malloc(200);
    void *c = ft_malloc(300);

    // Do work...

    // The Garbage Collector frees everything at once
    free_all();
    return 0;
}
```

### Multi-threading

```c
#include "ft_malloc.h"
#include <pthread.h>

void *thread_func(void *arg) {
    void *data = ft_malloc(512);
    // Use data safely...
    ft_free(data);
    return NULL;
}
```

## 📚 API Reference

| Function | Description |
|----------|-------------|
| `void *ft_malloc(size_t size)` | Allocates `size` bytes and tracks it in the global list. |
| `void ft_free(void *ptr)` | Frees a specific allocation and removes it from the list. |
| `void free_all(void)` | Frees all tracked allocations and resets the list (Garbage Collector). |

## 🧪 Testing

A multi-threaded test suite is included in the `tests/` directory.

```bash
gcc -o test tests/main.c ft_malloc.c -lpthread
./test
```

The test spawns 5 threads, each performing 100 allocations and frees, verifying memory integrity and thread safety.

## 👤 Author

- **tlaghzal** - [42 Student](https://profile.intra.42.fr)
## 📄 License

This project is part of the 42 School curriculum.

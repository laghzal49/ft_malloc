# ft_malloc

> A lightweight garbage collector for C — by [tlaghzal](https://github.com/tlaghzal)

Tracks heap allocations in a doubly linked list embedded directly inside each allocation. No separate bookkeeping, no global variables, one `free` per block.

---

## Memory layout

Every `ft_malloc(size)` call allocates a single contiguous block:

```
┌─────────────────────┬──────────────────┐
│   t_list_gc header  │   user data      │
│   (prev · next)     │   (size bytes)   │
└─────────────────────┴──────────────────┘
▲                      ▲
node                   node + 1  ← returned to caller
```

The header sits immediately before the user data. `ft_free` and `ft_panic` recover it with `(t_list_gc *)ptr - 1`.

---

## API

```c
void  *ft_malloc(size_t size);   // allocate and track
void   ft_free(void *ptr);       // unlink and free one allocation
void   free_all(void);           // free every tracked allocation
void   ft_panic(void *ptr);      // free ptr + all allocations, exit(1)
```

---

## Usage

```c
int main(void) {
    char *buf = ft_malloc(64);
    if (!buf)
        ft_panic(NULL);

    int *arr = ft_malloc(10 * sizeof(int));
    if (!arr)
        ft_panic(buf);   // ft_free(buf) + free_all() + exit(1)

    // ... use buf and arr ...

    ft_free(buf);   // early individual free
    free_all();     // clean up the rest
    return 0;
}
```

---

## How it works

### Global list head

```c
t_list_gc **get_manger(void) {
    static t_list_gc *gc = NULL;
    return (&gc);
}
```

The list head is a `static` local — no global variable. The function returns a pointer-to-pointer so callers can mutate the head directly.

### Insertion (prepend, O(1))

```
Before:  gc → [B] → [C] → NULL
After:   gc → [A] → [B] → [C] → NULL
```

```c
node->next = *get_manger();
node->prev = NULL;
if (*get_manger())
    (*get_manger())->prev = node;
*get_manger() = node;
return (node + 1);
```

### Removal (O(1))

```c
t_list_gc *node = (t_list_gc *)ptr - 1;  // recover header

if (node->prev)
    node->prev->next = node->next;
else
    *get_manger() = node->next;           // node was the head

if (node->next)
    node->next->prev = node->prev;

free(node);                               // frees header + user data
```

### Bulk cleanup

```c
t_list_gc *curr = *get_manger();
while (curr) {
    t_list_gc *next = curr->next;   // save before freeing
    free(curr);
    curr = next;
}
*get_manger() = NULL;
```

### Panic

```c
void ft_panic(void *ptr) {
    if (ptr)
        ft_free(ptr);   // NOT raw free() — ptr is node+1, not the block start
    free_all();
    exit(1);
}
```

---

## Design

| Property               | Detail                                                                   |
| ---------------------- | ------------------------------------------------------------------------ |
| **Intrusive list**     | Metadata lives inside the allocation — no extra `malloc` per node        |
| **Single block**       | `sizeof(t_list_gc) + size` in one allocation, one `free` to release both |
| **O(1) insert/remove** | Doubly linked, no traversal needed                                       |
| **No globals**         | Head is a `static` local accessed via returned pointer                   |

---

## Common mistakes

| Mistake                                            | Why it breaks                                             |
| -------------------------------------------------- | --------------------------------------------------------- |
| `free(ptr)` on a `ft_malloc`'d pointer             | `ptr` is `node+1` — not the block start → heap corruption |
| Using `free_all` without saving `next` first       | Use-after-free on next iteration                          |
| Calling raw `free` in `ft_panic` before `free_all` | Double-free on the same node                              |

---

## Header

```c
typedef struct s_list_gc {
    struct s_list_gc *prev;
    struct s_list_gc *next;
} t_list_gc;

t_list_gc **get_manger(void);
void       *ft_malloc(size_t size);
void        ft_free(void *ptr);
void        free_all(void);
void        ft_panic(void *ptr);
```

---

<p align="center">
  <sub>42 School project · tlaghzal</sub>
</p>

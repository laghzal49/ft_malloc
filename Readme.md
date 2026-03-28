GC-Malloc: A Simple C Garbage Collector
A lightweight wrapper for malloc and free that tracks allocations in a linked list, allowing for easy mass-deallocation and leak prevention.

🚀 Features
Centralized Tracking: Every allocation is registered in a static linked list behind the scenes.

Mass Cleanup: Free every byte of allocated memory with a single function call.

Safe Exits: Ensure no memory is leaked even during a crash or forced error exit.

Standard Compatibility: Drop-in replacements for standard memory management functions.

🛠️ Implementation Details
The system uses a Singly Linked List to store the addresses of all active allocations.

The Structure
C
typedef struct s_list_gc {
void *address;
struct s_list_gc *next;
} t_list_gc;
📖 Usage

1. Standard Allocation
   Use ft_malloc exactly like the standard malloc. It returns NULL if either the requested memory or the tracking node fails to allocate.

C
char _str = ft_malloc(sizeof(char) _ 15); 2. Manual Free
If you want to free a specific pointer before the program ends (to keep memory usage low), use ft_free. It unhooks the pointer from the collector and safely frees the memory.

C
ft_free(str); 3. Total Cleanup (No Leaks)
Call free_all() at the very end of your main() function. This acts as a safety net that catches and frees any pointers you forgot to clean up manually.

C
int main(void) {
// ... your program logic ...

    free_all();
    return (0);

} 4. Emergency Exit
If a critical error occurs (like a failed file open or a nested allocation failure), use ft_panic. This will attempt to free the immediate failing pointer, wipe the rest of the tracked memory, and exit the program with status 1.

C
void \*data = ft_malloc(100);
if (!data)
ft_panic(NULL);
⚠️ Performance Notes
This collector prioritizes safety over raw speed.

ft_malloc: O(1) - Very fast (prepends the tracking node to the head of the list).

ft_free: O(n) - Requires searching the list to find the specific pointer to unlink it.

free_all: O(n) - Iterates exactly once through the entire active list.

⚙️ Compilation
Ensure both the header and source files are in your directory, then compile as usual:

Bash
gcc -Wall -Wextra -Werror main.c ft_malloc.c -o my_program

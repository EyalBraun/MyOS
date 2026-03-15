# OrbitOS | High-Performance VFS & Command Dispatch Engine

**OrbitOS** is a specialized Virtual File System (VFS) and command execution engine implemented in C++17. Architected for low-latency environments, it demonstrates advanced systems programming patterns including custom hash-mapped dispatchers, N-ary tree recursion, and strict RAII memory management.

## 🚀 Technical Core & Engineering Patterns

### 1. Constant-Time Dispatch Engine ($O(1)$)
Unlike standard shells that use linear string comparisons, OrbitOS utilizes a **Jump Table Dispatcher**.
* **X-Macros:** Used to generate function prototypes and mapping logic simultaneously, ensuring the command list remains a single source of truth.
* **Custom Hashing:** Implements a localized hash function with linear probing to map command strings to function pointers, mimicking the interrupt vector tables found in real-time kernels.

### 2. Memory Architecture & RAII
The system is built on a **Zero-Leak Guarantee** using modern C++ memory primitives.
* **Ownership Model:** Employs `std::unique_ptr` for child node management. This creates a cascading destruction chain; when a parent node is deallocated, the entire sub-tree is safely purged from the heap.
* **Stateful Navigation:** Maintains a Persistent Context (CWD) via raw pointers (`File*`) to avoid ownership cycles while providing $O(1)$ access to the current environment.

### 3. N-ary Tree & Recursive Logic
The filesystem is modeled as a dynamic N-ary tree, optimized for:
* **DFS Discovery:** Global search operations (`find`, `fc`) utilize Depth-First Search to traverse the hierarchy.
* **Persistence:** Features custom binary serialization for state recovery, allowing the entire tree structure to be written to and read from disk while maintaining pointer integrity.

---

## 📂 Project Structure

```text
OrbitOS/
├── src/            # Core logic (.cpp)
├── include/        # Declarations & X-Macros (.h)
├── tests/          # Automated diagnostic suite
├── data/           # Persistent VFS disk images (Git ignored)
├── build/          # Compiled binaries (Git ignored)
└── Makefile        # Project automation
🛠 Command Set
Navigation: cd, pwd, ls, tree

Manipulation: mkdir, wtf (write to file), rm, cp

System: stats (node counting), save/load (persistence), help

Security: fv (Favorite/Write-protect toggle), showfv

Scripting: Supports semicolon-separated command strings and quoted arguments.

🔧 Building & Testing
OrbitOS uses a centralized Makefile for streamlined development.

1. Build and Run the OS
Bash
make run
2. Run Automated Diagnostic Suite
The test runner executes a silent validation of the hashing engine and filesystem integrity, providing a verbose report only on failure.

Bash
make test
3. Clean Project
Bash
make clean
🛡 Security & Real-Time Track
OrbitOS is developed with a focus on Real-Time Systems. Future iterations include:

Integration of PID Control and Kalman Filter simulation modules for drone flight-data logging.

Multi-user permission layers (Root/User) for file access.

Memory-mapped file I/O for high-speed data retrieval.

Author: Eyal Braun

Environment: Linux (Neovim / G++17)

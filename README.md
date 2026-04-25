// OVERVIEW
This project is the final submission for the Algorithm and Data Structures Laboratory. It serves as a practical implementation of fundamental concepts, featuring a Restaurant Ordering System.
The system allows users to manage restaurant orders, simulating the core logic of a digital restaurant management system through efficient data manipulation and structural organization in a terminal-based environment.

---

// TECH STACK

* Language  | C++ (Standard 17)
* Paradigm  | Procedural Programming
* Logic focus | Data Organization, Searching/Sorting Algorithms, Memory Management, and File Operations.

---

// CORE SYSTEM FEATURES

* Dynamic Order Archive   | Managing a collection of order entries with unique metadata.
* Menu Management         | Displaying and searching menu items based on code or name.
* Structural Operations   | Adding, deleting, and traversing data units.
* Sorting Engine          | Bubble Sort implementation to organize menu by price.
* File Persistence        | Saving completed transactions to an external `.txt` file.
* Terminal-based Interface | A streamlined CLI designed for efficient user interaction and system feedback.

---

// PREREQUISITES
This project requires a compiler that supports the C++17 standard (or later).

* GCC  : 7.0+
* Clang: 5.0+
* MSVC : Visual Studio 2017 (v15.3) or later

---

// Compilation
Use the command relevant to your toolchain:

// GCC / Clang
g++ -std=c++17 main.cpp -o app

// MSVC
cl /std:c++17 main.cpp /Fe:app.exe

---

// PROJECT STRUCTURE

├── main.cpp         # Entry point and core program logic
├── database.sql     # Database schema structure
├── transaksi.txt    # Output file for saved transactions
└── README.md        # Project documentation

---

// CORE CONCEPTS IMPLEMENTED

* Pointer            | Used to construct and traverse linked list nodes.
* Linked List        | Singly linked list for temporary order management.
* Double Linked List | Bidirectional traversal for order data structure.
* Sequential Search  | Linear search for menu lookup by code.
* Bubble Sort        | Sorting menu items by price.
* File Operations    | Writing transaction records to transaksi.txt.
* SQL Database       | Relational schema provided in database.sql.

  ---


// CONTRIBUTORS
Development Team // SI-D

* Rayhan Athiyya Alfarizi |  124250147
* Amalia Aryanti |  124250158

Laboratory Assistants

* Galang Ivandry | 124240042
* uhammad Ikram Mughni | 124240191

---

// ARCHIVAL NOTES
This project demonstrates the ability to bridge theoretical algorithmic concepts with functional C++ implementation. It prioritizes clean code structure and logical flow to ensure system stability and maintainability across all core restaurant management operations.

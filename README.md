# 🔍 File Indexing System

![Language](https://img.shields.io/badge/language-C99-blue.svg)
![Build](https://img.shields.io/badge/build-Makefile-brightgreen.svg)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)

A high-performance **file indexing and keyword retrieval engine** written in C. It organizes documents and associated keywords using a **Trie (multiway retrieval tree)** and a **Max-Heap**, enabling efficient file search, keyword lookups, and top-k relevance ranking. All data structures are built from scratch with manual memory management.

---

## 📋 Table of Contents

- [Features](#-features)
- [Data Structures](#-data-structures)
- [Getting Started](#-getting-started)
- [Input Format](#-input-format)
- [Commands](#-commands)

---

## ✨ Features

- **Multiway Keyword Indexing:** Fast keyword search using a 26-way prefix Trie.
- **Relevance Ranking (TOPK):** Retrieves the most relevant files using an integrated Max-Heap (ordered by score and lexicographically by name).
- **Dynamic File Management:** Insert, update, and remove files along with their keyword associations.
- **Automatic Pruning:** Recursively deletes unused branches in the Trie when a keyword has no remaining references.
- **Lexicographical Output:** Dumps entire index contents in alphabetical order.
- **Manual Memory Management:** Zero memory leaks with fine-grained allocation and deallocation for every structure.

---

## 🧱 Data Structures

| Structure | Implementation | Purpose |
|---|---|---|
| **File List** | Doubly linked list | Stores all registered files with their metadata and keywords |
| **Retrieval Tree (Trie)** | Multiway tree (26 children per node) | Indexes all unique keywords across files |
| **Terminal Heap** | Max-Heap (dynamic array) | Attached to each terminal node in the Trie to rank associated files by relevance |

### Design notes

- **Trie:** Each edge corresponds to a lowercase English letter (`a`–`z`). Terminal nodes mark complete keywords and store an embedded Max-Heap of file pointers.
- **Max-Heap Priority:** Primary key is the file's integer relevance score (`scor`). In case of a tie, names are compared lexicographically (`strcmp`).
- **Pointer-based Referencing:** The Trie nodes do not duplicate file contents or metadata; they reference nodes in the master doubly-linked list.
- **Recursive Cleanup:** When removing a keyword or file, nodes that no longer represent words and have no active children are pruned upward to prevent memory bloat.

---

## 🚀 Getting Started

### Requirements

- GCC (C99 support)
- GNU Make

### Build & Run

```bash
git clone git@github.com:SerbanNicolae/file-indexing-system.git
cd file-indexing-system

make          # build search_index executable
make run      # run (reads indexare.in, writes indexare.out)
make clean    # remove binaries and object files
```

---

## 📥 Input Format

The program reads operations from `indexare.in` and writes results to `indexare.out`.

```
<number_of_operations>
<command> [arguments]
...
```

---

## 🕹 Commands

| Command | Arguments | Description | Output |
|---|---|---|---|
| `ADD` | `<id> <score> <nr_words> <w1> ...` | Inserts a new file and indexes its keywords | `OK` or `EXISTS` |
| `DEL` | `<id>` | Deletes a file and cleans up its keyword references | `OK` or `NOT FOUND` |
| `ADDKW` | `<id> <keyword>` | Associates an additional keyword with an existing file | `OK` or `NOT FOUND` |
| `DELKW` | `<id> <keyword>` | Disassociates a keyword from a file (deletes file if 0 keywords remain) | `OK` or `NOT FOUND` |
| `FIND` | `<keyword>` | Finds all files containing the keyword, sorted lexicographically | `<count> <file1> ...` or `EMPTY` |
| `TOPK` | `<keyword> <k>` | Retrieves the top `k` most relevant files for a keyword | `<count> <file1> ...` or `EMPTY` |
| `PRINT` | – | Prints all indexed keywords and their files in alphabetical order | `<keyword> <count> <files...>` per line |

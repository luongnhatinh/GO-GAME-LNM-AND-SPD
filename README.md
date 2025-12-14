# Go Game (Weiqi) - C++ & Raylib

![Go Game Banner](media/menu.png)

## 📜 Overview

This project is a fully functional implementation of the ancient board game **Go (Cờ Vây)**, built using **C++** and the **Raylib** graphics library. The game features a standard 19x19 board, full rule implementation (capturing, liberties, suicide prevention), and a smart AI opponent utilizing **Minimax** and **Alpha-Beta Pruning** algorithms.

## ✨ Features

### 🎮 Game Modes
* **Player vs. Player (PvP):** Play against a friend on the same device. The game automatically handles turn switching (Black/White).
* **Player vs. AI (PvE):** Challenge the computer with 3 difficulty levels:
    * **Easy:** Makes random moves (good for learning the rules).
    * **Medium:** Uses Minimax (Depth 2) to calculate tactical moves.
    * **Hard:** Uses Alpha-Beta Pruning + Neighbor Pruning (Depth 3+) and heuristic evaluation for a challenging match.

### 🛠 Core Functionality
* **Undo/Redo:** Unlimited undo history to review mistakes or test variations.
* **Save/Load System:** Save your current game state to a file and resume later exactly where you left off.
* **Smart Board Logic:**
    * Automatic capture of dead stones.
    * Suicide move prevention.
    * Territory estimation and scoring.

### 🖥️ Interface
* Clean, wood-themed graphical interface.
* Interactive buttons for Pass, New Game, Save, and Load.
* Visual indicators for the last move and hover effects.

---

## 🚀 Installation & Build

### Prerequisites
* **C++ Compiler** (MinGW, g++, or MSVC) supporting C++17.
* **Raylib Library** (v4.5 or higher).

### Building with g++ (Command Line)
If you have Raylib installed and linked:

```bash
g++ src/*.cpp -o GoGame.exe -O2 -I include -lraylib -lopengl32 -lgdi32 -lwinmm

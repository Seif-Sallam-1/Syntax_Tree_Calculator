# Syntax Tree Calculator 🧮

A powerful C++ educational tool that parses mathematical expressions, constructs an **Abstract Syntax Tree (AST)**, and visualizes the structure interactively using the Qt framework.

*(Replace this link with an actual screenshot of your app later!)*

## 🚀 Features

* **Expression Parsing:** accurate parsing of mathematical expressions using the **Shunting-yard Algorithm**.
* **Visual AST Generation:** Automatically builds and draws the syntax tree node-by-node.
* **Interactive View:**
* **Zoom:** Mouse wheel to zoom in/out of the tree.
* **Pan:** Click and drag to move around large trees.


* **Robust Calculation:** Handles standard operators (`+`, `-`, `*`, `/`) and parentheses with correct Order of Operations (precedence).
* **Smart Validation:** Prevents invalid inputs (like letters or double operators) using Regular Expressions.
* **Error Handling:** Detects and reports errors like "Division by Zero" or malformed expressions.

## 🛠️ Technical Stack

* **Language:** C++17
* **Framework:** Qt 6 (Widgets & Graphics View Framework)
* **Build System:** CMake
* **Key Algorithms:**
* Shunting-yard Algorithm (Infix to Postfix conversion)
* Recursive Tree Traversal (for calculation and rendering)



## 📂 Project Structure

* **`AST.h` / `AST.cpp**`: The core logic engine. Handles tokenization, parsing, tree construction, and memory management.
* **`MainWindow.h`**: The GUI implementation. Manages the window, the zoomable graphics scene, and user input events.
* **`main.cpp`**: Application entry point.

## 📦 Build Instructions

### Prerequisites

* C++ Compiler (GCC, Clang, or MSVC)
* Qt 6 Development Libraries
* CMake (3.14 or higher)

### Steps

1. **Clone the repository:**
```bash
git clone https://github.com/your-username/Syntax-Tree-Calculator.git
cd Syntax-Tree-Calculator

```


2. **Create a build directory:**
```bash
mkdir build
cd build

```


3. **Run CMake and Build:**
```bash
cmake ..
make  # Or 'nmake' on Windows / Open the project in CLion/QtCreator

```


4. **Run the Application:**
```bash
./Syntax_Tree_Calculator

```



## 🎮 How to Use

1. **Enter an Expression:** Type a math problem (e.g., `5 + 3 * (2 - 1)`) into the input field.
2. **Calculate:** Press **`=`** or the Enter key.
3. **Explore:**
* The result appears in the text box.
* The structure of the equation is drawn below as a tree.
* Use the **Scroll Wheel** to zoom in on complex branches.
* **Click and Drag** to move the tree view.


4. **Clear:** Press **`AC`** to reset the screen and memory.

## 🤝 Contributing

1. Fork the project.
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a Pull Request.

## 📜 License

This project is open-source and available under the [MIT License](https://www.google.com/search?q=LICENSE).

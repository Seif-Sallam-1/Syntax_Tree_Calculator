# Syntax Tree Calculator 🧮

A powerful C++ educational tool that parses mathematical expressions, constructs an **Abstract Syntax Tree (AST)**, and visualizes the structure interactively using the Qt framework.

<img width="1919" height="1012" alt="image" src="https://github.com/user-attachments/assets/81d86703-b618-45f0-a963-504cecd7e8a0" />

## 🚀 Features

* **Expression Parsing:** Accurate parsing of mathematical expressions using the **Shunting-yard Algorithm**.
* **Visual AST Generation:** Automatically builds and draws the syntax tree node-by-node.
* **Interactive View:**
    * **Zoom:** Mouse wheel to zoom in/out of the tree.
    * **Pan:** Click and drag to move around large trees.
* **Robust Calculation:** Handles standard operators (`+`, `-`, `*`, `/`) and parentheses with correct Order of Operations (precedence).
* **Smart Validation:** Prevents invalid inputs (like letters or double operators) using Regular Expressions.
* **Error Handling:** Detects and reports errors like "Division by Zero" or malformed expressions.

## 📥 How to Run (No Coding Required)

If you want to use the application without installing Qt or CLion, follow these steps:

1.  Navigate to the **[cmake-build-debug](:\Syntax_Tree_Calculator\cmake-build-debug\Syntax_Tree_Calculator.exe)** section of this repository.
2.  Download the latest `.zip` file (e.g., `SyntaxTreeCalculator_v1.0.zip`).
3.  **Extract (Unzip)** the downloaded folder.
4.  Open the folder and double-click **`Syntax_Tree_Calculator.exe`**.

> **⚠️ Important:** You must keep the `.exe` file inside the folder. It depends on the surrounding DLL files to function correctly. Do not drag the `.exe` to your desktop; create a shortcut instead.

## 🛠️ Technical Stack

* **Language:** C++17
* **Framework:** Qt 6 (Widgets & Graphics View Framework)
* **Build System:** CMake
* **Key Algorithms:**
    * Shunting-yard Algorithm (Infix to Postfix conversion)
    * Recursive Tree Traversal (for calculation and rendering)

## 📂 Project Structure

* **`AST.h` / `AST.cpp`**: The core logic engine. Handles tokenization, parsing, tree construction, and memory management.
* **`MainWindow.h`**: The GUI implementation. Manages the window, the zoomable graphics scene, and user input events.
* **`main.cpp`**: Application entry point.

## 📦 Build from Source (For Developers)

If you want to modify the code or build it yourself:

### Prerequisites
* C++ Compiler (GCC, Clang, or MSVC)
* Qt 6 Development Libraries
* CMake (3.14 or higher)

### Steps
1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/Seif-Sallam-1/Syntax-Tree-Calculator.git](https://github.com/Seif-Sallam-1/Syntax-Tree-Calculator.git)
    cd Syntax-Tree-Calculator
    ```

2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Run CMake and Build:**
    ```bash
    cmake ..
    make  # Or 'nmake' on Windows / Open the project in CLion/QtCreator
    ```

4.  **Run the Application:**
    ```bash
    ./Syntax_Tree_Calculator
    ```

## 🤝 Contributing

1.  Fork the project.
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`).
3.  Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4.  Push to the branch (`git push origin feature/AmazingFeature`).
5.  Open a Pull Request.

## 📜 License

This project is open-source and available under the [MIT License](LICENSE).

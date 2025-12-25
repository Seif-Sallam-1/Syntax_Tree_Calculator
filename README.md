Syntax Tree Calculator 🧮
A powerful C++ educational tool that parses mathematical expressions, constructs an Abstract Syntax Tree (AST), and visualizes the structure interactively using the Qt framework.

(Replace this link with an actual screenshot of your app later!)

🚀 Features
Expression Parsing: accurate parsing of mathematical expressions using the Shunting-yard Algorithm.

Visual AST Generation: Automatically builds and draws the syntax tree node-by-node.

Interactive View:

Zoom: Mouse wheel to zoom in/out of the tree.

Pan: Click and drag to move around large trees.

Robust Calculation: Handles standard operators (+, -, *, /) and parentheses with correct Order of Operations (precedence).

Smart Validation: Prevents invalid inputs (like letters or double operators) using Regular Expressions.

Error Handling: Detects and reports errors like "Division by Zero" or malformed expressions.

🛠️ Technical Stack
Language: C++17

Framework: Qt 6 (Widgets & Graphics View Framework)

Build System: CMake

Key Algorithms:

Shunting-yard Algorithm (Infix to Postfix conversion)

Recursive Tree Traversal (for calculation and rendering)

📂 Project Structure
AST.h / AST.cpp: The core logic engine. Handles tokenization, parsing, tree construction, and memory management.

MainWindow.h: The GUI implementation. Manages the window, the zoomable graphics scene, and user input events.

main.cpp: Application entry point.

📦 Build Instructions
Prerequisites
C++ Compiler (GCC, Clang, or MSVC)

Qt 6 Development Libraries

CMake (3.14 or higher)

Steps
Clone the repository:

Bash

git clone https://github.com/Seif-sallam-1/Syntax-Tree-Calculator.git
cd Syntax-Tree-Calculator
Create a build directory:

Bash

mkdir build
cd build
Run CMake and Build:

Bash

cmake ..
make  # Or 'nmake' on Windows / Open the project in CLion/QtCreator
Run the Application:

Bash

./Syntax_Tree_Calculator
🎮 How to Use
Enter an Expression: Type a math problem (e.g., 5 + 3 * (2 - 1)) into the input field.

Calculate: Press = or the Enter key.

Explore:

The result appears in the text box.

The structure of the equation is drawn below as a tree.

Use the Scroll Wheel to zoom in on complex branches.

Click and Drag to move the tree view.

Clear: Press AC to reset the screen and memory.

🤝 Contributing
Fork the project.

Create your Feature Branch (git checkout -b feature/AmazingFeature).

Commit your changes (git commit -m 'Add some AmazingFeature').

Push to the branch (git push origin feature/AmazingFeature).

Open a Pull Request.

📜 License
This project is open-source and available under the MIT License.

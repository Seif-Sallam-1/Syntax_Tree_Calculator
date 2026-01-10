# Syntax Tree Calculator 🧮

A powerful C++ educational tool that parses mathematical expressions, constructs an **Abstract Syntax Tree (AST)**, and visualizes the structure interactively using the Qt framework.

<div align="center">
  <kbd>
    <img src="https://github.com/user-attachments/assets/30a408ec-bbf9-4457-9e8e-df8a51751ee7" alt="Syntax Tree Calculator Screenshot" width="900">
  </kbd>
</div>

## 🚀 Features

* **Expression Parsing:** Accurate parsing of mathematical expressions using the **Shunting-yard Algorithm** (see `Parser.h`).
* **Visual AST Generation:** Automatically builds and draws the syntax tree node-by-node (see `AST.h`).
* **Interactive View:**
    * **Zoom:** Mouse wheel to zoom in/out of the tree.
    * **Pan:** Click and drag to move around large trees.
* **Robust Calculation:** Handles standard operators (`+`, `-`, `*`, `/`) and parentheses with correct Order of Operations.
* **Smart Validation:** Prevents invalid inputs (letters, double operators) using Regular Expressions.
* **Error Handling:** Detects "Division by Zero" and malformed syntax.

## 🛠️ Technical Stack

* **Language:** C++17
* **Framework:** Qt 6 (Widgets & Graphics View Framework)
* **Build System:** CMake (Cross-platform)
* **Tools:** CLion, Git

---

## 📥 How to Run (No Coding Required)

If you just want to use the calculator without installing compilers:

1.  Navigate to the **Releases** or `cmake-build-debug` folder in this repo.
2.  Download the executable/zip.
3.  **Extract (Unzip)** the folder.
4.  Double-click **`Syntax_Tree_Calculator.exe`**.

> **⚠️ Important:** Keep the `.exe` file inside the folder. It depends on the surrounding DLL files to function.

---

## 📦 Build from Source (For Developers)

### Prerequisites
* **OS:** Windows 10/11 (Linux/Mac supported via CMake but untested)
* **IDE:** CLion (Recommended) or VS Code
* **Compiler:** MSVC or MinGW
* **Qt:** Qt 5 or 6 (Must be installed and linked to CMake)
* **Git:** For version control

### Quick Build & Run (CLion)
1.  Open CLion and select **Open**, pointing to the `CMakeLists.txt` folder.
2.  Configure the CMake profile to point to your Qt kit.
3.  **Build:** (Build → Build Project).
4.  **Run:** (Run → Run 'Syntax_Tree_Calculator').

### Quick Build (Command-Line / PowerShell)
```powershell
# 1. Create build directory
mkdir build; cd build

# 2. Configure (Example using Ninja)
cmake -S .. -B . -G "Ninja" -DCMAKE_BUILD_TYPE=Release

# 3. Build
cmake --build . --config Release

# 4. Run
.\Syntax_Tree_Calculator.exe

```

---

## 🔄 Versioning & Runtime Files

This project uses specific files to manage versioning and user data.

### 1. Version Control (`version.txt`)

* **Purpose:** The single source-of-truth for the app version (e.g., `0.1.0`).
* **Location:** Project root.
* **Usage:** Can be embedded into the binary using `resources.qrc` so the version travels with the executable.
* **How to Bump:** Update the text file manually or use a script (e.g., `scripts/bump-version.ps1`) before building.

### 2. Calculation History (`calc_history.txt`)

* **Purpose:** An append-only log of expressions and results.
* **Location:** By default, this is created in the working directory (where the app runs).
* **Recommendation:** For production builds, this should be excluded from Git and stored in the user's `AppData`.

**Git Ignore Recommendations:**
Ensure your `.gitignore` includes:

```text
calc_history.txt
cmake-build-*
*.exe

```

---

## 🤝 Contributing

Contributions are welcome! Suggested workflow:

1. **Fork** the repository.
2. Create a branch: `git checkout -b feature/your-feature-name`
3. Make changes and test.
4. Commit and push:
```bash
git add .
git commit -m "Add cool feature"
git push origin feature/your-feature-name

```


5. Open a **Pull Request** on GitHub.

## 📜 License

This project is open-source. Please see the [`LICENSE`](https://github.com/Seif-Sallam-1/Syntax_Tree_Calculator/blob/main/LICENSE) file for details.

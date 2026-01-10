# SmartCalculator

SmartCalculator is a small desktop calculator written in C++ using Qt and CMake. It supports parsing and evaluating mathematical expressions via a custom parser and generates an AST for expressions. The project is configured with CMake and is commonly developed with CLion on Windows.

## Features
- Evaluate arithmetic expressions and parentheses
- Custom parser and AST representation (see `AST.h`, `Parser.h`)
- Qt-based GUI (`MainWindow.h`, `main.cpp`)
- CMake build system for cross-platform builds

## Prerequisites
- Windows 10/11
- CLion (recommended) or CMake + a C++ compiler (MSVC, MinGW)
- Qt 5 or 6 (installed and available to CMake/CLion)
- Git (for source control)
- Optional: GitHub CLI (`gh`) for repository tasks

## Quick build & run (CLion)
1. Open CLion and select "Open" then choose the project folder (where `CMakeLists.txt` is).
2. Configure the CMake profile to point to your Qt kit if required.
3. Build the project (Build → Build Project).
4. Run the target from CLion (Run → Run 'SmartCalculator').

## Quick build & run (command-line, PowerShell)
1. Create an out-of-source build directory:
   mkdir build; cd build
2. Configure and generate the build system (example using Ninja):
   cmake -S .. -B . -G "Ninja" -DCMAKE_BUILD_TYPE=Release
3. Build:
   cmake --build . --config Release
4. Run:
   .\SmartCalculator.exe

## Git / GitHub — push this project to your remote
If your GitHub repository is empty (you created https://github.com/Seif-Sallam-1/SmartCalculator), here are the commands to push your local project to that remote. Run these in PowerShell from the project root (where this `README.md` lives).

1) Initialize (if you haven't already) and commit:
   git init
   git add --all
   git commit -m "Initial commit — add project files"

2) Make sure you are using `main` as the primary branch:
   git branch -M main

3) Add the remote (HTTPS):
   git remote add origin https://github.com/Seif-Sallam-1/SmartCalculator.git

   Or add via SSH (if you set up SSH keys on GitHub):
   git remote add origin git@github.com:Seif-Sallam-1/SmartCalculator.git

4) Push and set upstream:
   git push -u origin main

Authentication tips:
- Recommended: run `gh auth login` and follow the interactive flow to authenticate.
- Or create a Personal Access Token (PAT) on GitHub and use it when prompted for your password for HTTPS pushes.

If you prefer, you can create the remote and push in one command using GitHub CLI:
   gh repo create Seif-Sallam-1/SmartCalculator --public --source=. --remote=origin --push

## Repository metadata
After pushing, you can set the repo description and topics (tags) with the GitHub CLI:
   gh repo edit Seif-Sallam-1/SmartCalculator --description "SmartCalculator — Qt/C++ desktop calculator with expression parsing and AST." --add-topic qt --add-topic calculator --add-topic c++ --add-topic cmake

Or edit the "About" section on the repository page in your browser.

## Contributing
Contributions are welcome. Suggested workflow:
1. Fork the repository or create a branch: `git checkout -b feature/your-feature`
2. Make changes, add tests where applicable.
3. Commit and push your branch, then open a pull request.

## License
Add a `LICENSE` file to the project root to declare the project's license (for example MIT).

## Contact
For questions, open an issue on GitHub or contact the maintainer via your GitHub profile.

## Dynamic storage files (runtime / generated files)
This section explains which files the application generates or reads at runtime, where they live, and how to manage them in the repository and in distribution.

Files used by the app
- `version.txt` (optional, recommended): single-line text file containing the app version (e.g. `0.1.0`).
  - Purpose: single source-of-truth for the app version. Can be embedded into the binary using `resources.qrc` (so the version travels with the executable) or kept external next to the executable for runtime editable versions.
  - Location: project root (during development) or embedded as a resource (`:/version.txt`) when packaged.

- `calc_history.txt`: runtime log of calculations saved by the app.
  - Purpose: append-only history of expressions and results. Created in the working directory (same directory where the app runs).
  - Location: by default the app writes `calc_history.txt` into the current working directory (for development runs, this is the project root or the build output folder). For a production installer, prefer storing runtime data in the user's profile (AppData) or a per-user folder.

How to package vs keep external
- Embedded version (recommended for shipped builds): add `version.txt` to `resources.qrc` so it's embedded and accessible at runtime as `:/version.txt`. This guarantees the version inside the binary matches the installed build.
- External version (useful for quick local edits): keep a plain `version.txt` next to the executable and modify it without rebuilding.
- History and user data: do not commit `calc_history.txt` to the repo. Store it per-user (AppData) for installed builds.

Git / .gitignore recommendations
- Add runtime/temporary files to `.gitignore` so they are not accidentally committed. Suggested entries:

```
# runtime / generated
calc_history.txt
cmake-build-*
cmake-build-*/
*/CMakeFiles/
*/Syntax_Tree_Calculator_autogen/
*.exe
```

- Keep `version.txt` tracked if you want it as a source-of-truth and embed it into resources; otherwise add it to `.gitignore` if you prefer it to be edited per-deployment.

How to read and update the files
- Read `:/version.txt` in Qt (if embedded):

```cpp
QFile f(":/version.txt");
if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
  QString version = QString::fromUtf8(f.readAll()).trimmed();
}
```

- Read external `version.txt` next to executable:

```cpp
QFile f(QCoreApplication::applicationDirPath() + "/version.txt");
```

- Clear or move `calc_history.txt`:
  - Delete or open the file with truncate to clear it.
  - For production, modify the code to use `QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)` to save per-user data.

Bumping the version (quick PowerShell example)
- Simple patch bump (script example included in `scripts/bump-version.ps1`):

```powershell
# bump patch
.\scripts\bump-version.ps1
# or bump minor/major
.\scripts\bump-version.ps1 minor
.\scripts\bump-version.ps1 major
```

- After you change `version.txt` (external) or regenerate resources, rebuild the project so the embedded qrc contains the new value.

CI and releases
- Option A — tracked `version.txt`: CI reads `version.txt` to tag releases and build artifacts.
- Option B — tag-driven: use Git tags (v1.2.3) in CI and generate `version.h` or `version.txt` from the tag during the build.

Examples / small changes you may want
- Move `calc_history.txt` to AppData (recommended for installers): modify `saveToHistory()` to write to a path returned by `QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)`.
- Add a `scripts/bump-version.ps1` (already present in this repo) and keep `version.txt` tracked for simple releases.

If you want, I can now:
- Add a short `.gitignore` entry for `calc_history.txt` and common build outputs.
- Update `saveToHistory()` to write to AppData instead of the working directory.
- Convert the repo to track `version.txt` and show a quick release workflow (tagging + CI snippet).

Tell me which of those three follow-ups you want me to implement next, and I'll make the changes and re-run build/tests.

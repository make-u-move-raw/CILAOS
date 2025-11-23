# ⛰️ CILAOS

Cilaos is a simple project displaying mesh terrain with procedural generation using a small custom engine and raylib for visuals and events.

This projet's purpose is to consolidate C++ development fundamental and strengthen project management skills.

If you are intersted in how the project is organized and its used frameworks and libs, refer to the [Developers section](#-developers).

# 🙋‍♂️ Users

This section is for installing / building the app so as to test interactions and see what it looks like.

## <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/5/5f/Windows_logo_-_2012.svg/800px-Windows_logo_-_2012.svg.png " alt="windows" width="24"/> Windows

## <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/Tux.svg/800px-Tux.svg.png" alt="Linux" width="24"/> Linux

## <img src="https://upload.wikimedia.org/wikipedia/commons/1/1b/Apple_logo_grey.svg" alt="MacOs" width="24"/> MacOs

# 👨‍💻 Developers

This section is focused for developers of the project, feel free to change the code and experiment as you please.

### ⚠️ **Prerequisites**

All of these are basic installations for dependencies for the developpers and may vary with the OS you are using.

- Install [CMake](https://cmake.org/download/) if not already on your system
  (For windows users, install [MSys2](https://www.msys2.org/) if not already present on your system)
- _(Optional)_ Install [Doxygen](https://www.doxygen.nl/download.html) and [graphviz](https://graphviz.org/download/) and add it to path for generating doc

---

### ⚙️ **Build & Run**

This project uses CMake to compile and build source code and [CPack](https://cmake.org/download/) to deploy app for different OS.

For all the following commands concerning the app, choose the script to execute dependending on your OS :

#### 🏃 **Quick start**

**Linux and MacOs :** use bash script `run.sh`<br>
**Windows :** use powershell script `run.ps1`

Run the app with `./run.<sh|ps1>` (Can take a while the first time you build the project)

If you are running on windows and you see an error about the rights to execute `run.ps1` run `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser` (more info [here](https://learn.microsoft.com/fr-fr/powershell/module/microsoft.powershell.core/about/about_execution_policies?view=powershell-7.5))


#### 📌 **Specific commands**

You can also run `./run.<sh|ps1> help` to get all available options when running the script with `./run.<sh|ps1> <option>`

---

### 📋 List and versions of libraries

To avoid compatibility issues, make sure the versions of the libraries installed correspond to the one for the project

- [raylib](https://www.raylib.com/) = 5.0
- [CMake](https://cmake.org/download/) Minimum 3.14
- [Catch2](https://github.com/catchorg/Catch2/blob/v2.13.0/single_include/catch2) = 2.13.0

---

### 🧪 Running Tests

This project uses [Catch2](https://github.com/catchorg/Catch2) to test features and every unit test **MUST** pass add any feature. You can run tests with `./run.<sh|ps1> tests` and the results are displayed on terminal.

---

### 📖 Generating Documentation

Documentation is automatically generated with [Doxygen](https://www.doxygen.nl/index.html). After building the project, generate doc with : `./run.<sh|ps1> doc`

The resulting documentation should be located in `CILAOS/docs/html/index.html`

---

### 💻 Development practices

#### 🔄 **CI/CD**

Each pull request go through github actions CI cycle consisting of :

- Build
- Unit tests validation and coverage
- Code quality
- Doc generation

#### 🎯 **Tracking issues**

This project uses **Github issues** to keep track of the development process and custom labels are used for organizing tasks in different types and priority. All the labels purposes are described in the project.

#### 🌿 **Branches and commits**

All new features are merged into `develop`branch and then merged into `main`once a stable realease is available for production.

**main** → stable, production-ready <br>
**develop** → active development

**feature/\<name>** → feature branches <br>
**bugfix/\<name>** → bugfix <br>
**hotfix/\<name>** → hotfix <br>

Commit Messages must be clear, concise, and descriptive and **MUST** refer to a task (github issue ID) using this syntax : `<action> <scope/description>. <Issue reference>`

#### 🎨 **Code formatting and style**

The project use **PascalCasing** for classes and **pascalCasing** for methods and variable names. Also private attributes from a class are named following the "m_underscore" suffix convention i.e `m_privateVar`

The formatting of the code is done via VSCode C++ formatting tools, you can add the following lines to your `settings.json` file :

```json
"[cpp]": {
  "editor.defaultFormatter": "ms-vscode.cpptools",
  "editor.formatOnSave": true
}
```

This practice ensures uniform code throughout the entire project and prevents from committing modified files without changing the code itself

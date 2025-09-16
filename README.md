# ⛰️ CILAOS

Cilaos is a simple project displaying mesh terrain with procedural generation using a small custom engine and raylib for visuals and events.

This projet's purpose is to consolidate C++ development fundamental and strengthen project management skills. 

If you are intersted in how the project is organized and its used frameworks and libs, refer to the [Developers section](#-developers).

# 🙋‍♂️ Users
This section is for installing / building the app so as to test interactions and see what it looks like.

## <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/5/5f/Windows_logo_-_2012.svg/88px-Windows_logo_-_2012.svg.png " alt="windows" width="24"/> Windows

## <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/Tux.svg/800px-Tux.svg.png" alt="Linux" width="24"/> Linux

## <img src="https://upload.wikimedia.org/wikipedia/commons/1/1b/Apple_logo_grey.svg" alt="MacOs" width="24"/> MacOs


# 👨‍💻 Developers
This section is focused for developers of the project. Feel free to change the code as you please even if you are not a one.

### ⚙️ Build & Run
This project uses docker to compile and build source code and [CPack](https://cmake.org/cmake/help/latest/manual/cpack.1.html#manual:cpack(1)) to deploy app for different OS.

If you do not have docker and want to compile / build the project, check out [Docker installation](https://www.docker.com/get-started/) for compatibility.

1. Clone this repository using this command (or use github interface): 
    ```bash
    git clone https://github.com/make-u-move-raw/CILAOS.git
    ```
2. Install dependencies :
3. Build the app :
4. Run the app 


# TODO :

### 📋 List and versions of libraries

- [raylib](https://www.raylib.com/) 
### 🧪 Running Tests
This project uses [TO_DEFINE] to test features and every unit test **MUST** pass to merge feature, you can run tests with : `RUN_TEST_COMMAND`

### 📖 Generating Documentation
Documentation is automatically generated with [TO_DEFINE] use, to generate :#
`GENERATE_DOC_COMMAND`

### 💻 Development practices for the project

#### 🎯 Tracking issues
This project uses **Github issues** to keep track of the development process and custom labels are used for organizing tasks in different types and priority. All the labels purposes are described in the project.

#### 🌿 Branches and commits:
All new features are merged into `develop`branch and then merged into `main`once a stable realease is available for production.

**main** → stable, production-ready <br>
**develop** → active development

**feature/\<name>** → feature branches <br>
**bugfix/\<name>** → bugfix <br>
**hotfix/\<name>** → hotfix <br>

Commit Messages must be clear, concise, and descriptive and **MUST** refer to a task (github issue ID) using this syntax : `<action> <scope/description>. <Issue reference>`

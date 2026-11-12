# Qt Menu and Toolbar Modular Project

A small Qt 6 Widgets application split into reusable header files.

## Requirements

- Qt 6 with the Widgets module
- CMake 3.16 or newer
- A C++17 compiler

On Debian, the Qt development package is commonly installed with:

```bash
sudo apt install build-essential cmake qt6-base-dev
```

## Build

Run these commands from the project root:

```bash
cmake -S . -B build
cmake --build build -j
./build/Qt_Menu_Toolbar_Modular_Project
```

## Header responsibilities

| Header | Responsibility |
|---|---|
| `app_actions.h` | Stores shared menu and action pointers |
| `menu_bar.h` | Creates menus and menu actions |
| `tool_bar.h` | Creates toolbars and reuses menu actions |
| `connection.h` | Connects signals to behavior |
| `tool_tip.h` | Sets tooltips and status tips |
| `enable_disable.h` | Enables/disables File menu and main toolbar |
| `set_shortcut.h` | Assigns keyboard shortcuts |
| `set_icon.h` | Assigns icons to QAction objects |
| `status_bar.h` | Displays status-bar messages |
| `library.h` | Includes all custom project headers |

## Icons

The sample code keeps the resource paths from the original application, for example:

```cpp
":/new/prefix1/icons/ui-tab--plus.png"
```

Those paths work only if the matching images are registered in a Qt resource file (`.qrc`) using the same resource aliases. If you do not add the resource file and images, the actions still work, but their icons will be blank.

## Enable/disable demonstration

- Select **Edit -> Open**: the File menu and Main Toolbar become disabled.
- Select **Edit -> New**: the File menu and Main Toolbar become enabled again.
- The Edit menu and Edit Toolbar remain available so the user can restore the interface.

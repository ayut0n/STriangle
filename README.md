# STriangle

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat&logo=c%2B%2B) ![Build](https://img.shields.io/badge/Build-CMake-064F8C?style=flat&logo=cmake) ![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=flat&logo=windows)[![SFML](https://img.shields.io/badge/SFML-3.0-8CC414?style=flat)](https://www.sfml-dev.org/)

**[Русский](#русский)** | **[English](#english)**

---

<a id="русский"></a>
# Russian

Визуализатор бесконечного фрактала (треугольника Серпинского) с эффектом непрерывного погружения (полета).

### Особенности:

- **GLSL Шейдеры:** Плавный бесконечный полет сквозь структуру треугольника Серпинского в реальном времени на GPU.
- **Меню настроек:** Кастомизация цвета акцента с помощью RGB-слайдеров или сброс на динамическую радугу.
- **Сохранение конфигурации:** Настройки автоматически сохраняются в файл `settings.mlpeg`.
- **Discord Rich Presence:** Интеграция с Discord показывает ваш статус («Looking at Serpinsky triangles»).
- **Полноэкранный режим:** Автоматическое определение разрешения и скрытие курсора.

### Управление:

- `Shift + B` - Открыть / Закрыть меню настроек (показывает курсор и слайдеры)
- `ESC` — Закрыть программу

### Установка:

1. Перейдите по сайту `https://striangle.netlify.app` и скачайте STriangle_Setup.exe
2. Установите программу при помощи Inno Setup
3. Запустите STriangle.exe

### Самостоятельная сборка::

Для самостоятельной сборки программы вам понадобится Visual Studio Code, а также расширение CMake Tools. Для компиляции программы установите MSYS2 MinGW. Для работы программы необходима библиотека SFML.

1. Установите библиотеку SFML при помощи MSYS2 MinGW:
   ```
   pacman -S mingw-w64-x86_64-sfml
   ```
2. Скачайте исходный код программы в ZIP-архиве
3. Распакуйте архив
4. В терминале Visual Studio Code перейдите в место распаковки архива (папку для сборки)
5. Сгенерируйте файлы сборки и скомпилируйте в режиме Release:
```
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
5. Готовый файл .exe появится в папке `build`

---

<a id="english"></a>
## English

An infinite fractal visualizer (the Sierpinski triangle) with a continuous immersion (flight) effect.

### Features:

- **GLSL Shaders:** Smooth, infinite flight through the structure of the Sierpinski triangle in real time on the GPU.
- **Settings Menu:** Customize the accent color using RGB sliders or reset to a dynamic rainbow.
- **Configuration Saving:** Settings are automatically saved to the `settings.mlpeg` file.
- **Discord Rich Presence:** Integration with Discord displays your status (“Looking at Serpinsky triangles”).
- **Full-Screen Mode:** Automatically detects resolution and hides the cursor.

### Controls:

- `Shift + B` - Open / Close the settings menu (shows the cursor and sliders)
- `ESC` — Close the program

### Installation:

1. Go to `https://striangle.netlify.app` and download STriangle_Setup.exe
2. Install the program using Inno Setup
3. Run STriangle.exe

### Building from Source:

To build the program from source, you’ll need Visual Studio Code and the CMake Tools extension. To compile the program, install MSYS2 MinGW. The SFML library is required for the program to run

1. Install the SFML library using MSYS2 MinGW:
```
pacman -S mingw-w64-x86_64-sfml
```
2. Download the program's source code in a ZIP archive
3. Extract the archive
4. In the Visual Studio Code terminal, navigate to the directory where the archive was extracted (the build folder)
5. Generate the build files and compile in Release mode:
```
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
5. The finished .exe file will appear in the `build` folder

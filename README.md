# mtt
MTT - это набор инструментов для создания 3D приложений на c++. По сути, это - набор библиотек, которые могут облегчить вам жизнь, если вы хотите создать свой редактор или визуализатор.

# Что вы сможете здесь найти
Прежде всего, стоит обратить внимание на библиотеку render. Она, по сути, является объетной оберткой вокруг Vulkan API. Тут также реализована механика графа сцены и построения очереди рендера на его основе.
Библиотека clPipeline дополняет библтотеку render конкретными техниками отрисовки, комбинируя прямое и отложенное освещение. Если render - это довольно низкоуровневая и универсальная библиотека, то clPipeline - готовая рендер система, которую вы можете использовать, пока не построите свою(если это вам конечно понадобится).
Библиотека application даст вам такие блага, как рабочий цикл, очередь асинхронных задач, манипуляторы, библиотеки ресурсов, undo/redo и набор часто используемых виджетов.

# Как собрать
На сегодняшний день я работаю на Windows 10 и Visual Studio 2022. Сборка на других платформах вероятно будет связана с некоторыми трудностями. Если вам это удастся, дайте, пожалуйста, знать.

Итак, что вам понадобится:
* C++20 компилятор (я использовал VS2022 и VS2019 до этого)
* CMake 3.8 и выше
* Vulkan SDK(https://www.lunarg.com/vulkan-sdk/)
* Vulkan Memory Allocator(https://gpuopen.com/vulkan-memory-allocator/)
* Библиотека GLM(https://github.com/g-truc/glm)
* Qt5
* Fbx SDK

Скорее всего, вам потребуется указать cmake расположение некоторых библитек. Добавте в переменную окружения CMAKE_PREFIX_PATH путь до cmake папки qt, путь до главной папки FbxSDK, путь до lib папки FbxSDK, путь до glm и vulkan memory allocator. Например так: "C:\Qt\5.15.2\msvc2019_64\lib\cmake;C:\Program Files\Autodesk\FBX\FBX SDK\2020.0.1;C:\Program Files\Autodesk\FBX\FBX SDK\2020.0.1\lib\vs2017\x64;C:\glm;C:\VulkanMemoryAllocator\include".

Для сборки вы можете использовать cmake-gui или собрать из командной строки:

    mkdir build
    cd build
    cmake ..
    cmake --build .

# Что почитать
Прежде всего, посмотрите документ introduction.md. Это краткое введение, там вы найдете общее описание mtt и разбор наиболее важных классов и концепций.

Дальше стоит обратить внимание на примеры, они находятся в папке applications/Examples. Каждый из них снабжен собственным README.md.

Приложения ObjectEditor и ParticlesEditor также можно использовать для изучения mtt, но они уже гораздо сложнее, и пока что не документированы. Собственно, ObjectEditor изначально и планировался как простенький учебный проект, но потом что-то пошло не так.

# Контакты
* email: AluminiumRat@yandex.ru
* telegram: @AluminiumRat

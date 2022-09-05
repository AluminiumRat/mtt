# Кратко
Инициализируем mtt, создаем пустую сцену и рисуем её через RenderWidget.

# Детально
## mtt::Application
Первое, что надо сделать, прежде чем использовать mtt - это создать экземпляр mtt::Application (или EditorApplication, если вы хотите использовать editorLib). Application - это синглтон, который выполняет работу общего характера: инициализирует QApplication и Vulkan, организует рабочий цикл. Кстати, mtt::Application унаследован от QApplication, так что создавать экземпляр QApplication вам не надо.

    mtt::Application application( argc,
                                  argv,
                                  "Mtt empty scene example",
                                  { 0, 0, 0 },
                                  VK_API_VERSION_1_2,
                                  features,
                                  true);

Первые два аргумента - это аргументы командной строки, они будут переданы в конструктор QApplication.

Следующие два аргуметра "Mtt empty scene example" и { 0, 0, 0 } - это название приложения и его версия, которые будет передается в vkCreateInstance при инициализации. Эти значения мало на что влияют, только если кто-то захочет оптимизировать драйверы под вашу игру.

VK_API_VERSION_1_2 - версия API вулкан, выставляйте не ниже 1.2.

features - структура VkPhysicalDeviceFeatures, содержащая возможности вулкана, которые обязательно должны быть включены для вашего приложения. Нам сейчас не требуются какие-то дополнительные возможности, так что передаем пустую структуру.

Последний параметр enableValidation включает(или выключает если false) валидацию со стороны вулкана. Если передать true, то вулкан будет проверять корректность обращений к VulkanAPI и сообщать обо всех ошибках в лог, но производительность при этом заметно просядет. Рекомендуется включать в debug сборке.

## RenderWidget
RenderWidget - это виджет, унаследованный от QWidget, и предназначенный для того, чтобы производить в него отрисовку. Создание виджета тривиально. Здесь он выполняет роль единственного окна приложения.

    mtt::RenderWidget window;
    window.setFixedSize(800, 600);
    window.show();

RenderScene - это, по большому счету, список рисуемых объектов. Так как в этом примере мы ничего не рисуем, то и сцену оставляем пустой.

CameraNode - это объект, который отвечает за то, откуда и под каким углом мы будем видеть нашу сцену, то есть это прямой аналог камеры из телевидения. Положение камеры в сцене мы устанавливается так же, как и для любой mtt::AbstractNode, с помощью метода setTransformMatrix, а углы и дистанции обзора - методом setPerspectiveProjection:

    mtt::CameraNode camera;
    camera.setTransformMatrix(glm::translate(glm::vec3(2.f, 0.f, 1.f)));
    camera.setPerspectiveProjection(glm::pi<float>() / 2.f, 1.33f, 0.1f, 50.f);

Метод setTransformMatrix устанавливает матрицу трансформации из локальной системы координат ноды в мировую. Этот метод позволяет устанавливать не только смещение, но и поворот и масштабирование.

Общим методом для установки матрицы проекции является CameraNode::setProjectionMatrix. Но мы воспользуемся методом setPerspectiveProjection - оберткой вокруг setProjectionMatrix. Агрументы этого метода аналогичны аргументам glm::perspective. Вообще, вызов setPerspectiveProjection эвкивалентен вызову

  camera.setProjectionMatrix(CameraNode::projectionCorrect *
                                  glm::perspective(fovY, aspect, zNear, zFar));

CameraNode::projectionCorrect - это корректировка проекционной матрицы, связанная с инверсией оси Y в экранных координатах Vulkan(по сравнению с OpenGL), а также с использованием техики reversed-Z.

Сцена и камера устанавливаются методом RenderWidget::setSource:

    window.setSource(&scene, &camera);

Кроме сцены и камеры, рендер виджет требует также предоставить ему фрэйм билдер. Фрэйм билдер - это объект, который отвечает за создание фрэйм буфера, а также за правильное чередование проходов рендера. Например, если используется отложенное освещение, то именно он отвечает за то, что сначала будет выполнена запись в G-буфер, а после этого будет отрисовываться освещение в цветовой буфер. В примере мы используем clPipeline::ColorFrameBuilder.

    mtt::clPipeline::ColorFrameBuilder colorFrameBuilder(
                                                    window.surfaceFormat(),
                                                    application.displayDevice());
    window.setFrameBuilder(&colorFrameBuilder);
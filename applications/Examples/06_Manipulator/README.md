# Кратко
Создаем собственный манипулятор и используем его для перемещения объекта по сцене.

# Детально
## Сцена
Подготовим сцену, прежде чем делать манипулятор. Здесь всё просто и уже знакомо:

    class Scene : public mtt::RenderScene
    {
    public:
      mtt::clPipeline::AmbientLight ambientLight;
      mtt::MasterDrawModel dinoModel;
      mtt::MasterDrawModel fieldModel;

ambientLight - освещение сцены, dinoModel - модель, которую мы будем двигать по сцене, fieldModel - неподвижная поверхность, нужна только для того, чтобы видеть перемещение dinoModel в пространстве.

## Манипулятор
К сожалению mtt не предоставляет универсальные манипуляторы на все случаи жизни. Не получится взять готовое решение и просто подключить его. Можно, конечно, воспользоваться манипуляторами из editorLib, но тогда необходимо привязываться к объектной модели этой библиотеки.

Тем не менее mtt предоставляет абстрактные классы манипуляторов, которые выполняют большую часть работы. Нам нужно только приспособить их под наши нужды. Унаследуемся от класса mtt::PlaneMove3DManipulator, он реализует логику перемещения в плоскости:

    class Manipulator : public mtt::PlaneMove3DManipulator
    {
    private:
      mtt::AbstractNode& _movableObject;
    public:
      Manipulator(mtt::AbstractNode& movableObject) :
        PlaneMove3DManipulator(mtt::AutoscaleDrawableModificator::PIXEL_SCALE_MODE),
        _movableObject(movableObject)

Обратите внимание на значение AutoscaleDrawableModificator::PIXEL_SCALE_MODE, которое мы передаем в конструктор PlaneMove3DManipulator. Оно означает, что манипулятор будет иметь фиксированный размер на экране, а координаты точек, составляющих геометрию, будут заданы в пикселях.

Первое, что нам надо сделать - это установить геометрию манипулятора, то есть форму, в виде которой он будет рисоваться, и с которой будет производиться проверка пересечения указателем мыши.

    const glm::vec3 p1(-20.f, -20.f, 0.f);
    const glm::vec3 p2(-20.f,  20.f, 0.f);
    const glm::vec3 p3( 20.f,  20.f, 0.f);
    const glm::vec3 p4( 20.f, -20.f, 0.f);
    std::vector<glm::vec3> positions{ p1, p2, p3,
                                      p1, p3, p4,
                                      p1, p3, p2,
                                      p1, p4, p3};
    setGeometry(positions);

Геометрия манипулятора всегда устанавливается в топологии VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST. В нашем случае геометрия - это квадрат размером в 40 пикселей. Мы использовали 4 треугольника вместо 2 для того чтобы избавиться от фэйс кулинга(Face culling). Правильнее было бы отключить его в технике рисования, но clPipeline::UnlightedColorTechnique не поддерживает эту возможность.

Установка техники рисования, полагаю, не вызовет никаких вопросов.

Всё, что нам осталось сделать в конструкторе - это переместить манипулятор туда, где находится объект:

    node().setTransformMatrix(_movableObject.transformMatrix());

Манипуляторы сами по себе не являются нодами. Вместо этого они вольны выстраивать собственные произвольные иерархии нод как им заблагорассудится, но каждый манипулятор обязан предоставить доступ к корневой ноде с помощью виртуального метода Manipulator3D::node().

Последнее, что необходимо сделать, чтобы получить полноценный манипулятор - это связать логику работы PlaneMove3DManipulator с перемещаемым объектом. Для этого переопределим виртуальный метод PlaneMove3DManipulator::processShift

    virtual void processShift(const glm::vec3& startTouchPoint,
                              const glm::vec3& shiftedPoint) noexcept override
    {
      _movableObject.setTransformMatrix(glm::translate(shiftedPoint));
      node().setTransformMatrix(_movableObject.transformMatrix());
    }

Этот метод вызывается каждый раз, когда пользователь пытается перетащить манипулятор. Так как наш объект и манипулятор находятся в одной системе координат, то мы просто перемещаем объект с манипулятором в нужную точку.

## Контроллер манипулятора
Манипуляторы подключаются к рендер-виджету через специальный адаптер mtt::ManipulatorController:

    Manipulator manipulator(scene.dinoModel);

    mtt::ManipulatorController manipulatorController(window);
    manipulatorController.setManipulator(&manipulator);

Контроллер манипулятора добавляет дравэйблы манипулятора в рендер-сцену, а также перехватывает часть GUI событий рендер-виджета и преобразовывает их в команды манипулятору.
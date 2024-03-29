# Кратко
Объединяем объектную модель с рендером. Используем виджеты для управления свойствами объекта.

# Детально
## Класс Dino
В этом классе мы связываем между собой данные и рендер. Для этого Dino наследуется и от QObject, и от mtt::MasterDrawModel. Решение, конечно, не самое удобное, если мы хотим строить сколь-нибудь сложное приложение, но вполне рабочее для нашего примера.

Итак, класс Dino является мастер моделью, и его уже можно встраивать в рендер-сцену. Для того чтобы мы смогли управлять им через виджеты mtt, мы должны добавить в него свойства. Обладает свойством, если он имеет некоторое внутреннее состояние, геттер для этого состояния, сеттер и сигнал об изменении состояния. В нашем случае это свойство position - положение модели вдоль оси Y.

## Рендер-сцена
Построение рендер-сцены в целом такое же, как и в предыдущих примерах. Из интересного здесь только применение теней в clPipeline::DirectLight. Этот источник света поддерживает каскадные карты теней, поэтому мы должны установить размер каскада:

    directLight.setCascadeSize(3);

Кроме того, DirectLight имеет бесконечную область действия, поэтому мы должны установить дистанцию от наблюдателя, до которой будут действовать тени:

    directLight.setShadowDistance(50.f);

## Виджеты управления
Окно управления заполняется в функции fillWindow, и именно здесь находятся главные нововведения.

Для управления свойствами через виджеты есть два подхода - это использование специализированных виджетов и использование соединений(connections) между объектом и стандартными виджетами Qt.

Слайдер демонстрирует первый подход:

    mtt::FloatPropertyLinearSlider<Dino>* slider =
              new mtt::FloatPropertyLinearSlider<Dino>( scene.dinoModel,
                                                        &Dino::position,
                                                        &Dino::setPosition,
                                                        &Dino::positionChanged,
                                                        undoStack);

Обратите внимание, что виджеты не управляют свойствами напрямую через геттеры и сеттеры, вместо этого они создают команды и помещают их в стек команд, поэтому при создании виджета мы должны передать в конструктор ссылку на mtt::UndoStack.

Второй подход показан на примере спин бокса:

    QDoubleSpinBox* spin = new QDoubleSpinBox;
    ...
    mtt::FloatSpinConnection<Dino>* spinConnction =
                    new mtt::FloatSpinConnection<Dino>( *spin,
                                                        scene.dinoModel,
                                                        &Dino::position,
                                                        &Dino::setPosition,
                                                        &Dino::positionChanged,
                                                        undoStack);

Здесь мы создаем внешний к спинбоксу объект - соединение, которое и занимается синхронизацией объекта и UI. Этот подход удобнее, когда вы используете Qt designer для создания своих элементов управления.
# Кратко
Используем слэйв-модели.

# Детально
Слэйв-модели - это модели, которые могут использовать общие меши и анимации. Этот подход может значительно сократить затраты памяти на повторяющиеся данные. Роль общего ресурса играет мастер-модель, поэтому первое, что нам необходимо - это загрузить её:

    std::shared_ptr<mtt::MasterDrawModel> masterModel(
                                  new mtt::MasterDrawModel( ":/Dino.mmd",
                                                            techniquesFactory,
                                                            nullptr,
                                                            displayDevice));

Обратите внимание, что мы используем shared_ptr. Это позволяет не беспокоиться о времени жизни мастер-модели. Она будет уничтожена вместе с последней слэйв-моделью, которая её использует.

После того как мастер-модель загружена, мы создаем в цикле 4 слэйв-модели, мастер-модель при этом передается в конструктор:

    for (int modelIndex = 0; modelIndex < 4; modelIndex++)
    {
      std::unique_ptr <mtt::SlaveDrawModel> newModel(
                                          new mtt::SlaveDrawModel(masterModel));

Каждая слэйв-модель использует меши мастер-модели, а так же поддерживает все её анимации, но при этом имеет собственную иерархию Joint-нод, поэтому к каждой модели может быть применена своя собственная анимация:

    const mtt::DrawModelAnimation& animation = newModel->animation(
                                modelIndex % newModel->animationsNumber());
    newModel->updateFromAnimation(animation,
                                  animation.timeRange().length() / 2);
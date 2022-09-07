# Кратко
Добавляем тени к clPipeline::PointLight

# Детально
Большая часть кода знакома вам по предыдущим примерам. Здесь нет ни новых классов, ни каких-либо структурных изменений. Разница заключается только в настройках.

Сцена состоит из двух моделей и источников света:

    class Scene : public mtt::RenderScene
    {
    private:
      mtt::MasterDrawModel dinoModel;
      mtt::MasterDrawModel fieldModel;
      mtt::clPipeline::AmbientLight ambientLight;
      mtt::clPipeline::PointLight pointLight;

Отличие от предыдущих примеров заключается только в настройке pointLight:

    pointLight.setShadowmapField(&culledData());
    pointLight.setShadowmapExtent(1024, 256);
    pointLight.setBlurAngle(0.01f);

Тени в clPipeline основаны на картах теней(shadow map). Поэтому pointLight должен самостоятельно строить эти карты. Для того чтобы он мог это делать, необходимо сообщить ему, что именно нужно отрисовывать. Для этого мы вызываем setShadowmapField.

Вызов PointLight::setShadowmapExtent может сначала удивить тем, что имеет 2 аргумента. Дело в том, что источники света в clPipeline строят 2 карты теней: тени от полностью непрозрачных объектов и тени от полупрозрачных объектов (сейчас это только системы частиц). Поэтому мы и передаем 2 значения для разрешений.

PointLight::setBlurAngle позволяет размывать границы теней, что дает возможность создавать более естественное освещение и скрывать артефакты пикселизации.
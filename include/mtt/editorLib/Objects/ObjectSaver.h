# pragma once

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/DataStream.h>
#include <mtt/application/TimeT.h>
#include <mtt/editorLib/Objects/CEVisitor.h>
#include <mtt/editorLib/Objects/ObjectFactory.h>

namespace mtt
{
  class ObjectSaver : public CEVisitor
  {
  public:
    ObjectSaver();
    ObjectSaver(const ObjectSaver&) = delete;
    ObjectSaver& operator = (const ObjectSaver&) = delete;
    virtual ~ObjectSaver() noexcept = default;

    void saveObject(const Object& object,
                    DataStream& stream,
                    const QDir& fileDirectory,
                    const ObjectFactory& objectFactory);
    /// Save without type index and id.
    /// For embedded objects.
    void saveObjectData(const Object& object,
                        DataStream& stream,
                        const QDir& fileDirectory,
                        const ObjectFactory& objectFactory);

    virtual void visitConstAmbientLightObject(
                                    const AmbientLightObject& object) override;
    virtual void visitConstAnimationGroup(
                                        const AnimationGroup& object) override;
    virtual void visitConstAnimationObject(
                                        const AnimationObject& object) override;
    virtual void visitConstAnimationTrack(
                                        const AnimationTrack& object) override;
    virtual void visitConstBackgroundObject(
                                      const BackgroundObject& object) override;
    virtual void visitConstCubemapObject(const CubemapObject& object) override;
    virtual void visitConstDirectLightObject(
                                      const DirectLightObject& object) override;
    virtual void visitConstDisplayedObject(
                                        const DisplayedObject& object) override;
    virtual void visitConstEnvironmentGroup(
                                      const EnvironmentGroup& object) override;
    virtual void visitConstEnvironmentModel(
                                      const EnvironmentModel& object) override;
    virtual void visitConstEnvironmentRootObject(
                                  const EnvironmentRootObject& object) override;
    virtual void visitConstFadingLightObject(
                                      const FadingLightObject& object) override;
    virtual void visitConstLightObject(const LightObject& object) override;
    virtual void visitConstLightWithShadowsObject(
                                const LightWithShadowsObject& object) override;
    virtual void visitConstObject(const mtt::Object& object) override;
    virtual void visitConstMovableObject(const MovableObject& object) override;
    virtual void visitConstPointLightObject(
                                      const PointLightObject& object) override;
    virtual void visitConstPositionAnimator(
                                      const PositionAnimator& object) override;
    virtual void visitConstRotatableObject(
                                        const RotatableObject& object) override;
    virtual void visitConstScalableObject(
                                        const ScalableObject& object) override;
    virtual void visitConstSkeletonGroup(const SkeletonGroup& object) override;
    virtual void visitConstSkeletonObject(
                                        const SkeletonObject& object) override;
    virtual void visitConstSpotLightObject(
                                        const SpotLightObject& object) override;

  protected:
    inline DataStream& stream() const noexcept;
    inline const QDir& fileDirectory() const noexcept;
    inline const ObjectFactory& objectFactory() const noexcept;

    void writeFilename(const QString& filename);
    template<typename ValueType>
    inline void writeKeypoint(ValueKeypoint<ValueType, TimeT> keypoint);
    void writeCubemapData(const CubemapObject& object);

    void writeChilds(const Object& parent);

  private:
    DataStream* _stream;
    QDir _fileDirectory;
    const ObjectFactory* _objectFactory;
  };

  inline DataStream& ObjectSaver::stream() const noexcept
  {
    return *_stream;
  }

  inline const QDir& ObjectSaver::fileDirectory() const noexcept
  {
    return _fileDirectory;
  }

  inline const ObjectFactory& ObjectSaver::objectFactory() const noexcept
  {
    return *_objectFactory;
  }

  template<typename ValueType>
  inline void ObjectSaver::writeKeypoint(
                                      ValueKeypoint<ValueType, TimeT> keypoint)
  {
    *_stream << keypoint.time();
    *_stream << keypoint.value();
    *_stream << (uint8_t) keypoint.interpolation();
  }
}
#include <mtt/application/Application.h>
#include <mtt/clPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/editorLib/Render/HullDrawableNode.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/utilities/Box.h>

using namespace mtt;

HullDrawableNode::HullDrawableNode() :
  _mesh(Application::instance().displayDevice())
{
  _mesh.setTechnique(
        clPipeline::colorFrameType,
        std::make_unique<clPipeline::InstrumentalCompositeTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));
  _mesh.setTechnique( clPipeline::uidFrameType,
                      std::make_unique<UidMeshTechnique>(
                                                VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                                                true,
                                                true));

  setColor(glm::vec3(.7f, .7f, .4f));

  setDrawable(&_mesh, Sphere());
}

void HullDrawableNode::setColor(const glm::vec3& color)
{
  SurfaceMaterialData materialData;
  materialData.albedo = glm::vec3(.7f, .7f, .4f);
  materialData.roughness = 1;
  materialData.specularStrength = 1;
  _mesh.extraData().setSurfaceMaterialData(materialData);
}

void HullDrawableNode::setGeometry(const std::vector<glm::vec3> newGeometry)
{
  if (newGeometry.empty())
  {
    resetGeometry();
    return;
  }

  try
  {
    Box boundingBox;
    for(const glm::vec3& point : newGeometry) boundingBox.extend(point);
    setLocalBoundSphere(boundingBox.buildBoundingSphere());

    LogicalDevice& device = Application::instance().displayDevice();
    std::shared_ptr<Buffer> positionsBuffer(new Buffer(device,
                                            Buffer::VERTEX_BUFFER));
    positionsBuffer->setData( newGeometry.data(),
                              newGeometry.size() * sizeof(glm::vec3));
    _mesh.setPositionBuffer(positionsBuffer);
    _mesh.setVerticesNumber(uint32_t(newGeometry.size()));
  }
  catch (...)
  {
    resetGeometry();
    throw;
  }
}

void HullDrawableNode::resetGeometry() noexcept
{
  setLocalBoundSphere(Sphere());
  _mesh.setVerticesNumber(0);
}

void HullDrawableNode::setSphereGeometry( float radius,
                                          unsigned int segmentNumber)
{
  if (radius <= 0.f)
  {
    Log() << "HullDrawableNode::setSphereGeometry: radius <= 0";
    resetGeometry();
    return;
  }

  if (segmentNumber < 3)
  {
    Log() << "HullDrawableNode::setSphereGeometry: segmentNumber < 3";
    resetGeometry();
    return;
  }

  try
  {
    std::vector<glm::vec3> vertices;
    vertices.reserve(segmentNumber * 2 * 3);

    float angleStep = 2.f * glm::pi<float>() / segmentNumber;
    float currentAngle = 0;
    for(unsigned int segment = 0;
        segment < segmentNumber - 1;
        segment++)
    {
      float nextAngle = currentAngle + angleStep;

      float sinCurrent = sin(currentAngle) * radius;
      float cosCurrent = cos(currentAngle) * radius;
      float sinNext = sin(nextAngle) * radius;
      float cosNext = cos(nextAngle) * radius;

      vertices.emplace_back(cosCurrent, sinCurrent, 0.f);
      vertices.emplace_back(cosNext, sinNext, 0.f);
      vertices.emplace_back(cosCurrent, 0.f, sinCurrent);
      vertices.emplace_back(cosNext, 0.f, sinNext);
      vertices.emplace_back(0.f, cosCurrent, sinCurrent);
      vertices.emplace_back(0.f, cosNext, sinNext);

      currentAngle = nextAngle;
    }

    float sinCurrent = sin(currentAngle) * radius;
    float cosCurrent = cos(currentAngle) * radius;
    vertices.emplace_back(cosCurrent, sinCurrent, 0.f);
    vertices.emplace_back(radius, 0.f, 0.f);
    vertices.emplace_back(cosCurrent, 0.f, sinCurrent);
    vertices.emplace_back(radius, 0.f, 0.f);
    vertices.emplace_back(0.f, cosCurrent, sinCurrent);
    vertices.emplace_back(0.f, radius, 0.f);

    setGeometry(vertices);
  }
  catch (...)
  {
    resetGeometry();
    throw;
  }
}

void HullDrawableNode::setCylinderGeometry( float radius,
                                            float length,
                                            float centerShift,
                                            unsigned int bodySegmentNumber,
                                            unsigned int capSegmentNumber)
{
  if (radius <= 0.f)
  {
    Log() << "HullDrawableNode::setCylinderGeometry: radius <= 0";
    resetGeometry();
    return;
  }
  if (length <= 0.f)
  {
    Log() << "HullDrawableNode::setCylinderGeometry: length <= 0";
    resetGeometry();
    return;
  }
  if (capSegmentNumber < 3)
  {
    Log() << "HullDrawableNode::setCylinderGeometry: capSegmentNumber < 3";
    resetGeometry();
    return;
  }

  try
  {
    std::vector<glm::vec3> vertices;
    vertices.reserve(bodySegmentNumber * 2 + capSegmentNumber * 2 * 2);

    float zUp = length / 2.f + centerShift;
    float zDown = -length / 2.f + centerShift;

    if(bodySegmentNumber != 0)
    {
      float bodyStep = 2.f * glm::pi<float>() / bodySegmentNumber;
      float bodyAngle = 0;
      for(size_t bodySegment = 0;
          bodySegment < bodySegmentNumber;
          bodySegment++)
      {
        vertices.emplace_back(cos(bodyAngle) * radius,
                              sin(bodyAngle) * radius,
                              zUp);
        vertices.emplace_back(cos(bodyAngle) * radius,
                              sin(bodyAngle) * radius,
                              zDown);
        bodyAngle += bodyStep;
      }
    }

    float capStep = 2.f * glm::pi<float>() / capSegmentNumber;
    float capAngle = 0;
    for(size_t capSegment = 0; capSegment < capSegmentNumber - 1; capSegment++)
    {
      float nextAngle = capAngle + capStep;

      vertices.emplace_back(cos(capAngle) * radius,
                            sin(capAngle) * radius,
                            zUp);
      vertices.emplace_back(cos(nextAngle) * radius,
                            sin(nextAngle) * radius,
                            zUp);

      vertices.emplace_back(cos(capAngle) * radius,
                            sin(capAngle) * radius,
                            zDown);
      vertices.emplace_back(cos(nextAngle) * radius,
                            sin(nextAngle) * radius,
                            zDown);

      capAngle = nextAngle;
    }

    vertices.emplace_back(cos(capAngle) * radius,
                          sin(capAngle) * radius,
                          zUp);
    vertices.emplace_back(radius, 0.f, zUp);

    vertices.emplace_back(cos(capAngle) * radius,
                          sin(capAngle) * radius,
                          zDown);
    vertices.emplace_back(radius, 0.f, zDown);

    setGeometry(vertices);
  }
  catch (...)
  {
    resetGeometry();
    throw;
  }
}

void HullDrawableNode::setCircleGeometry( float radius,
                                          unsigned int segmentNumber)
{
  if (radius <= 0.f)
  {
    Log() << "HullDrawableNode::setCircleGeometry: radius <= 0";
    resetGeometry();
    return;
  }
  if (segmentNumber < 3)
  {
    Log() << "HullDrawableNode::setCircleGeometry: segmentNumber < 3";
    resetGeometry();
    return;
  }

  try
  {
    std::vector<glm::vec3> vertices;
    vertices.reserve(segmentNumber * 2);

    float capStep = 2.f * glm::pi<float>() / segmentNumber;
    float capAngle = 0;
    for(size_t segmentIndex = 0;
        segmentIndex < segmentNumber - 1;
        segmentIndex++)
    {
      float nextAngle = capAngle + capStep;

      vertices.emplace_back(cos(capAngle) * radius,
                            sin(capAngle) * radius,
                            0.f);
      vertices.emplace_back(cos(nextAngle) * radius,
                            sin(nextAngle) * radius,
                            0.f);
      capAngle = nextAngle;
    }

    vertices.emplace_back(cos(capAngle) * radius,
                          sin(capAngle) * radius,
                          0.f);
    vertices.emplace_back(radius, 0.f, 0.f);

    setGeometry(vertices);
  }
  catch (...)
  {
    resetGeometry();
    throw;
  }
}

void HullDrawableNode::setBoxGeometry(glm::vec3 size)
{
  if (size.x <= 0.f || size.y <= 0.f || size.z <= 0.f)
  {
    Log() << "HullDrawableNode::setBoxGeometry: size <= 0";
    resetGeometry();
    return;
  }

  try
  {
    glm::vec3 halfSize = size / 2.f;

    std::vector<glm::vec3> newVertices;
    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z));
    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y,  halfSize.z));

    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y,  halfSize.z));
    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x,  halfSize.y,  halfSize.z));

    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y,  halfSize.z));
    newVertices.push_back(glm::vec3( halfSize.x,  halfSize.y,  halfSize.z));

    newVertices.push_back(glm::vec3( halfSize.x, -halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3( halfSize.x,  halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x,  halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, halfSize.y, halfSize.z));
    newVertices.push_back(glm::vec3(halfSize.x, halfSize.y, halfSize.z));

    newVertices.push_back(glm::vec3(halfSize.x, halfSize.y, halfSize.z));
    newVertices.push_back(glm::vec3(halfSize.x, halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(halfSize.x, halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x, halfSize.y, -halfSize.z));

    newVertices.push_back(glm::vec3(-halfSize.x, halfSize.y, -halfSize.z));
    newVertices.push_back(glm::vec3(-halfSize.x, halfSize.y, halfSize.z));

    setGeometry(newVertices);
  }
  catch (...)
  {
    resetGeometry();
    throw;
  }
}

#include "SpaceshipRender.h"

#include <game/renders/ActorSpawnController.h>

#include <unordered_map>
#include <unordered_set>

#include "MainPlayer.h"
#include "Npc.h"
#include "SpaceshipsActor.h"
#include "game/rendering/resources/ResourceLoader.h"
#include "game/rendering/vertices/PositionNormalTextureVertex.h"
#include "game/renders/spaceships/SpaceshipMesh.h"
#include "general/files/images/png.h"
#include "general/files/obj.h"
#include "util/filenames.h"

struct MeshFrameLoadParams {
  std::string_view model;
};

struct MeshLoadParams {
  std::vector<MeshFrameLoadParams> frames;

  std::string_view texture;
  std::string_view emissive;
};

SpaceshipMesh LoadSpaceshipMesh(ResourceLoader& resourceLoader,
                                const MeshLoadParams& params) {
  std::unordered_set<file::ModelFace> uniqueFaces;
  std::unordered_map<PositionNormalTextureVertex, u16> uniqueVertices;
  std::vector<PositionNormalTextureVertex> vertices;
  std::vector<u16> indices;
  std::vector<MeshFrame> meshFrames(params.frames.size());

  glm::vec3 corner1 = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 corner2 = glm::vec3(0.0f, 0.0f, 0.0f);

  for (u32 frameIndex = 0; frameIndex < params.frames.size(); ++frameIndex) {
    const auto& frame = params.frames[frameIndex];
    const file::Model frameModel = file::ModelFromObjFile(frame.model);

    for (const auto& face : frameModel.faces) {
      if (uniqueFaces.contains(face)) {
        continue;
      }

      uniqueFaces.emplace(face);

      for (u32 vertexIndex = 0; vertexIndex < 3; ++vertexIndex) {
        const file::ModelFaceVertex modelFaceVertex =
            face.faceVertices[vertexIndex];

        PositionNormalTextureVertex vertex;
        vertex.position.x = frameModel.vertices[modelFaceVertex.vertexIndex].x;
        vertex.position.y = frameModel.vertices[modelFaceVertex.vertexIndex].y;
        vertex.position.z = frameModel.vertices[modelFaceVertex.vertexIndex].z;
        vertex.normal.x =
            frameModel.normalVertices[modelFaceVertex.normalVertexIndex].x;
        vertex.normal.y =
            frameModel.normalVertices[modelFaceVertex.normalVertexIndex].y;
        vertex.normal.z =
            frameModel.normalVertices[modelFaceVertex.normalVertexIndex].z;
        vertex.textureCoordinate.x =
            frameModel.textureVertices[modelFaceVertex.textureVertexIndex].u;
        vertex.textureCoordinate.y =
            frameModel.textureVertices[modelFaceVertex.textureVertexIndex].v;

        if (uniqueVertices.contains(vertex)) {
          indices.push_back(uniqueVertices[vertex]);
        } else {
          const u16 index = static_cast<u16>(uniqueVertices.size());

          uniqueVertices.emplace(vertex, index);

          vertices.push_back(vertex);
          indices.push_back(index);

          if (corner1.x < vertex.position.x) {
            corner1.x = vertex.position.x;
          }
          if (corner1.y < vertex.position.y) {
            corner1.y = vertex.position.y;
          }
          if (corner1.z < vertex.position.z) {
            corner1.z = vertex.position.z;
          }
          if (corner2.x > vertex.position.x) {
            corner2.x = vertex.position.x;
          }
          if (corner2.y > vertex.position.y) {
            corner2.y = vertex.position.y;
          }
          if (corner2.z > vertex.position.z) {
            corner2.z = vertex.position.z;
          }
        }
      }
    }

    meshFrames[frameIndex] =
        MeshFrame{.indexCount = static_cast<u32>(indices.size())};
  }

  const glm::vec3 modelCenter = (corner1 + corner2) / 2.0f;
  const glm::vec3 modelSize = corner1 - corner2;

  return SpaceshipMesh(resourceLoader.AllocateVertexBuffer(vertices),
                       resourceLoader.AllocateIndexBuffer(indices),
                       resourceLoader.LoadTexture(params.texture),
                       resourceLoader.LoadTexture(params.emissive),
                       std::move(meshFrames), modelCenter, modelSize);
}

struct ModelTransform {
  alignas(16) glm::mat4 model;
};

class SpaceshipPipelineStateFactory : public PipelineStateFactory {
 public:
  SpaceshipPipelineStateFactory()
      : pushConstantsRange(PushConstantRangeBuilder()
                               .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT)
                               .SetOffset(0)
                               .SetSize(sizeof(ModelTransform))),
        vertexBindingDescriptions({
            VertexInputBindingDescriptionBuilder()
                .SetBinding(0)
                .SetStride(sizeof(PositionNormalTextureVertex))
                .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
        }),
        vertexAttributeDescriptions({
            VertexInputAttributeDescriptionBuilder()
                .SetBinding(0)
                .SetLocation(0)
                .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                .SetOffset(offsetof(PositionNormalTextureVertex, position)),
            VertexInputAttributeDescriptionBuilder()
                .SetBinding(0)
                .SetLocation(1)
                .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                .SetOffset(offsetof(PositionNormalTextureVertex, normal)),
            VertexInputAttributeDescriptionBuilder()
                .SetBinding(0)
                .SetLocation(2)
                .SetFormat(VK_FORMAT_R32G32_SFLOAT)
                .SetOffset(
                    offsetof(PositionNormalTextureVertex, textureCoordinate)),
        }) {}

  std::vector<ShaderModule> LoadShaders(
      const ShaderModuleFactory& shaderModuleFactory) const override {
    std::vector<ShaderModule> shaders;
    shaders.emplace_back(shaderModuleFactory.LoadShader(
        VK_SHADER_STAGE_VERTEX_BIT, "shaders/spaceship.vert.spv"));
    shaders.emplace_back(shaderModuleFactory.LoadShader(
        VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/spaceship.frag.spv"));
    return shaders;
  }

  PipelineLayoutCreateInfoBuilder CreatePipelineLayout() const override {
    return PipelineLayoutCreateInfoBuilder()
        .SetPushConstantRangeCount(1)
        .SetPPushConstantRanges(&pushConstantsRange);
  }

  PipelineVertexInputStateCreateInfoBuilder CreateVertexInputState()
      const override {
    return PipelineVertexInputStateCreateInfoBuilder()
        .SetVertexBindingDescriptionCount(vertexBindingDescriptions.size())
        .SetPVertexBindingDescriptions(vertexBindingDescriptions.data())
        .SetVertexAttributeDescriptionCount(vertexAttributeDescriptions.size())
        .SetPVertexAttributeDescriptions(vertexAttributeDescriptions.data());
  }

 private:
  const VkPushConstantRange pushConstantsRange;
  const std::array<VkVertexInputBindingDescription, 1>
      vertexBindingDescriptions;
  const std::array<VkVertexInputAttributeDescription, 3>
      vertexAttributeDescriptions;
};

class SpaceshipDescriptorConfiguration : public DescriptorConfiguration {
 public:
  std::optional<DescriptorSetLayout> ConfigureActorDescriptorSet(
      const DescriptorSetLayoutFactory& descriptorSetLayoutFactory)
      const override {
    constexpr const std::array<VkDescriptorSetLayoutBinding, 2>
        textureSamplerLayoutBindings = {
            DescriptorSetLayoutBindingBuilder()
                .SetBinding(0)
                .SetDescriptorCount(1)
                .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT),
            DescriptorSetLayoutBindingBuilder()
                .SetBinding(1)
                .SetDescriptorCount(1)
                .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT),
        };
    return descriptorSetLayoutFactory.CreateDescriptorSetLayout(
        DescriptorSetLayoutCreateInfoBuilder()
            .SetBindingCount(textureSamplerLayoutBindings.size())
            .SetPBindings(textureSamplerLayoutBindings.data()));
  }
};

SpaceshipRender::SpaceshipRender(Camera& camera,
                                 ParticleController& particleController)
    : camera(camera), particleController(particleController) {}

std::unique_ptr<PipelineStateFactory> SpaceshipRender::ConfigurePipeline()
    const {
  return std::make_unique<SpaceshipPipelineStateFactory>();
}

std::unique_ptr<DescriptorConfiguration> SpaceshipRender::ConfigureDescriptors()
    const {
  return std::make_unique<SpaceshipDescriptorConfiguration>();
}

void SpaceshipRender::LoadActors(ResourceLoader& resourceLoader,
                                 ActorSpawnController& actorSpawnController) {
  std::vector<std::unique_ptr<Actor>> actors(2);
  actors[0] = std::make_unique<SpaceshipsActor>(
      LoadSpaceshipMesh(
          resourceLoader,
          MeshLoadParams{
              .frames = {MeshFrameLoadParams{
                             .model = SPACESHIP_STATIONARY_MODEL_FILENAME},
                         MeshFrameLoadParams{
                             .model = SPACESHIP_MOVING_MODEL_FILENAME}},
              .texture = SPACESHIP_TEXTURE_FILENAME,
              .emissive = SPACESHIP_EMISSIVE_FILENAME}),
      camera, particleController);
  actors[1] = std::make_unique<Npc>(
      LoadSpaceshipMesh(
          resourceLoader,
          MeshLoadParams{.frames = {MeshFrameLoadParams{
                             .model = NPC_SPACESHIP_MODEL_FILENAME}},
                         .texture = NPC_SPACESHIP_TEXTURE_FILENAME,
                         .emissive = NPC_SPACESHIP_EMISSIVE_FILENAME}),
      particleController.CreateParticleStream());

  actorSpawnController.SpawnActorsImmediately(std::move(actors));
}

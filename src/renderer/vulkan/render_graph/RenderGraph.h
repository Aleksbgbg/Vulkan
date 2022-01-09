#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_RENDERGRAPH_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_RENDERGRAPH_H_

#include <list>
#include <memory>
#include <unordered_map>

#include "HostDescriptorWriter.h"
#include "InsertComputeInfo.h"
#include "InsertPipelineBuilder.h"
#include "InsertRenderInfo.h"
#include "game/actor/resource/Resource.h"
#include "game/actor/resource/ResourceList.h"
#include "renderer/vulkan/IndexedVertexBuffer.h"
#include "renderer/vulkan/Texture.h"
#include "renderer/vulkan/api/CommandBuffer.h"
#include "renderer/vulkan/render_graph/layout/RenderGraphLayout.h"

class RenderGraph {
 public:
  class ResourceAllocator {
   public:
    virtual ~ResourceAllocator() = default;

    virtual vk::DescriptorSetLayout CreateDescriptorSetLayout(
        const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const = 0;
    virtual vk::DescriptorSet CreateDescriptorSet(
        const vk::DescriptorSetLayout& layout) const = 0;
    virtual void UpdateDescriptorSets(
        const std::vector<vk::DescriptorSet::WriteDescriptorSet>&
            descriptorSetWrites) const = 0;

    virtual BoundBuffer AllocateHostBuffer(std::size_t size,
                                           VkBufferUsageFlags usage) = 0;
    virtual BoundBuffer AllocateDeviceBuffer(std::size_t size,
                                             VkBufferUsageFlags usage) = 0;

    virtual vk::ShaderModule LoadComputeShader(std::string_view name) const = 0;
    virtual vk::ShaderModule LoadGraphicsShader(
        std::string_view name, VkShaderStageFlagBits stage) const = 0;

    virtual vk::ComputePipeline CreateComputePipeline(
        const std::vector<const vk::DescriptorSetLayout*>& descriptorSetLayouts,
        vk::ShaderModule computeShader) const = 0;
    virtual vk::GraphicsPipeline CreateGraphicsPipeline(
        const std::vector<const vk::DescriptorSetLayout*>& descriptorSetLayouts,
        std::vector<vk::ShaderModule> shaders,
        const VkVertexInputBindingDescription& vertexInputBindingDescription,
        const std::vector<VkVertexInputAttributeDescription>&
            vertexInputAttributeDescriptions) const = 0;
  };

 private:
  struct DescriptorBinding {
    enum class Type { Buffer, Sampler };
    Type type;

    std::optional<BoundBuffer> hostBuffer;
    BoundBuffer deviceBuffer;
  };

  struct Descriptor {
    vk::DescriptorSet set;
    std::vector<std::shared_ptr<DescriptorBinding>> bindings;
  };

  struct ComputeInstance {
    std::unique_ptr<HostDescriptorWriter> descriptorWriter;
    Descriptor descriptor;
  };

  struct ComputePipeline {
    vk::ComputePipeline pipeline;
    vk::DescriptorSetLayout descriptorLayout;
    DescriptorSetStructure descriptorStructure;
    ResourceList<ComputeInstance> instances;
  };

  struct RenderInstance {
    std::unique_ptr<HostDescriptorWriter> descriptorWriter;
    Descriptor descriptor;
    const IndexedVertexBuffer* drawBuffer;
    u32 drawInstances;
  };

  struct RenderPipeline {
    vk::GraphicsPipeline pipeline;
    vk::DescriptorSetLayout descriptorLayout;
    DescriptorSetStructure descriptorStructure;
    ResourceList<RenderInstance> instances;
    std::vector<std::list<RenderInstance>> instancesToReleasePerFrame;
  };

  template <typename TPipeline>
  struct PipelineGraph {
    std::unordered_map<PipelineKey, TPipeline> pipelines;
    const vk::PipelineLayout* rootPipelineLayout;
    vk::DescriptorSetLayout descriptorLayout;
    Descriptor descriptor;
  };

  ResourceAllocator& allocator_;
  std::vector<vk::DescriptorSet::WriteDescriptorSet> descriptorSetWrites_;
  PipelineGraph<ComputePipeline> computePipelineGraph_;
  PipelineGraph<RenderPipeline> graphicsPipelineGraph_;

 public:
  RenderGraph(ResourceAllocator& allocator, u32 framesInFlight,
              const RenderGraphLayout& layout);

  std::unique_ptr<Resource> Insert(InsertPipelineBuilder insertBuilder);

  void ExecuteComputePipelines(const vk::CommandBuffer& transfer,
                               const vk::CommandBuffer& compute,
                               const void* globalUniformData);
  void ExecuteRenderPipelines(const vk::CommandBuffer& transfer,
                              const vk::CommandBuffer& graphics,
                              const void* globalUniformData, u32 frameIndex);

 private:
  std::unique_ptr<Resource> InsertCompute(PipelineKey key,
                                          InsertComputeInfo insertInfo,
                                          Descriptor descriptor);
  std::unique_ptr<Resource> InsertRender(PipelineKey key,
                                         InsertRenderInfo insertInfo,
                                         Descriptor descriptor);

  std::shared_ptr<DescriptorBinding> CreateSamplerBinding(
      const Texture& texture, const Descriptor& descriptor,
      const Binding& binding);
  struct BufferDescriptor {
    std::optional<BoundBuffer> hostBuffer;
    BoundBuffer deviceBuffer;
  };
  BufferDescriptor AllocateBufferDescriptor(const Binding& binding,
                                            u32 instances);

  Descriptor CreateDescriptor(const vk::DescriptorSetLayout& layout,
                              const DescriptorSetStructure& structure);

  void FlushDescriptors();
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_RENDERGRAPH_H_

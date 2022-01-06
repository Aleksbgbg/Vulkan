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
#include "renderer/vulkan/IndexedVertexBuffer.h"
#include "renderer/vulkan/Texture.h"
#include "renderer/vulkan/api/CommandBuffer.h"
#include "renderer/vulkan/render_graph/layout/RenderGraphLayout.h"

using ResourceKey = u32;

class RenderGraph {
 public:
  class ResourceAllocator {
   public:
    virtual ~ResourceAllocator() = default;

    virtual DescriptorSetLayout CreateDescriptorSetLayout(
        const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const = 0;
    virtual DescriptorSet CreateDescriptorSet(
        const DescriptorSetLayout& layout) const = 0;
    virtual void UpdateDescriptorSets(
        const std::vector<DescriptorSet::WriteDescriptorSet>&
            descriptorSetWrites) const = 0;

    virtual BoundBuffer AllocateHostBuffer(std::size_t size,
                                           VkBufferUsageFlags usage) = 0;
    virtual BoundBuffer AllocateDeviceBuffer(std::size_t size,
                                             VkBufferUsageFlags usage) = 0;

    virtual ShaderModule LoadComputeShader(std::string_view name) const = 0;
    virtual ShaderModule LoadGraphicsShader(
        std::string_view name, VkShaderStageFlagBits stage) const = 0;

    virtual ComputePipeline CreateComputePipeline(
        const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
        ShaderModule computeShader) const = 0;
    virtual GraphicsPipeline CreateGraphicsPipeline(
        const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
        std::vector<ShaderModule> shaders,
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
    DescriptorSet set;
    std::vector<std::shared_ptr<DescriptorBinding>> bindings;
  };

  struct ComputeInstance {
    std::unique_ptr<HostDescriptorWriter> descriptorWriter;
    Descriptor descriptor;
  };

  struct ComputePipeline {
    ::ComputePipeline pipeline;
    DescriptorSetLayout descriptorLayout;
    DescriptorSetStructure descriptorStructure;
    std::unordered_map<ResourceKey, ComputeInstance> instances;
  };

  struct RenderInstance {
    std::unique_ptr<HostDescriptorWriter> descriptorWriter;
    Descriptor descriptor;
    const IndexedVertexBuffer* drawBuffer;
    u32 drawInstances;
  };

  struct RenderPipeline {
    GraphicsPipeline pipeline;
    DescriptorSetLayout descriptorLayout;
    DescriptorSetStructure descriptorStructure;
    std::unordered_map<ResourceKey, RenderInstance> instances;
  };

  struct ResourceIdentifier {
    PipelineKey pipeline;
    ResourceKey key;
  };

  template <typename TPipeline>
  struct PipelineGraph {
    std::unordered_map<PipelineKey, TPipeline> pipelines;
    std::list<ResourceIdentifier> instancesToRemove;
    const PipelineLayout* rootPipelineLayout;
    DescriptorSetLayout descriptorLayout;
    Descriptor descriptor;
  };

  ResourceAllocator& allocator_;
  std::vector<DescriptorSet::WriteDescriptorSet> descriptorSetWrites_;
  PipelineGraph<ComputePipeline> computePipelineGraph_;
  PipelineGraph<RenderPipeline> graphicsPipelineGraph_;

 public:
  RenderGraph(ResourceAllocator& initializer, const RenderGraphLayout& layout);

  std::unique_ptr<Resource> Insert(InsertPipelineBuilder insertBuilder);

  void ExecuteComputePipelines(const CommandBuffer& transfer,
                               const CommandBuffer& compute,
                               const void* globalUniformData);
  void ExecuteRenderPipelines(const CommandBuffer& transfer,
                              const CommandBuffer& graphics,
                              const void* globalUniformData);

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

  Descriptor CreateDescriptor(const DescriptorSetLayout& layout,
                              const DescriptorSetStructure& structure);

  void FlushDescriptors();
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_RENDERGRAPH_H_

#include "RenderGraph.h"

#include "core/adapters/MapValueIterator.h"
#include "engine/resource/CompositeResource.h"
#include "engine/resource/ReleaseListResource.h"
#include "renderer/vulkan/buffer_structures/ModelTransform.h"

namespace {

std::string_view StageExtension(const VkShaderStageFlagBits shaderStage) {
  switch (shaderStage) {
    case VK_SHADER_STAGE_VERTEX_BIT:
      return "vert";
    case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
      return "tesc";
    case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
      return "tese";
    case VK_SHADER_STAGE_GEOMETRY_BIT:
      return "geom";
    case VK_SHADER_STAGE_FRAGMENT_BIT:
      return "frag";
    case VK_SHADER_STAGE_COMPUTE_BIT:
      return "comp";
    default:
      return "";
  }
}

std::string ShaderName(const std::string_view pipelineName,
                       const VkShaderStageFlagBits shaderStage) {
  return std::string("shaders/") + pipelineName.data() + "." +
         StageExtension(shaderStage).data() + ".spv";
}

}  // namespace

vk::DescriptorSetLayout DescriptorSetStructureAsLayout(
    const DescriptorSetStructure& structure,
    const RenderGraph::ResourceAllocator& initializer) {
  std::vector<VkDescriptorSetLayoutBinding> descriptorBindings;
  for (const auto& binding : structure.bindings) {
    descriptorBindings.push_back(DescriptorSetLayoutBindingBuilder()
                                     .SetBinding(binding.index)
                                     .SetDescriptorCount(1)
                                     .SetDescriptorType(binding.type)
                                     .SetStageFlags(binding.stageFlags));
  }

  return initializer.CreateDescriptorSetLayout(
      DescriptorSetLayoutCreateInfoBuilder()
          .SetBindingCount(descriptorBindings.size())
          .SetPBindings(descriptorBindings.data()));
}

void RenderGraph::FlushDescriptors() {
  allocator_->UpdateDescriptorSets(descriptorSetWrites_);
  descriptorSetWrites_.clear();
}

std::shared_ptr<RenderGraph::DescriptorBinding>
RenderGraph::CreateSamplerBinding(const Texture& texture,
                                  const RenderGraph::Descriptor& descriptor,
                                  const Binding& binding) {
  descriptorSetWrites_.push_back(descriptor.set.CreateImageSamplerWrite(
      texture.view, *binding.sampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      binding.index));
  return std::make_shared<DescriptorBinding>(
      DescriptorBinding{.type = DescriptorBinding::Type::Sampler});
}

RenderGraph::BufferDescriptor RenderGraph::AllocateBufferDescriptor(
    const Binding& binding, const u32 instances) {
  const std::size_t size = binding.size * instances;
  BufferDescriptor bufferDescriptor;
  if (binding.hostAccessible) {
    bufferDescriptor.hostBuffer = allocator_->AllocateHostBuffer(
        size, binding.bufferUsage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  }
  bufferDescriptor.deviceBuffer = allocator_->AllocateDeviceBuffer(
      size, binding.bufferUsage | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  return bufferDescriptor;
}

RenderGraph::Descriptor RenderGraph::CreateDescriptor(
    const vk::DescriptorSetLayout& layout,
    const DescriptorSetStructure& structure) {
  Descriptor descriptor{.set = allocator_->CreateDescriptorSet(layout)};

  for (const auto& binding : structure.bindings) {
    BufferDescriptor bufferDescriptor = AllocateBufferDescriptor(binding, 1);

    descriptorSetWrites_.push_back(bufferDescriptor.deviceBuffer.WriteBuffer(
        descriptor.set, binding.type, binding.index));

    descriptor.bindings.push_back(
        std::make_shared<DescriptorBinding>(DescriptorBinding{
            .type = DescriptorBinding::Type::Buffer,
            .hostBuffer = std::move(bufferDescriptor.hostBuffer),
            .deviceBuffer = std::move(bufferDescriptor.deviceBuffer)}));
  }

  return descriptor;
}

RenderGraph::RenderGraph(RenderGraph::ResourceAllocator& allocator,
                         const u32 framesInFlight,
                         const RenderGraphLayout& layout)
    : allocator_(&allocator),
      descriptorSetWrites_(),
      computePipelineGraph_(
          {.descriptorLayout = DescriptorSetStructureAsLayout(
               layout.globalComputeDescriptors, *allocator_)}),
      graphicsPipelineGraph_(
          {.descriptorLayout = DescriptorSetStructureAsLayout(
               layout.globalGraphicsDescriptors, *allocator_)}) {
  computePipelineGraph_.descriptor = CreateDescriptor(
      computePipelineGraph_.descriptorLayout, layout.globalComputeDescriptors);
  graphicsPipelineGraph_.descriptor =
      CreateDescriptor(graphicsPipelineGraph_.descriptorLayout,
                       layout.globalGraphicsDescriptors);

  for (const auto& pipelineStructure : layout.computePipelines) {
    vk::DescriptorSetLayout descriptorLayout = DescriptorSetStructureAsLayout(
        pipelineStructure.descriptors, *allocator_);

    computePipelineGraph_.pipelines.insert(std::make_pair(
        pipelineStructure.pipelineKey,
        ComputePipeline{
            .pipeline = allocator_->CreateComputePipeline(
                {&computePipelineGraph_.descriptorLayout, &descriptorLayout},
                allocator_->LoadComputeShader(
                    ShaderName(pipelineStructure.name,
                               pipelineStructure.shader.shaderStage))),
            .descriptorLayout = std::move(descriptorLayout),
            .descriptorStructure = pipelineStructure.descriptors}));
  }
  for (const auto& pipelineStructure : layout.graphicsPipelines) {
    vk::DescriptorSetLayout descriptorLayout = DescriptorSetStructureAsLayout(
        pipelineStructure.descriptors, *allocator_);
    std::vector<vk::ShaderModule> shaders;
    for (const auto& shader : pipelineStructure.shaders) {
      shaders.push_back(allocator_->LoadGraphicsShader(
          ShaderName(pipelineStructure.name, shader.shaderStage),
          shader.shaderStage));
    }

    graphicsPipelineGraph_.pipelines.insert(std::make_pair(
        pipelineStructure.pipelineKey,
        RenderPipeline{
            .pipeline = allocator_->CreateGraphicsPipeline(
                {&graphicsPipelineGraph_.descriptorLayout, &descriptorLayout},
                std::move(shaders), pipelineStructure.vertexBinding,
                pipelineStructure.vertexAttributes,
                pipelineStructure.graphicsPipelineTemplate),
            .descriptorLayout = std::move(descriptorLayout),
            .descriptorStructure = pipelineStructure.descriptors,
            .instancesToReleasePerFrame =
                std::vector<std::list<RenderInstance>>(framesInFlight)}));
  }

  computePipelineGraph_.rootPipelineLayout =
      &FirstValue(computePipelineGraph_.pipelines).pipeline.GetLayout();
  graphicsPipelineGraph_.rootPipelineLayout =
      &FirstValue(graphicsPipelineGraph_.pipelines).pipeline.GetLayout();
}

std::unique_ptr<Resource> RenderGraph::Insert(
    InsertPipelineBuilder insertBuilder) {
  InsertPipelineBuilder::BuildResult& result = insertBuilder.Build();

  std::unordered_map<u32, std::shared_ptr<DescriptorBinding>> connectedBuffers;
  std::unordered_map<PipelineKey, Descriptor> descriptors;

  for (const auto& compute : result.computes) {
    ComputePipeline& pipeline = computePipelineGraph_.pipelines.at(compute.key);
    Descriptor descriptor{
        .set = allocator_->CreateDescriptorSet(pipeline.descriptorLayout)};
    for (const auto& binding : pipeline.descriptorStructure.bindings) {
      switch (binding.type) {
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: {
          const PipelineDescriptorReference descriptorReference =
              PipelineDescriptorReference(compute.key, binding.index);

          if (result.pipelineConnections.contains(descriptorReference)) {
            const InsertPipelineBuilder::BuildResult::ConnectionId
                connectionId =
                    result.pipelineConnections.at(descriptorReference);

            if (connectedBuffers.contains(connectionId)) {
              const std::shared_ptr<DescriptorBinding>& descriptorBinding =
                  connectedBuffers.at(connectionId);
              descriptorSetWrites_.push_back(
                  descriptorBinding->deviceBuffer.WriteBuffer(
                      descriptor.set, binding.type, binding.index));
            } else {
              BufferDescriptor bufferDescriptor = AllocateBufferDescriptor(
                  binding,
                  binding.instanced ? compute.insertInfo.instances : 1);
              const std::shared_ptr<DescriptorBinding> descriptorBinding =
                  std::make_shared<DescriptorBinding>(DescriptorBinding{
                      .type = DescriptorBinding::Type::Buffer,
                      .hostBuffer = std::move(bufferDescriptor.hostBuffer),
                      .deviceBuffer =
                          std::move(bufferDescriptor.deviceBuffer)});

              descriptorSetWrites_.push_back(
                  descriptorBinding->deviceBuffer.WriteBuffer(
                      descriptor.set, binding.type, binding.index));

              connectedBuffers.insert(
                  std::make_pair(connectionId, descriptorBinding));

              descriptor.bindings.push_back(descriptorBinding);
            }
          } else {
            BufferDescriptor bufferDescriptor = AllocateBufferDescriptor(
                binding, binding.instanced ? compute.insertInfo.instances : 1);
            std::shared_ptr<DescriptorBinding> descriptorBinding =
                std::make_shared<DescriptorBinding>(DescriptorBinding{
                    .type = DescriptorBinding::Type::Buffer,
                    .hostBuffer = std::move(bufferDescriptor.hostBuffer),
                    .deviceBuffer = std::move(bufferDescriptor.deviceBuffer)});
            descriptorSetWrites_.push_back(
                descriptorBinding->deviceBuffer.WriteBuffer(
                    descriptor.set, binding.type, binding.index));
            descriptor.bindings.push_back(std::move(descriptorBinding));
          }
        } break;

        default:
          throw std::runtime_error("Unexpected descriptor binding type.");
      }
    }

    descriptors.insert(std::make_pair(compute.key, std::move(descriptor)));
  }

  for (const auto& render : result.renders) {
    RenderPipeline& pipeline = graphicsPipelineGraph_.pipelines.at(render.key);
    Descriptor descriptor{
        .set = allocator_->CreateDescriptorSet(pipeline.descriptorLayout)};
    u32 textureIndex = 0;
    for (const auto& binding : pipeline.descriptorStructure.bindings) {
      switch (binding.type) {
        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {
          const Texture& texture =
              render.insertInfo.textures->at(textureIndex++);
          descriptor.bindings.push_back(
              CreateSamplerBinding(texture, descriptor, binding));
        } break;

        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: {
          const PipelineDescriptorReference descriptorReference =
              PipelineDescriptorReference(render.key, binding.index);

          if (result.pipelineConnections.contains(descriptorReference)) {
            const InsertPipelineBuilder::BuildResult::ConnectionId
                connectionId =
                    result.pipelineConnections.at(descriptorReference);

            if (connectedBuffers.contains(connectionId)) {
              const std::shared_ptr<DescriptorBinding>& descriptorBinding =
                  connectedBuffers.at(connectionId);
              descriptorSetWrites_.push_back(
                  descriptorBinding->deviceBuffer.WriteBuffer(
                      descriptor.set, binding.type, binding.index));
            } else {
              BufferDescriptor bufferDescriptor = AllocateBufferDescriptor(
                  binding, binding.instanced ? render.insertInfo.instances : 1);
              const std::shared_ptr<DescriptorBinding> descriptorBinding =
                  std::make_shared<DescriptorBinding>(DescriptorBinding{
                      .type = DescriptorBinding::Type::Buffer,
                      .hostBuffer = std::move(bufferDescriptor.hostBuffer),
                      .deviceBuffer =
                          std::move(bufferDescriptor.deviceBuffer)});

              descriptorSetWrites_.push_back(
                  descriptorBinding->deviceBuffer.WriteBuffer(
                      descriptor.set, binding.type, binding.index));

              connectedBuffers.insert(
                  std::make_pair(connectionId, descriptorBinding));

              descriptor.bindings.push_back(descriptorBinding);
            }
          } else {
            BufferDescriptor bufferDescriptor = AllocateBufferDescriptor(
                binding, binding.instanced ? render.insertInfo.instances : 1);
            std::shared_ptr<DescriptorBinding> descriptorBinding =
                std::make_shared<DescriptorBinding>(DescriptorBinding{
                    .type = DescriptorBinding::Type::Buffer,
                    .hostBuffer = std::move(bufferDescriptor.hostBuffer),
                    .deviceBuffer = std::move(bufferDescriptor.deviceBuffer)});
            descriptorSetWrites_.push_back(
                descriptorBinding->deviceBuffer.WriteBuffer(
                    descriptor.set, binding.type, binding.index));
            descriptor.bindings.push_back(std::move(descriptorBinding));
          }
        } break;

        default:
          throw std::runtime_error("Unexpected descriptor binding type.");
      }
    }
    descriptors.insert(std::make_pair(render.key, std::move(descriptor)));
  }

  std::list<std::unique_ptr<Resource>> resources;

  for (auto& compute : result.computes) {
    const PipelineKey key = compute.key;
    resources.push_back(InsertCompute(key, std::move(compute.insertInfo),
                                      std::move(descriptors.at(key))));
  }

  for (auto& render : result.renders) {
    const PipelineKey key = render.key;
    resources.push_back(InsertRender(key, std::move(render.insertInfo),
                                     std::move(descriptors.at(key))));
  }

  FlushDescriptors();

  return CompositeResource(std::move(resources));
}

std::unique_ptr<Resource> RenderGraph::InsertCompute(
    const PipelineKey key, InsertComputeInfo insertInfo,
    Descriptor descriptor) {
  ComputePipeline& pipeline = computePipelineGraph_.pipelines.at(key);
  return pipeline.instances.Insert(
      {.descriptorWriter = std::move(insertInfo.descriptorWriter),
       .descriptor = std::move(descriptor)});
}

std::unique_ptr<Resource> RenderGraph::InsertRender(const PipelineKey key,
                                                    InsertRenderInfo insertInfo,
                                                    Descriptor descriptor) {
  RenderPipeline& pipeline = graphicsPipelineGraph_.pipelines.at(key);
  return pipeline.instances.Insert(
      {.visible = insertInfo.visible,
       .descriptorWriter = std::move(insertInfo.descriptorWriter),
       .descriptor = std::move(descriptor),
       .drawBuffer = insertInfo.drawBuffer,
       .drawInstances = insertInfo.instances});
}

void RenderGraph::ExecuteComputePipelines(const vk::CommandBuffer& transfer,
                                          const vk::CommandBuffer& compute,
                                          const void* const globalUniformData) {
  computePipelineGraph_.descriptor.bindings[0]->hostBuffer->MapCopy(
      globalUniformData);

  transfer.CmdCopyBufferFull(
      computePipelineGraph_.descriptor.bindings[0]->hostBuffer->RawBuffer(),
      computePipelineGraph_.descriptor.bindings[0]->deviceBuffer.RawBuffer());

  compute.CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_COMPUTE,
                               *computePipelineGraph_.rootPipelineLayout, 0,
                               computePipelineGraph_.descriptor.set);
  for (ComputePipeline& pipeline :
       IterateValues(computePipelineGraph_.pipelines)) {
    pipeline.instances.ReleasePendingResources();

    compute.CmdBindComputePipeline(pipeline.pipeline);

    const vk::PipelineLayout& layout = pipeline.pipeline.GetLayout();

    for (const ComputeInstance& instance : pipeline.instances) {
      std::unordered_map<u32, HostDescriptorWriter::Descriptor>
          availableHostDescriptors;

      for (u32 index = 0; index < instance.descriptor.bindings.size();
           ++index) {
        const auto& binding = instance.descriptor.bindings[index];

        if (binding->hostBuffer.has_value()) {
          availableHostDescriptors.insert(std::make_pair(
              index, HostDescriptorWriter::Descriptor{
                         .buffer = binding->hostBuffer.value()}));
          transfer.CmdCopyBufferFull(binding->hostBuffer->RawBuffer(),
                                     binding->deviceBuffer.RawBuffer());
        }
      }

      instance.descriptorWriter->Fill(availableHostDescriptors);

      compute.CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_COMPUTE, layout, 1,
                                   instance.descriptor.set);
      compute.CmdDispatch(1, 1, 1);
    }
  }
}

void RenderGraph::ExecuteRenderPipelines(const vk::CommandBuffer& transfer,
                                         const vk::CommandBuffer& graphics,
                                         const void* const globalUniformData,
                                         const u32 frameIndex) {
  graphicsPipelineGraph_.descriptor.bindings[0]->hostBuffer->MapCopy(
      globalUniformData);

  transfer.CmdCopyBufferFull(
      graphicsPipelineGraph_.descriptor.bindings[0]->hostBuffer->RawBuffer(),
      graphicsPipelineGraph_.descriptor.bindings[0]->deviceBuffer.RawBuffer());

  graphics.CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                *graphicsPipelineGraph_.rootPipelineLayout, 0,
                                graphicsPipelineGraph_.descriptor.set);

  for (RenderPipeline& pipeline :
       IterateValues(graphicsPipelineGraph_.pipelines)) {
    pipeline.instancesToReleasePerFrame[frameIndex] =
        std::move(pipeline.instances.AcquireResourcesPendingRelease());

    graphics.CmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS,
                             pipeline.pipeline);

    const vk::PipelineLayout& layout = pipeline.pipeline.GetLayout();

    for (const RenderInstance& instance : pipeline.instances) {
      if (!instance.visible.IsVisible()) {
        continue;
      }

      graphics.CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 1,
                                    instance.descriptor.set);

      if (instance.descriptorWriter != nullptr) {
        std::unordered_map<u32, HostDescriptorWriter::Descriptor>
            availableHostDescriptors;

        for (u32 index = 0; index < instance.descriptor.bindings.size();
             ++index) {
          const auto& binding = instance.descriptor.bindings[index];

          if (binding->hostBuffer.has_value()) {
            availableHostDescriptors.insert(std::make_pair(
                index, HostDescriptorWriter::Descriptor{
                           .buffer = binding->hostBuffer.value()}));
            transfer.CmdCopyBufferFull(binding->hostBuffer->RawBuffer(),
                                       binding->deviceBuffer.RawBuffer());
          }
        }

        instance.descriptorWriter->Fill(availableHostDescriptors);
      }

      instance.drawBuffer->Draw(graphics, instance.drawInstances);
    }
  }
}

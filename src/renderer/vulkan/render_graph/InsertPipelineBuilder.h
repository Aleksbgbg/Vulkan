#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTPIPELINEBUILDER_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTPIPELINEBUILDER_H_

#include <initializer_list>
#include <list>
#include <unordered_map>

#include "InsertComputeInfo.h"
#include "InsertRenderInfo.h"
#include "renderer/vulkan/render_graph/layout/PipelineKey.h"

struct PipelineDescriptorReference {
  PipelineDescriptorReference(PipelineKey key, u32 descriptorId);

  bool operator==(const PipelineDescriptorReference& other) const;

  PipelineKey key;
  u32 descriptorId;
};

namespace std {

template <>
class hash<PipelineDescriptorReference> {
 public:
  std::size_t operator()(const PipelineDescriptorReference& value) const;
};

}  // namespace std

class InsertPipelineBuilder {
 public:
  InsertPipelineBuilder();

  InsertPipelineBuilder&& Compute(PipelineKey key,
                                  InsertComputeInfo computeInfo);
  InsertPipelineBuilder&& Render(PipelineKey key, InsertRenderInfo renderInfo);
  InsertPipelineBuilder&& ConnectDescriptors(
      std::initializer_list<PipelineDescriptorReference> descriptorReferences);

  struct BuildResult {
    struct Compute {
      PipelineKey key;
      InsertComputeInfo insertInfo;
    };
    std::list<Compute> computes;

    struct Render {
      PipelineKey key;
      InsertRenderInfo insertInfo;
    };
    std::list<Render> renders;

    using ConnectionId = u32;

    std::unordered_map<PipelineDescriptorReference, ConnectionId>
        pipelineConnections;
  };
  BuildResult& Build();

 private:
  BuildResult::ConnectionId currentConnectionId_;
  BuildResult result_;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTPIPELINEBUILDER_H_

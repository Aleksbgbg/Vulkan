#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PHYSICALDEVICEFEATURES_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PHYSICALDEVICEFEATURES_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(PhysicalDeviceFeaturesBuilder,
                          VkPhysicalDeviceFeatures)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, RobustBufferAccess, VkBool32,
                 robustBufferAccess)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, FullDrawIndexUint32, VkBool32,
                 fullDrawIndexUint32)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ImageCubeArray, VkBool32,
                 imageCubeArray)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, IndependentBlend, VkBool32,
                 independentBlend)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, GeometryShader, VkBool32,
                 geometryShader)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, TessellationShader, VkBool32,
                 tessellationShader)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SampleRateShading, VkBool32,
                 sampleRateShading)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, DualSrcBlend, VkBool32,
                 dualSrcBlend)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, LogicOp, VkBool32, logicOp)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, MultiDrawIndirect, VkBool32,
                 multiDrawIndirect)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, DrawIndirectFirstInstance,
                 VkBool32, drawIndirectFirstInstance)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, DepthClamp, VkBool32,
                 depthClamp)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, DepthBiasClamp, VkBool32,
                 depthBiasClamp)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, FillModeNonSolid, VkBool32,
                 fillModeNonSolid)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, DepthBounds, VkBool32,
                 depthBounds)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, WideLines, VkBool32, wideLines)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, LargePoints, VkBool32,
                 largePoints)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, AlphaToOne, VkBool32,
                 alphaToOne)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, MultiViewport, VkBool32,
                 multiViewport)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SamplerAnisotropy, VkBool32,
                 samplerAnisotropy)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, TextureCompressionETC2,
                 VkBool32, textureCompressionETC2)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, TextureCompressionASTC_LDR,
                 VkBool32, textureCompressionASTC_LDR)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, TextureCompressionBC, VkBool32,
                 textureCompressionBC)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, OcclusionQueryPrecise, VkBool32,
                 occlusionQueryPrecise)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, PipelineStatisticsQuery,
                 VkBool32, pipelineStatisticsQuery)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, VertexPipelineStoresAndAtomics,
                 VkBool32, vertexPipelineStoresAndAtomics)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, FragmentStoresAndAtomics,
                 VkBool32, fragmentStoresAndAtomics)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder,
                 ShaderTessellationAndGeometryPointSize, VkBool32,
                 shaderTessellationAndGeometryPointSize)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ShaderImageGatherExtended,
                 VkBool32, shaderImageGatherExtended)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder,
                 ShaderStorageImageExtendedFormats, VkBool32,
                 shaderStorageImageExtendedFormats)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ShaderStorageImageMultisample,
                 VkBool32, shaderStorageImageMultisample)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder,
                 ShaderStorageImageReadWithoutFormat, VkBool32,
                 shaderStorageImageReadWithoutFormat)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder,
                 ShaderStorageImageWriteWithoutFormat, VkBool32,
                 shaderStorageImageWriteWithoutFormat)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder,
                 ShaderUniformBufferArrayDynamicIndexing, VkBool32,
                 shaderUniformBufferArrayDynamicIndexing)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder,
                 ShaderSampledImageArrayDynamicIndexing, VkBool32,
                 shaderSampledImageArrayDynamicIndexing)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder,
                 ShaderStorageBufferArrayDynamicIndexing, VkBool32,
                 shaderStorageBufferArrayDynamicIndexing)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder,
                 ShaderStorageImageArrayDynamicIndexing, VkBool32,
                 shaderStorageImageArrayDynamicIndexing)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ShaderClipDistance, VkBool32,
                 shaderClipDistance)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ShaderCullDistance, VkBool32,
                 shaderCullDistance)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ShaderFloat64, VkBool32,
                 shaderFloat64)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ShaderInt64, VkBool32,
                 shaderInt64)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ShaderInt16, VkBool32,
                 shaderInt16)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ShaderResourceResidency,
                 VkBool32, shaderResourceResidency)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, ShaderResourceMinLod, VkBool32,
                 shaderResourceMinLod)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SparseBinding, VkBool32,
                 sparseBinding)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SparseResidencyBuffer, VkBool32,
                 sparseResidencyBuffer)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SparseResidencyImage2D,
                 VkBool32, sparseResidencyImage2D)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SparseResidencyImage3D,
                 VkBool32, sparseResidencyImage3D)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SparseResidency2Samples,
                 VkBool32, sparseResidency2Samples)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SparseResidency4Samples,
                 VkBool32, sparseResidency4Samples)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SparseResidency8Samples,
                 VkBool32, sparseResidency8Samples)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SparseResidency16Samples,
                 VkBool32, sparseResidency16Samples)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, SparseResidencyAliased,
                 VkBool32, sparseResidencyAliased)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, VariableMultisampleRate,
                 VkBool32, variableMultisampleRate)
STRUCTURE_SETTER(PhysicalDeviceFeaturesBuilder, InheritedQueries, VkBool32,
                 inheritedQueries)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PHYSICALDEVICEFEATURES_H

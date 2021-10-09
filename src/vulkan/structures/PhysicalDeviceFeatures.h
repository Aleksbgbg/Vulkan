#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PHYSICALDEVICEFEATURES_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PHYSICALDEVICEFEATURES_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_STYPE(PhysicalDeviceFeaturesBuilder,
                           VkPhysicalDeviceFeatures)
STRUCTURE_SETTER(RobustBufferAccess, VkBool32, robustBufferAccess)
STRUCTURE_SETTER(FullDrawIndexUint32, VkBool32, fullDrawIndexUint32)
STRUCTURE_SETTER(ImageCubeArray, VkBool32, imageCubeArray)
STRUCTURE_SETTER(IndependentBlend, VkBool32, independentBlend)
STRUCTURE_SETTER(GeometryShader, VkBool32, geometryShader)
STRUCTURE_SETTER(TessellationShader, VkBool32, tessellationShader)
STRUCTURE_SETTER(SampleRateShading, VkBool32, sampleRateShading)
STRUCTURE_SETTER(DualSrcBlend, VkBool32, dualSrcBlend)
STRUCTURE_SETTER(LogicOp, VkBool32, logicOp)
STRUCTURE_SETTER(MultiDrawIndirect, VkBool32, multiDrawIndirect)
STRUCTURE_SETTER(DrawIndirectFirstInstance, VkBool32, drawIndirectFirstInstance)
STRUCTURE_SETTER(DepthClamp, VkBool32, depthClamp)
STRUCTURE_SETTER(DepthBiasClamp, VkBool32, depthBiasClamp)
STRUCTURE_SETTER(FillModeNonSolid, VkBool32, fillModeNonSolid)
STRUCTURE_SETTER(DepthBounds, VkBool32, depthBounds)
STRUCTURE_SETTER(WideLines, VkBool32, wideLines)
STRUCTURE_SETTER(LargePoints, VkBool32, largePoints)
STRUCTURE_SETTER(AlphaToOne, VkBool32, alphaToOne)
STRUCTURE_SETTER(MultiViewport, VkBool32, multiViewport)
STRUCTURE_SETTER(SamplerAnisotropy, VkBool32, samplerAnisotropy)
STRUCTURE_SETTER(TextureCompressionETC2, VkBool32, textureCompressionETC2)
STRUCTURE_SETTER(TextureCompressionASTC_LDR, VkBool32,
                 textureCompressionASTC_LDR)
STRUCTURE_SETTER(TextureCompressionBC, VkBool32, textureCompressionBC)
STRUCTURE_SETTER(OcclusionQueryPrecise, VkBool32, occlusionQueryPrecise)
STRUCTURE_SETTER(PipelineStatisticsQuery, VkBool32, pipelineStatisticsQuery)
STRUCTURE_SETTER(VertexPipelineStoresAndAtomics, VkBool32,
                 vertexPipelineStoresAndAtomics)
STRUCTURE_SETTER(FragmentStoresAndAtomics, VkBool32, fragmentStoresAndAtomics)
STRUCTURE_SETTER(ShaderTessellationAndGeometryPointSize, VkBool32,
                 shaderTessellationAndGeometryPointSize)
STRUCTURE_SETTER(ShaderImageGatherExtended, VkBool32, shaderImageGatherExtended)
STRUCTURE_SETTER(ShaderStorageImageExtendedFormats, VkBool32,
                 shaderStorageImageExtendedFormats)
STRUCTURE_SETTER(ShaderStorageImageMultisample, VkBool32,
                 shaderStorageImageMultisample)
STRUCTURE_SETTER(ShaderStorageImageReadWithoutFormat, VkBool32,
                 shaderStorageImageReadWithoutFormat)
STRUCTURE_SETTER(ShaderStorageImageWriteWithoutFormat, VkBool32,
                 shaderStorageImageWriteWithoutFormat)
STRUCTURE_SETTER(ShaderUniformBufferArrayDynamicIndexing, VkBool32,
                 shaderUniformBufferArrayDynamicIndexing)
STRUCTURE_SETTER(ShaderSampledImageArrayDynamicIndexing, VkBool32,
                 shaderSampledImageArrayDynamicIndexing)
STRUCTURE_SETTER(ShaderStorageBufferArrayDynamicIndexing, VkBool32,
                 shaderStorageBufferArrayDynamicIndexing)
STRUCTURE_SETTER(ShaderStorageImageArrayDynamicIndexing, VkBool32,
                 shaderStorageImageArrayDynamicIndexing)
STRUCTURE_SETTER(ShaderClipDistance, VkBool32, shaderClipDistance)
STRUCTURE_SETTER(ShaderCullDistance, VkBool32, shaderCullDistance)
STRUCTURE_SETTER(ShaderFloat64, VkBool32, shaderFloat64)
STRUCTURE_SETTER(ShaderInt64, VkBool32, shaderInt64)
STRUCTURE_SETTER(ShaderInt16, VkBool32, shaderInt16)
STRUCTURE_SETTER(ShaderResourceResidency, VkBool32, shaderResourceResidency)
STRUCTURE_SETTER(ShaderResourceMinLod, VkBool32, shaderResourceMinLod)
STRUCTURE_SETTER(SparseBinding, VkBool32, sparseBinding)
STRUCTURE_SETTER(SparseResidencyBuffer, VkBool32, sparseResidencyBuffer)
STRUCTURE_SETTER(SparseResidencyImage2D, VkBool32, sparseResidencyImage2D)
STRUCTURE_SETTER(SparseResidencyImage3D, VkBool32, sparseResidencyImage3D)
STRUCTURE_SETTER(SparseResidency2Samples, VkBool32, sparseResidency2Samples)
STRUCTURE_SETTER(SparseResidency4Samples, VkBool32, sparseResidency4Samples)
STRUCTURE_SETTER(SparseResidency8Samples, VkBool32, sparseResidency8Samples)
STRUCTURE_SETTER(SparseResidency16Samples, VkBool32, sparseResidency16Samples)
STRUCTURE_SETTER(SparseResidencyAliased, VkBool32, sparseResidencyAliased)
STRUCTURE_SETTER(VariableMultisampleRate, VkBool32, variableMultisampleRate)
STRUCTURE_SETTER(InheritedQueries, VkBool32, inheritedQueries)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PHYSICALDEVICEFEATURES_H

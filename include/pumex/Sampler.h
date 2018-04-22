//
// Copyright(c) 2017-2018 Pawe� Ksi�opolski ( pumexx )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#pragma once
#include <unordered_map>
#include <memory>
#include <mutex>
#include <vulkan/vulkan.h>
#include <gli/texture.hpp>
#include <pumex/Export.h>
#include <pumex/Resource.h>

namespace pumex
{

class RenderContext;

// struct describing VkSampler / combined sampler 
struct PUMEX_EXPORT SamplerTraits
{
  explicit SamplerTraits(bool linearTiling = false, VkFilter magFilter = VK_FILTER_LINEAR, VkFilter minFilter = VK_FILTER_LINEAR, VkSamplerMipmapMode mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
    VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    float mipLodBias = 0.0f, VkBool32 anisotropyEnable = VK_TRUE, float maxAnisotropy = 8, VkBool32 compareEnable = false, VkCompareOp compareOp = VK_COMPARE_OP_NEVER, 
    float minLod = 0.0f, float maxLod = 10.0f, VkBorderColor borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE, VkBool32 unnormalizedCoordinates = false);

  bool                    linearTiling            = false; 
  VkFilter                magFilter               = VK_FILTER_LINEAR;
  VkFilter                minFilter               = VK_FILTER_LINEAR;
  VkSamplerMipmapMode     mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  VkSamplerAddressMode    addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  VkSamplerAddressMode    addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  VkSamplerAddressMode    addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  float                   mipLodBias              = 0.0f;
  VkBool32                anisotropyEnable        = VK_TRUE;
  float                   maxAnisotropy           = 8;
  VkBool32                compareEnable           = false;
  VkCompareOp             compareOp               = VK_COMPARE_OP_NEVER;
  float                   minLod                  = 0.0f;
  float                   maxLod                  = 10.0f; // FIXME
  VkBorderColor           borderColor             = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
  VkBool32                unnormalizedCoordinates = false;
};

class PUMEX_EXPORT Sampler : public Resource
{
public:
  Sampler()                          = delete;
  // create single texture and clear it with specific value
  explicit Sampler(const SamplerTraits& samplerTraits, Resource::SwapChainImageBehaviour swapChainImageBehaviour = Resource::ForEachSwapChainImage);
  Sampler(const Sampler&)            = delete;
  Sampler& operator=(const Sampler&) = delete;

  VkSampler                         getHandleSampler(const RenderContext& renderContext) const;
  inline const SamplerTraits&       getSamplerTraits() const;

  std::pair<bool, VkDescriptorType> getDefaultDescriptorType() override;
  void                              validate(const RenderContext& renderContext) override;
  void                              invalidate() override;
  DescriptorSetValue                getDescriptorSetValue(const RenderContext& renderContext) override;


protected:
  struct PerDeviceData
  {
    PerDeviceData(uint32_t ac)
    {
      resize(ac);
    }
    void resize(uint32_t ac)
    {
      valid.resize(ac, false);
      sampler.resize(ac, VK_NULL_HANDLE);
    }

    std::vector<bool>      valid;
    std::vector<VkSampler> sampler;
  };

  std::unordered_map<VkDevice, PerDeviceData> perDeviceData;
  SamplerTraits                               samplerTraits;
};

const SamplerTraits& Sampler::getSamplerTraits() const { return samplerTraits; }

}
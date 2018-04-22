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
#include <pumex/Export.h>
#include <pumex/Resource.h>
#include <pumex/Image.h>

namespace pumex
{

class RenderContext;
class Sampler;

// Uses gli::texture to hold texture on CPU
// Texture may contain usual textures, texture arrays, texture cubes, arrays of texture cubes etc, but cubes were not tested in real life ( be aware )
// Texture may be used in a descriptor as as sampled image, combined image sampler and image store
// Class stores information about images PER DEVICE. Additionally it also stores a VkSampler
class PUMEX_EXPORT Texture : public Resource
{
public:
  Texture()                          = delete;
  // create single texture and clear it with specific value
  explicit Texture(const ImageTraits& imageTraits, std::shared_ptr<Sampler> sampler, std::shared_ptr<DeviceMemoryAllocator> allocator, VkClearValue initValue, Resource::SwapChainImageBehaviour swapChainImageBehaviour = Resource::ForEachSwapChainImage);
  // create single texture and load it with provided data ( gli::texture )
  explicit Texture(std::shared_ptr<gli::texture> texture, std::shared_ptr<Sampler> sampler, std::shared_ptr<DeviceMemoryAllocator> allocator, VkImageUsageFlags usage, Resource::SwapChainImageBehaviour swapChainImageBehaviour = Resource::ForEachSwapChainImage);
  Texture(const Texture&)            = delete;
  Texture& operator=(const Texture&) = delete;
  virtual ~Texture();

  Image*                          getHandleImage(const RenderContext& renderContext) const;
  inline const ImageTraits&       getImageTraits() const;
  inline std::shared_ptr<Sampler> getSampler() const;

  void                            validate(const RenderContext& renderContext) override;
  void                            invalidate() override;
  DescriptorSetValue              getDescriptorSetValue(const RenderContext& renderContext) override;

  void                            setLayer(uint32_t layer, std::shared_ptr<gli::texture> tex);

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
      image.resize(ac, nullptr);
    }

    std::vector<bool>                   valid;
    std::vector<std::shared_ptr<Image>> image;
  };
  void buildImageTraits(VkImageUsageFlags usage);

  std::unordered_map<VkDevice, PerDeviceData> perDeviceData;
  ImageTraits                                 imageTraits;
  std::shared_ptr<Sampler>                    sampler;
  std::shared_ptr<gli::texture>               texture;
  VkClearValue                                initValue;
  std::shared_ptr<DeviceMemoryAllocator>      allocator;
};

const ImageTraits&              Texture::getImageTraits() const { return imageTraits; }
inline std::shared_ptr<Sampler> Texture::getSampler() const     { return sampler; }

}


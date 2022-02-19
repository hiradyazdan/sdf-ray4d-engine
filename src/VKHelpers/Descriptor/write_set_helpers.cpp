/*****************************************************
 * Partial Class: DescriptorHelper
 * Members: Write Descriptor Set Helpers (Public)
 *****************************************************/

#include "VKHelpers/Descriptor.hpp"

using namespace sdfRay4d::vkHelpers;

/**
 * @brief add write descriptor set buffer (overload)
 *
 * @note since bufferInfo in this function can potentially
 * use multiple material data, or each material can have
 * multiple/different bufferInfo data, it cannot be part
 * of a single material and, therefore its data/pointer
 * needs to be handled per descriptor than per material scope
 *
 * @param[in] _descSet
 * @param[in] _layoutBinding
 * @param[in] _bufferInfo
 */
void DescriptorHelper::addWriteSet(
  const descriptor::Set &_descSet,
  const descriptor::LayoutBinding &_layoutBinding,
  const descriptor::BufferInfo &_bufferInfo
) noexcept
{
  /**
   * @note fixes use-after-free pointer bug using a custom set, by: \n
   * - keeping bufferInfo while descriptor is in memory \n
   * - avoiding to overwrite previous bufferInfo on repeated function calls
   */
  m_bufferInfoPtrSet.push_back(
    std::make_unique<descriptor::BufferInfo>()
  );

  auto &bufferInfo  = m_bufferInfoPtrSet.back();

  bufferInfo->buffer              = _bufferInfo.buffer;
  bufferInfo->offset              = _bufferInfo.offset;
  bufferInfo->range               = _bufferInfo.range;

  descriptor::Write descriptor    = {}; // memset

  descriptor.sType                = descriptor::StructureType::WRITE_SET;
  descriptor.dstSet               = _descSet;
  descriptor.dstBinding           = _layoutBinding.binding;
  descriptor.descriptorCount      = _layoutBinding.descriptorCount;
  descriptor.descriptorType       = _layoutBinding.descriptorType;
  descriptor.pBufferInfo          = bufferInfo.get();

  m_descWriteList.push_back(descriptor);
}

/**
 * @brief add write descriptor set image (overload)
 *
 * @note since imageInfo in this function can potentially
 * use multiple material data, or each material can have
 * multiple/different imageInfo data, it cannot be part
 * of a single material and, therefore its data/pointer
 * needs to be handled per descriptor than per material scope
 *
 * @param[in] _descSet
 * @param[in] _layoutBinding
 * @param[in] _imageInfo
 */
void DescriptorHelper::addWriteSet(
  const descriptor::Set &_descSet,
  const descriptor::LayoutBinding &_layoutBinding,
  const descriptor::ImageInfo &_imageInfo
) noexcept
{
  /**
   * @note fixes use-after-free pointer bug using a custom set, by: \n
   * - keeping imageInfo while descriptor is in memory \n
   * - avoiding to overwrite previous imageInfo on repeated function calls
   */
  m_imageInfoPtrSet.push_back(
    std::make_unique<descriptor::ImageInfo>()
  );

  auto &imageInfo   = m_imageInfoPtrSet.back();

  imageInfo->sampler              = _imageInfo.sampler;
  imageInfo->imageView            = _imageInfo.imageView;
  imageInfo->imageLayout          = _imageInfo.imageLayout;

  descriptor::Write descriptor    = {};

  descriptor.sType                = descriptor::StructureType::WRITE_SET;
  descriptor.dstSet               = _descSet;
  descriptor.dstBinding           = _layoutBinding.binding;
  descriptor.descriptorCount      = _layoutBinding.descriptorCount;
  descriptor.descriptorType       = _layoutBinding.descriptorType;
  descriptor.pImageInfo           = imageInfo.get();

  m_descWriteList.push_back(descriptor);
}

/*****************************************************
 * Partial Class: DescriptorHelper
 * Members: Write Descriptor Set Helpers (Public)
 *****************************************************/

#include "Helpers/Descriptor.hpp"

using namespace sdfRay4d::helpers;

/**
 * @brief add write descriptor set buffer (overload)
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
  descriptor::Write descriptor = {};

  descriptor.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptor.dstSet           = _descSet;
  descriptor.dstBinding       = _layoutBinding.binding;
  descriptor.descriptorCount  = _layoutBinding.descriptorCount;
  descriptor.descriptorType   = _layoutBinding.descriptorType;
  descriptor.pBufferInfo      = &_bufferInfo;

  m_descWriteList.push_back(descriptor);
}

/**
 * @brief add write descriptor set image (overload)
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
  descriptor::Write descriptor = {};

  descriptor.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptor.dstSet           = _descSet;
  descriptor.dstBinding       = _layoutBinding.binding;
  descriptor.descriptorCount  = _layoutBinding.descriptorCount;
  descriptor.descriptorType   = _layoutBinding.descriptorType;
  descriptor.pImageInfo       = &_imageInfo;

  m_descWriteList.push_back(descriptor);
}

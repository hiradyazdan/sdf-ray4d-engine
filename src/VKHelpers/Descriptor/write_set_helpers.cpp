/*****************************************************
 * Partial Class: DescriptorHelper
 * Members: Write Descriptor Set Helpers (Public)
 *****************************************************/

#include "VKHelpers/Descriptor.hpp"

using namespace sdfRay4d::vkHelpers;

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
  /**
   * @note avoids going out of scope
   */
  m_bufferInfo = _bufferInfo;

  descriptor::Write descriptor  = {}; // memset

  descriptor.sType              = descriptor::StructureType::WRITE_SET;
  descriptor.dstSet             = _descSet;
  descriptor.dstBinding         = _layoutBinding.binding;
  descriptor.descriptorCount    = _layoutBinding.descriptorCount;
  descriptor.descriptorType     = _layoutBinding.descriptorType;
  descriptor.pBufferInfo        = &m_bufferInfo;

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
  /**
   * @note avoids going out of scope
   */
  m_imageInfo = _imageInfo;

  descriptor::Write descriptor  = {};

  descriptor.sType              = descriptor::StructureType::WRITE_SET;
  descriptor.dstSet             = _descSet;
  descriptor.dstBinding         = _layoutBinding.binding;
  descriptor.descriptorCount    = _layoutBinding.descriptorCount;
  descriptor.descriptorType     = _layoutBinding.descriptorType;
  descriptor.pImageInfo         = &m_imageInfo;

  m_descWriteList.push_back(descriptor);
}

/*****************************************************
 * Partial Class: DescriptorHelper (Ctor & General)
 * Members: General Functions (Public)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 * - create_set_helpers.cpp
 * - write_set_helpers.cpp
 *****************************************************/

#include "Helpers/Descriptor.hpp"

using namespace sdfRay4d::helpers;

/**
 *
 * @param[in] _device
 * @param[in] _deviceFuncs
 */
DescriptorHelper::DescriptorHelper(
  const Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs
) noexcept :
  m_device(_device)
, m_deviceFuncs(_deviceFuncs)
{}

void DescriptorHelper::updateDescriptorSets() noexcept
{
  m_deviceFuncs->vkUpdateDescriptorSets(
    m_device,
    m_descWriteList.size(),
    m_descWriteList.data(),
    0,
    nullptr
  );
}

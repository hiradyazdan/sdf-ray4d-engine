#pragma once

#include "BaseHelper.hpp"

namespace sdfRay4d::helpers
{
  /**
   * @class DescriptorHelper
   * @brief
   *
   * @example
   *
   */
  class DescriptorHelper : protected BaseHelper
  {
    friend class PipelineHelper;

    public:
      DescriptorHelper(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs
      ) noexcept;

    /**
     * @note DescriptorHelper is non-copyable
     */
    public:
      DescriptorHelper() = default;
      DescriptorHelper(const DescriptorHelper&) = delete;

    public:
      void updateDescriptorSets() noexcept;
      void addWriteSet(
        const descriptor::Set &_descSet,
        const descriptor::LayoutBinding &_layoutBinding,
        const descriptor::BufferInfo &_bufferInfo
      ) noexcept;
      void addWriteSet(
        const descriptor::Set &_descSet,
        const descriptor::LayoutBinding &_layoutBinding,
        const descriptor::ImageInfo &_imageInfo
      ) noexcept;

    /**
     * Create Descriptor Sets/Layouts: per Material Pipeline
     * -------------------------------------------------
     */
    private:
      void createDescriptorSets       (const MaterialPtr &_material) noexcept;
      void createDescriptorPool       (const MaterialPtr &_material) noexcept;
      void createDescriptorSetLayout  (const MaterialPtr &_material) noexcept;
      void allocateDescriptorSets     (const MaterialPtr &_material) noexcept;

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      std::vector<descriptor::Write> m_descWriteList = {};
  };
}

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

//    using LayoutBindingList = std::vector<descriptor::LayoutBinding>;

    public:
      DescriptorHelper(
        const Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs
      );

    /**
     * @note DescriptorHelper is non-copyable
     */
    public:
      DescriptorHelper() = default;
      DescriptorHelper(const DescriptorHelper&) = delete;

    public:
      void updateDescriptorSets();
      void addWriteSet(
        const descriptor::Set &_descSet,
        const descriptor::LayoutBinding &_layoutBinding,
        const descriptor::BufferInfo &_bufferInfo
      );
      void addWriteSet(
        const descriptor::Set &_descSet,
        const descriptor::LayoutBinding &_layoutBinding,
        const descriptor::ImageInfo &_imageInfo
      );

    /**
     * Create Descriptor Sets/Layouts: per Material Pipeline
     * -------------------------------------------------
     */
    private:
      void createDescriptorSets       (const MaterialPtr &_material);
      void createDescriptorPool       (const MaterialPtr &_material);
      void createDescriptorSetLayout  (const MaterialPtr &_material);
      void allocateDescriptorSets     (const MaterialPtr &_material);

    private:
      Device m_device = VK_NULL_HANDLE;
      QVulkanDeviceFunctions *m_deviceFuncs = VK_NULL_HANDLE;

      std::vector<descriptor::Write> m_descWriteList = {};
  };
}

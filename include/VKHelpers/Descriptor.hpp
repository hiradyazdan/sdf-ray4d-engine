#pragma once

#include "BaseHelper.hpp"

namespace sdfRay4d::vkHelpers
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

    using BufferInfoPtr = std::unique_ptr<descriptor::BufferInfo>;
    using ImageInfoPtr  = std::unique_ptr<descriptor::ImageInfo>;

    /**
     * TODO: implement custom move ctors and operator overloads while removing copy ctors
     */
    public:
      DescriptorHelper() = default;
      DescriptorHelper(
        const device::Device &_device,
        QVulkanDeviceFunctions *_deviceFuncs
      ) noexcept;

    /**
     * Add WriteSet Overloads (buffer and image infos)
     * -------------------------------------------------
     *
     */
    public:
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

    public:
      void updateDescriptorSets() noexcept;

    /**
     * Create Descriptor Sets/Layouts: per Material Pipeline
     * -------------------------------------------------
     *
     */
    private:
      void createDescriptorPool       (const MaterialPtr &_material) noexcept;
      void createDescriptorSetLayouts (const MaterialPtr &_material) noexcept;
      void allocateDescriptorSets     (const MaterialPtr &_material) noexcept;
      void createDescriptorSets       (const MaterialPtr &_material) noexcept;

    private:
      device::Device                  m_device        = VK_NULL_HANDLE;
      QVulkanDeviceFunctions          *m_deviceFuncs  = VK_NULL_HANDLE;

      std::vector<descriptor::Write>  m_descWriteList = {};

      std::vector<BufferInfoPtr>      m_bufferInfoPtrSet;
      std::vector<ImageInfoPtr>       m_imageInfoPtrSet;
  };
}

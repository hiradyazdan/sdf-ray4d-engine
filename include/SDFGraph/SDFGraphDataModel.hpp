#pragma once

#include <nodes/NodeDataModel>
#include <nodes/NodeData>

#include "Util.hpp"

#include "MapData.hpp"

namespace sdfRay4d::sdfGraph
{
  using namespace QtNodes;

  /**
   * @class
   * @brief Abstract Class + Interface
   */
  class SDFGraphDataModel : public NodeDataModel
  {
    /**
     * Abstract Class Members
     * -------------------------------------------------
     *
     */
    public:
      virtual std::string getExtraParams() const { return ""; }
//      virtual Mat4f getTransform() { return Mat4f(); }
//      virtual void setTransform(const Mat4f &_transform) {}
//      virtual QMatrix4x4 getTransform() { return QMatrix4x4(); }
//      virtual void setTransform(const QMatrix4x4 &_transform) {}

    /**
     * Interface Members
     * -------------------------------------------------
     *
     */
    public:
      virtual QString getShaderCode() = 0;
      virtual QString getData() = 0;
//      std::shared_ptr<NodeData> outData(PortIndex _port) override;
//      void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;
//      virtual DFNodeType getNodeType() const = 0;
  };
}

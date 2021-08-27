#pragma once

#include <iostream>

#include <nodes/NodeDataModel>
#include <nodes/NodeData>

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>
#include <QtGui/QDoubleValidator>

#include "SDFGraph/Util.hpp"

#include "SDFGraph/ShapeData.hpp"
#include "SDFGraph/OperationData.hpp"
#include "SDFGraph/MapData.hpp"

namespace sdfRay4d::sdfGraph
{
  using namespace QtNodes;

  /**
   * @class
   * @brief Abstract Class + Interface
   */
  class BaseDataModel : public NodeDataModel
  {
    Q_OBJECT

    using NodePtr    = std::unique_ptr<Node>;
    using NodePtrSet = std::unordered_map<QUuid, NodePtr>;

    /**
     * Abstract Class Members (Overrides)
     * -------------------------------------------------
     *
     */
    public:
      [[nodiscard]] bool captionVisible() const override { return true; }
      [[nodiscard]] NodeValidationState validationState() const override { return modelValidationState; }
      [[nodiscard]] QString validationMessage() const override { return modelValidationError; }

    /**
     * Abstract Class Members (Virtual)
     * -------------------------------------------------
     *
     */
    public:

    /**
     * Interface Members (explicit abstract member functions)
     * -------------------------------------------------
     *
     */
    public:
      virtual QString getData() = 0;

    protected:
      NodeValidationState modelValidationState = NodeValidationState::Warning;
      QString modelValidationError = QString("Missing or incorrect inputs");
  };
}

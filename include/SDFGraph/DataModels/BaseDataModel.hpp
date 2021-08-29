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

#include "SDFGraph/Interfaces/IDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  using namespace QtNodes;
  using NodeDataPtr = std::shared_ptr<NodeData>;

  /**
   * @class BaseDataModel
   * @brief This class is used as the super class for all subclass data models
   * inheriting from NodeDataModel and forces its subclasses to implement IDataModel interface
   */
  class BaseDataModel : public NodeDataModel, public IDataModel
  {
    Q_OBJECT

    public:
      QWidget *embeddedWidget() override { return nullptr; }

      [[nodiscard]] bool captionVisible() const override { return true; }
      [[nodiscard]] NodeValidationState validationState() const override { return m_validationState; }
      [[nodiscard]] QString validationMessage() const override { return m_validationError; }

    protected:
      NodeValidationState m_validationState = NodeValidationState::Warning;
      QString m_validationError = QString("Missing or incorrect inputs");
  };
}

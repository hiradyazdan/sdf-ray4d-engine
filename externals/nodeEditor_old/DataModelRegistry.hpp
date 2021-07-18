#pragma once

#include <unordered_map>

#include <QtCore/QString>

#include "NodeDataModel.hpp"
#include "Export.hpp"
#include "UniquePtr.hpp"
#include "QStringStdHash.hpp"

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor
/// Modified by Teemu Lindborg & Phil Gifford

/// Base abstract class for Model Registry items
///

class RegistryItem
{
public:
  RegistryItem() {}

  virtual
  ~RegistryItem() {}

  virtual std::unique_ptr<NodeDataModel> create() const = 0;

  virtual QString name() const = 0;
};

//------------------------------------------------------------------------------

/// Encapsulate templated concrete Model type T
template<typename T>
class RegistryItemImpl : public RegistryItem
{

public:

  /// Gives derived classes the ability to create instances of T
  std::unique_ptr<NodeDataModel>create() const override { return std::make_unique<T>(); }

  QString name() const override { return T::name(); }
};

//------------------------------------------------------------------------------

/// Class uses static map for storing models (name, model)
class NODE_EDITOR_PUBLIC DataModelRegistry
{

public:

  using RegistryItemPtr     = std::unique_ptr<RegistryItem>;
	using RegisteredModelsMap = std::map<QString, std::map<QString ,RegistryItemPtr>>;

public:

  template<typename ModelType>
  static void
  registerModel(QString category = QString(), QString menuName = QString())
  {
    QString const name = ModelType::name();

    if (_registeredModels.count(name) == 0)
    {
      auto uniqueModel = std::make_unique<RegistryItemImpl<ModelType> > ();

      _registeredModels[category][uniqueModel->name()] = std::move(uniqueModel);
    }
  }

  static std::unique_ptr<NodeDataModel> create(const QString &catName, QString const &modelName);
  static RegisteredModelsMap const &registeredModels();


private:
  static RegisteredModelsMap _registeredModels;
};

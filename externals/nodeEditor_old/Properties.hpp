#pragma once

#include <unordered_map>

#include <QtCore/QVariantMap>

#include "Export.hpp"

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor

class NODE_EDITOR_PUBLIC Properties
{
public:

  void
  put(QString const &name, QVariant const &v);

  template <typename T>
  bool
  get(QString name, T* v) const
  {
    QVariant const &var = _values[name];

    if (var.canConvert<T>())
    {
      *v = _values[name].value<T>();

      return true;
    }

    return false;
  }

  QVariantMap const &
  values() const
  { return _values; }

  QVariantMap &
  values()
  { return _values; }

private:

  QVariantMap _values;
};

#pragma once

#include "Properties.hpp"

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor

class Serializable
{
public:

  virtual void
  save(Properties & p) const = 0;

  virtual void
  restore(Properties const &p) {}
};

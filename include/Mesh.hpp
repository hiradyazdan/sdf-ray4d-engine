#pragma once

#include <QString>
#include <QFuture>

#include "Types.hpp"

namespace sdfRay4d
{
  /**
   * @todo Rewrite this
   */

  class Mesh
  {
    public:
      struct Data
      {
        bool isValid() const { return vertexCount > 0; }
        int vertexCount = 0;
        float aabb[6];
        QByteArray geom; // x, y, z, u, v, nx, ny, nz
      };

    public:
      void load(const QString &_fileName);
      Data *data();
      bool isValid() { return data()->isValid(); }
      void reset();

    private:
      bool m_isLoading = false;
      QFuture<Data> m_worker;
      Data m_data;
  };
}

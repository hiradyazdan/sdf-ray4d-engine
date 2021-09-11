/*****************************************************
 * Class: Mesh (General)
 * Members: General Functions (Public)
 * Partials: None
 *****************************************************/

#include <QtConcurrentRun>
#include <QFile>

#include "Mesh.hpp"

using namespace sdfRay4d;

/**
 *
 * @param[in] _fileName
 */
void Mesh::load(const QString &_fileName)
{
  reset();

  m_isLoading = true;

  m_worker = QtConcurrent::run([_fileName]()
  {
    Data md;
    QFile f(_fileName);
    if (!f.open(QIODevice::ReadOnly)) {
      qWarning("Failed to open %s", qPrintable(_fileName));
      return md;
    }
    QByteArray buf = f.readAll();
    const char *p = buf.constData();
    quint32 format;
    memcpy(&format, p, 4);

    if (format != 1)
    {
      qWarning("Invalid format in %s", qPrintable(_fileName));
      return md;
    }
    int ofs = 4;
    memcpy(&md.vertexCount, p + ofs, 4);
    ofs += 4;
    memcpy(md.aabb, p + ofs, 6 * 4);
    ofs += 6 * 4;
    const int byteCount = md.vertexCount * 8 * 4;
    md.geom.resize(byteCount);
    memcpy(md.geom.data(), p + ofs, byteCount);
    return md;
  });
}

/**
 *
 * @return Mesh::Data instance
 */
Mesh::Data *Mesh::data()
{
  if (m_isLoading && !m_data.isValid())
    m_data = m_worker.result();

  return &m_data;
}

void Mesh::reset()
{
  *data() = Data();
  m_isLoading = false;
}

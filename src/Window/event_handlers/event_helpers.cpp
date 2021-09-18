/*****************************************************
 * Partial Class: MainWindow
 * Members: Event Helpers (Private)
 *****************************************************/

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

/**
 *
 * @param[in] _area
 */
void MainWindow::resizeSDFGraph(Qt::DockWidgetArea _area)
{
  if(m_sdfGraphWidget->isFloating()) return;

  const auto &width = m_windowSize.width();
  const auto &height = m_windowSize.height();

  const auto &isTopBottom = _area == Qt::TopDockWidgetArea  || _area == Qt::BottomDockWidgetArea;
  const auto &isLeftRight = _area == Qt::LeftDockWidgetArea || _area == Qt::RightDockWidgetArea;

  if(isTopBottom)
  {
    m_sdfGraphWidget->setMaximumSize(width, height / 2);
    m_sdfGraphWidget->setMinimumSize(width, height / 3);
  }

  if(isLeftRight)
  {
    m_sdfGraphWidget->setMaximumSize(width / 2, height);
    m_sdfGraphWidget->setMinimumSize(width / 3, height);
  }
}

void MainWindow::resizeSDFGraphFloating()
{
  const auto &width = m_windowSize.width();
  const auto &height = m_windowSize.height();

  m_sdfGraphWidget->setFixedSize(int(width / 1.5), int(height / 1.5));
  m_sdfGraphWidget->setMaximumSize(width, height);
}
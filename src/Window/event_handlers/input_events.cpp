/*****************************************************
 * Partial Class: VulkanWindow
 * Members: Event Handlers - Input Key Events (Public)
 *****************************************************/

#include <QMouseEvent>
#include <QtWidgets/QApplication>

#include "Window/MainWindow.hpp"
#include "Window/VulkanWindow.hpp"

using namespace sdfRay4d;

void VulkanWindow::mousePressEvent(QMouseEvent *_event)
{
  m_pressed = true;
  m_lastPos = _event->pos();
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent *_event)
{
  m_pressed = false;
}

void VulkanWindow::mouseMoveEvent(QMouseEvent *_event)
{
  if (!m_pressed) return;

  int dx = _event->pos().x() - m_lastPos.x();
  int dy = _event->pos().y() - m_lastPos.y();

  if (dy)
    m_renderer->pitch(dy / 10.0f);

  if (dx)
    m_renderer->yaw(dx / 10.0f);

  m_lastPos = _event->pos();
}

void VulkanWindow::wheelEvent(QWheelEvent *_event)
{
  /**
   * FIXME
   */

//  int numDegrees = _event->angleDelta().y() / 8;
//  int stepCount = numDegrees / 15;
//
//  if(_event->angleDelta().y() == Qt::Vertical)
//  {
//    m_camDist -= stepCount * 0.25f;
//  }
//  else if(_event->angleDelta().y())
//  {
//
//  }

//  _event->accept();
}

void VulkanWindow::keyPressEvent(QKeyEvent *_event)
{
  const auto amount = _event->modifiers().testFlag(Qt::ShiftModifier)
    ? 1.0f
    : 0.1f;

  switch(_event->key())
  {
    // escape key to quite
    case Qt::Key_Escape: QApplication::exit(EXIT_SUCCESS); break;

    //
    case Qt::Key_W: m_renderer->walk(amount);     break;
    case Qt::Key_A: m_renderer->strafe(-amount);  break;
    case Qt::Key_S: m_renderer->walk(-amount);    break;
    case Qt::Key_D: m_renderer->strafe(amount);   break;

//    case Qt::Key_Space:
//    case Qt::Key_Return:
//    break;
//    case Qt::Key_S : m_nodes->save(); break;
//    case Qt::Key_L : m_nodes->load(); break;
    default: break;
  }
}











void MainWindow::keyPressEvent(QKeyEvent *_event)
{
  switch(_event->key())
  {
    // escape key to quite
    case Qt::Key_Escape: quitApp(); break;
    case Qt::Key_Space:
    case Qt::Key_Return:
    {
			if(_event->modifiers() == Qt::ShiftModifier)
			{
			  // Signal for compile SDF Graph (Slot = SDFGraph::compileGraph)
			  emit m_vkWindow->sdfGraphChanged();
			}
    }
      break;
//    case Qt::Key_S : m_nodes->save(); break;
//    case Qt::Key_L : m_nodes->load(); break;
    default: break;
  }
}

void MainWindow::resizeEvent(QResizeEvent *_event)
{
  m_windowSize = _event->size();

  if(m_sdfGraphWidget == nullptr) return;

  resizeSDFGraph(dockWidgetArea(m_sdfGraphWidget));
}

void MainWindow::resizeSDFGraph(Qt::DockWidgetArea _area)
{
  auto isTopBottom = _area == Qt::TopDockWidgetArea  || _area == Qt::BottomDockWidgetArea;
  auto isLeftRight = _area == Qt::LeftDockWidgetArea || _area == Qt::RightDockWidgetArea;
  auto isUndocked = !isTopBottom && !isLeftRight;

  auto width = m_windowSize.width();
  auto height = m_windowSize.height();

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

  if(isUndocked)
  {
    m_sdfGraphWidget->setFixedSize(int(width / 1.5), int(height / 1.5));
    m_sdfGraphWidget->setMaximumSize(width, height);
  }
}

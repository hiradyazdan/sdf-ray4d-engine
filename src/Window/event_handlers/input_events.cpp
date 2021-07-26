/*****************************************************
 * Partial Class: MainWindow
 * Members: Event Handlers - Input Key Events (Public)
 *****************************************************/

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

void MainWindow::mousePressEvent(QMouseEvent *_event)
{
  m_pressed = true;
  m_lastPos = _event->pos();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *_event)
{
  m_pressed = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *_event)
{
  if (!m_pressed) return;

  int dx = _event->pos().x() - m_lastPos.x();
  int dy = _event->pos().y() - m_lastPos.y();

//  if (dy)
//    m_vkWindow->getRenderer()->pitch(dy / 10.0f);

//  if (dx)
//    m_renderer->yaw(dx / 10.0f);

  m_lastPos = _event->pos();
}

void MainWindow::keyPressEvent(QKeyEvent *_event)
{
  switch(_event->key())
  {
    // escape key to quite
    case Qt::Key_Escape: QGuiApplication::exit(EXIT_SUCCESS); break;
    case Qt::Key_Space:
    case Qt::Key_Return: {
//			if(_event->modifiers() == Qt::ShiftModifier) {
      emit m_vkWindow->nodeEditorModified(getNodes());
//			}
    } break;
//    case Qt::Key_S : m_nodes->save(); break;
//    case Qt::Key_L : m_nodes->load(); break;
    default: break;
  }
}

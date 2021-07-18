/*****************************************************
 * Partial Class: MainWindow
 * Members: Event Handlers - Input Key Events (Public)
 *****************************************************/

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

void MainWindow::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window receives a key event.
  // we then switch on the key value and set the camera in the GLWindow
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

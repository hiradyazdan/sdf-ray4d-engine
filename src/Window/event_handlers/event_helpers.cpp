/*****************************************************
 * Partial Class: MainWindow
 * Members: Event Helpers (Private)
 *****************************************************/

#include <QtWidgets/QFileDialog>

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

//void VulkanWindow::onVulkanInfoReceived(const QString &text)
//{
//  m_info->setPlainText(text);
//}

//void VulkanWindow::onFrameQueued(int colorValue)
//{
////  m_number->display(colorValue);
//}

//void VulkanWindow::onGrabRequested()
//{
//  if (!m_vkWindow->supportsGrab()) {
//    QMessageBox::warning(this, tr("Cannot grab"), tr("This swapchain does not support readbacks."));
//    return;
//  }
//
//  QImage img = m_vkWindow->grab();
//
//  // Our startNextFrame() implementation is synchronous so img is ready to be
//  // used right here.
//
//  QFileDialog fd(this);
//  fd.setAcceptMode(QFileDialog::AcceptSave);
//  fd.setDefaultSuffix("png");
//  fd.selectFile("test.png");
//  if (fd.exec() == QDialog::Accepted)
//    img.save(fd.selectedFiles().first());
//}

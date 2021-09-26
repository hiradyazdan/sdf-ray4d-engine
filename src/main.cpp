#include <QtWidgets/QStyleFactory>

#include "Window/MainWindow.hpp"

using MainWindow = sdfRay4d::MainWindow;

/**
 *
 * @param[in] _argc
 * @param[in] _argv
 * @return
 */
int main(int _argc, char *_argv[])
{
  QApplication app(_argc, _argv);

  QApplication::setStyle(QStyleFactory::create("fusion"));
  QApplication::setPalette(MainWindow::setPalette());

  MainWindow mainWindow;

  mainWindow.setMinimumSize(1024, 768);
  mainWindow.showMaximized();

  return QApplication::exec();
}

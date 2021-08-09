#include <QtWidgets/QStyleFactory>

#include "Window/MainWindow.hpp"

using MainWindow = sdfRay4d::MainWindow;

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QApplication::setStyle(QStyleFactory::create("fusion"));
  QApplication::setPalette(MainWindow::setPalette());

  MainWindow mainWindow;

  mainWindow.setMinimumSize(1024, 768);
  mainWindow.showMaximized();

  return QApplication::exec();
}

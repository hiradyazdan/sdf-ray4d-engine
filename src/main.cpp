#include <QtWidgets/QStyleFactory>

#include "MainWindow.hpp"

using MainWindow = ray4d::MainWindow;

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QApplication::setStyle(QStyleFactory::create("fusion"));
  QApplication::setPalette(MainWindow::setPalette());

  MainWindow mainWindow;

  mainWindow.resize(1024, 768);
  mainWindow.showMaximized();

  return QApplication::exec();
}

/*****************************************************
 * Partial Class: MainWindow (Ctor & General)
 * Members: General Functions (Public)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 *****************************************************/

#include <QPointer>
#include <QLibraryInfo>

#include "Window/MainWindow.hpp"

using namespace sdfRay4d;

MainWindow::MainWindow()
{
  initVkWindow();

  initWidgets();
  initLayouts();

  createActions();
  createMenus();
}


QPalette MainWindow::setPalette()
{
  QPalette palette;

  /**
   * https://gist.github.com/skyrpex/5547015
   */

  palette.setColor(QPalette::Window, QColor(53,53,53));
  palette.setColor(QPalette::WindowText, Qt::white);

  palette.setColor(QPalette::Base, QColor(25,25,25));
  palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
  palette.setColor(QPalette::ToolTipBase, Qt::white);

  palette.setColor(QPalette::ToolTipText, Qt::white);

  palette.setColor(QPalette::Text, Qt::white);
  palette.setColor(QPalette::Button, QColor(53,53,53));
  palette.setColor(QPalette::ButtonText, Qt::white);
  palette.setColor(QPalette::BrightText, Qt::red);

  palette.setColor(QPalette::Link, QColor(42, 130, 218));

  palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
  palette.setColor(QPalette::HighlightedText, Qt::black);

  palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
  palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

  return palette;
}

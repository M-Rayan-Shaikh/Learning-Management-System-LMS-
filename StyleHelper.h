#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QString>

class StyleHelper {
public:
    static QString getStyle() {
        return R"(
            /* Main Window & General Background */
            QMainWindow, QDialog, QWidget {
                background-color: #0f172a;
                color: #f1f5f9;
                font-family: 'Segoe UI', 'Roboto', 'Helvetica Neue', sans-serif;
                font-size: 10pt;
            }

            /* Headers */
            QLabel[text="Welcome!"] {
                font-size: 26pt;
                font-weight: bold;
                color: #38bdf8;
                padding: 10px 0;
            }
            
            QLabel {
                color: #e2e8f0;
            }

            /* Buttons */
            QPushButton {
                background-color: #1e293b;
                color: #f1f5f9;
                border: 1px solid #334155;
                border-radius: 6px;
                padding: 8px 16px;
                min-height: 24px;
                font-weight: 600;
            }
            QPushButton:hover {
                background-color: #334155;
                border-color: #475569;
            }
            QPushButton:pressed {
                background-color: #38bdf8;
                color: #0f172a;
            }
            QPushButton:disabled {
                background-color: #0f172a;
                color: #475569;
                border-color: #1e293b;
            }

            /* Input Fields */
            QLineEdit, QTextEdit, QPlainTextEdit, QComboBox {
                background-color: #1e293b;
                color: #f1f5f9;
                border: 1px solid #334155;
                border-radius: 6px;
                padding: 6px;
                selection-background-color: #38bdf8;
                selection-color: #0f172a;
            }
            QLineEdit:focus, QTextEdit:focus, QComboBox:focus {
                border: 1px solid #38bdf8;
                background-color: #0f172a;
            }

            /* Combo Box */
            QComboBox::drop-down {
                subcontrol-origin: padding;
                subcontrol-position: top right;
                width: 20px;
                border-left-width: 1px;
                border-left-color: #334155;
                border-left-style: solid;
                border-top-right-radius: 6px;
                border-bottom-right-radius: 6px;
            }
            QComboBox QAbstractItemView {
                background-color: #1e293b;
                border: 1px solid #334155;
                selection-background-color: #334155;
                color: #f1f5f9;
            }

            /* Tab Widget */
            QTabWidget::pane {
                border: 1px solid #334155;
                background: #0f172a;
                border-radius: 4px;
            }
            QTabBar::tab {
                background: #1e293b;
                border: 1px solid #334155;
                padding: 8px 16px;
                border-top-left-radius: 6px;
                border-top-right-radius: 6px;
                color: #94a3b8;
                margin-right: 4px;
                font-weight: 600;
            }
            QTabBar::tab:selected {
                background: #334155;
                border-bottom-color: #334155;
                color: #38bdf8;
            }
            QTabBar::tab:hover {
                background: #334155;
                color: #f1f5f9;
            }

            /* Tables */
            QTableWidget {
                background-color: #1e293b;
                alternate-background-color: #0f172a;
                gridline-color: #334155;
                color: #f1f5f9;
                selection-background-color: #38bdf8;
                selection-color: #0f172a;
                border: 1px solid #334155;
                border-radius: 4px;
            }
            QHeaderView::section {
                background-color: #0f172a;
                color: #94a3b8;
                padding: 8px;
                border: none;
                border-bottom: 2px solid #334155;
                font-weight: bold;
                text-transform: uppercase;
                font-size: 9pt;
            }
            QTableCornerButton::section {
                background-color: #0f172a;
                border: 1px solid #334155;
            }
            
            /* Scrollbars */
            QScrollBar:vertical {
                border: none;
                background: #0f172a;
                width: 12px;
                margin: 0px 0px 0px 0px;
            }
            QScrollBar::handle:vertical {
                background: #334155;
                min-height: 20px;
                border-radius: 6px;
                margin: 2px;
            }
            QScrollBar::handle:vertical:hover {
                background: #475569;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                border: none;
                background: none;
            }

            /* Tooltips */
            QToolTip {
                border: 1px solid #38bdf8;
                padding: 4px;
                border-radius: 4px;
                background-color: #0f172a;
                color: #f1f5f9;
            }
            
            /* Status Bar */
            QStatusBar {
                background-color: #0f172a;
                color: #94a3b8;
                border-top: 1px solid #334155;
            }
        )";
    }
};

#endif // STYLEHELPER_H

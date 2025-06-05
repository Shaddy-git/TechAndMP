#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;
class QLineEdit;
class QDoubleSpinBox;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onFunctionButtonClicked(int id);
    void onChordMethodClicked();

private:
    QTextEdit *m_resultDisplay;
    QLineEdit *m_functionInput;
    QDoubleSpinBox *m_intervalAInput;
    QDoubleSpinBox *m_intervalBInput;
    QPushButton *m_chordMethodButton;
};

#endif // MAINWINDOW_H

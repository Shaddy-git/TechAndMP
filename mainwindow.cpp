#include "mainwindow.h"
#include "ClientSingleton.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qspinbox.h"
#include "qtextedit.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Настройка главного виджета
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Группа для метода хорд
    QGroupBox *chordGroup = new QGroupBox("Метод хорд", this);
    QFormLayout *formLayout = new QFormLayout(chordGroup);

    // Поля ввода
    m_functionInput = new QLineEdit("x*x - 2", this);
    m_intervalAInput = new QDoubleSpinBox(this);
    m_intervalBInput = new QDoubleSpinBox(this);

    // Настройка spinbox
    m_intervalAInput->setRange(-1000, 1000);
    m_intervalBInput->setRange(-1000, 1000);
    m_intervalAInput->setValue(1.0);
    m_intervalBInput->setValue(2.0);
    m_intervalAInput->setSingleStep(0.1);
    m_intervalBInput->setSingleStep(0.1);

    // Кнопка метода хорд
    m_chordMethodButton = new QPushButton("Вычислить корень", this);
    connect(m_chordMethodButton, &QPushButton::clicked,
            this, &MainWindow::onChordMethodClicked);

    // Добавление элементов в форму
    formLayout->addRow("Функция f(x):", m_functionInput);
    formLayout->addRow("Начало интервала (a):", m_intervalAInput);
    formLayout->addRow("Конец интервала (b):", m_intervalBInput);
    formLayout->addRow(m_chordMethodButton);

    // Кнопки для других функций
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    for (int i = 1; i <= 4; ++i) {
        QPushButton *btn = new QPushButton(QString("Функция %1").arg(i), this);
        connect(btn, &QPushButton::clicked, [i, this]() { onFunctionButtonClicked(i); });
        buttonsLayout->addWidget(btn);
    }

    // Поле вывода результатов
    m_resultDisplay = new QTextEdit(this);
    m_resultDisplay->setReadOnly(true);

    // Компоновка всех элементов
    mainLayout->addWidget(chordGroup);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(new QLabel("Результаты:", this));
    mainLayout->addWidget(m_resultDisplay);

    setCentralWidget(centralWidget);
    setWindowTitle("Численные методы");
    resize(600, 500);

    // Подключение сигналов
    connect(&ClientSingleton::instance(), &ClientSingleton::resultUpdated,this, [this](const QString& result) {
                m_resultDisplay->append(result);
                m_resultDisplay->append("--------------------------");
            });
}

void MainWindow::onFunctionButtonClicked(int id)
{
    ClientSingleton::instance().executeFunction(id);
}

void MainWindow::onChordMethodClicked()
{
    ClientSingleton::instance().executeChordMethod(
        m_functionInput->text(),
        m_intervalAInput->value(),
        m_intervalBInput->value()
        );
}

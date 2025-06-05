#include "ChordMethod.h"
#include <QRegularExpression>

double ChordMethod::solve(const QString& function, double a, double b, double epsilon, int maxIterations) {
    double fa = evaluateBasicExpression(function, a);
    double fb = evaluateBasicExpression(function, b);

    if (fa * fb >= 0) {
        throw std::invalid_argument("Функция должна иметь разные знаки на концах интервала");
    }

    // Формула метода хорд
    for (int i = 0; i < maxIterations; ++i) {
        double x = a - fa * (b - a) / (fb - fa);
        double fx = evaluateBasicExpression(function, x);

        if (std::abs(fx) < epsilon) {
            return x;
        }

        if (fa * fx < 0) {
            b = x;
            fb = fx;
        } else {
            a = x;
            fa = fx;
        }
    }
    throw std::runtime_error("Превышено максимальное количество итераций");
}

// Обработка записаной функции (Удаление пробелов)
QString ChordMethod::preprocessExpression(QString expr) {
    expr = expr.trimmed().toLower();
    expr.replace(" ", "");
    expr.replace("^", "**");
    return expr;
}


double ChordMethod::evaluateBasicExpression(const QString& expr, double x) {

    QString processed = preprocessExpression(expr);

    if (processed == "x*x-4" || processed == "x**2-4") return x*x - 4;
    if (processed == "x*x-2" || processed == "x**2-2") return x*x - 2;
    if (processed == "sin(x)") return std::sin(x);
    if (processed == "cos(x)") return std::cos(x);
    if (processed == "exp(x)") return std::exp(x);
    if (processed == "log(x)") return std::log(x);
    if (processed == "sqrt(x)") return std::sqrt(x);

    throw std::invalid_argument("Неподдерживаемая функция. Используйте: x*x-2, sin(x), cos(x) и т.д.");
}

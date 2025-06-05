#ifndef CHORDMETHOD_H
#define CHORDMETHOD_H

#include <QString>
#include <stdexcept>
#include <cmath>

class ChordMethod
{
public:
    static double solve(
        const QString& function,
        double a,
        double b,
        double epsilon = 1e-3,
        int maxIterations = 1000
        );

private:
    static double evaluateBasicExpression(const QString& expr, double x);
    static QString preprocessExpression(QString expr);
};

#endif // CHORDMETHOD_H

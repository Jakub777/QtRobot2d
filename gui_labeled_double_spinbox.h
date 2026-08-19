#pragma once

#include <QWidget>

class QLabel;
class QDoubleSpinBox;

class LabeledDoubleSpinBox : public QWidget
{
    Q_OBJECT

public:
    explicit LabeledDoubleSpinBox(const QString& name,
                            QWidget* parent = nullptr);

    double value() const;
    void setValue(double value);
    void setRange(double minimum, double maximum);
    void setSingleStep(double step);
    void setReadOnly(bool readOnly);

signals:
    void valueChanged(double value);

private:
    QLabel* m_label;
    QDoubleSpinBox* m_spinBox;
};
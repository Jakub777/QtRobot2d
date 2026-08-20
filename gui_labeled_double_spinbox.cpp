#include "gui_labeled_double_spinbox.h"

#include <QLabel>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

LabeledDoubleSpinBox::LabeledDoubleSpinBox(const QString& label, QWidget* parent)
    : QWidget(parent)
{
    m_label = new QLabel(label, this);
    m_spinBox = new QDoubleSpinBox(this);

    m_spinBox->setRange(-360.0, 360.0);
    m_spinBox->setSingleStep(1.0);
    m_spinBox->setDecimals(2);

    auto* layout = new QHBoxLayout(this);

    layout->addWidget(m_label);
    layout->addWidget(m_spinBox);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    setLayout(layout);

    connect(m_spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &LabeledDoubleSpinBox::valueChanged);
}

double LabeledDoubleSpinBox::value() const
{
    return m_spinBox->value();
}

void LabeledDoubleSpinBox::setValue(double value)
{
    m_spinBox->setValue(value);
}

void LabeledDoubleSpinBox::setRange(double minimum, double maximum)
{
    m_spinBox->setRange(minimum, maximum);
}

void LabeledDoubleSpinBox::setSingleStep(double step)
{
    m_spinBox->setSingleStep(step);
}

void LabeledDoubleSpinBox::setReadOnly(bool readOnly)
{
    m_spinBox->setReadOnly(readOnly);
}
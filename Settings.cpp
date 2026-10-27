//
// Created by Роман on 24.11.2025.
//

#include "Settings.h"

Settings::Settings(QDialog *parent) : QDialog(parent) {
    title = new QLabel(this);
    title->setText(tr("<h3>Enter parameters</h3>"));
    title->setAlignment(Qt::AlignLeft);

    lengthBox = new QSpinBox(this);
    lengthBox->setRange(5, 50);
    lengthBox->setValue(10);
    volumeBox = new QComboBox(this);
    volumeBox->addItems({"20", "40", "60", "80", "100"});
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    connect(buttonBox, &QDialogButtonBox::accepted, this, &Settings::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &Settings::slotReject);

    lengthLabel = new QLabel(tr("1. Pipeline length, km (from 5 to 50 km)"));
    lengthLabel->setAlignment(Qt::AlignLeft);
    volumeLabel = new QLabel(tr("2. Water injection volume, thousand barrels/day"));
    volumeLabel->setAlignment(Qt::AlignLeft);

    formLayout = new QFormLayout();
    formLayout->addRow(title);
    formLayout->addRow(lengthLabel, lengthBox);
    formLayout->addRow(volumeLabel, volumeBox);
    formLayout->addRow(buttonBox);
    setWindowTitle(tr("Parameters input"));
    setLayout(formLayout);
    setFixedSize(500, 140);
}

void Settings::slotReject() {
    emit signalUpdate(0, 0);
    reject();
}

void Settings::slotAccepted() {
    emit signalUpdate(lengthBox->value(), volumeBox->currentText().toInt());
    accept();
}

void Settings::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setWindowTitle(tr("Parameters input"));
        title->setText(tr("<h3>Enter parameters</h3>"));

        lengthLabel->setText(tr("1. Pipeline length, km (from 5 to 50 km)"));
        volumeLabel->setText(tr("2. Water injection volume, thousand barrels/day"));

        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
        buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    } else
        QWidget::changeEvent(event);
}

Settings::~Settings() {
    delete title;
    delete lengthBox;
    delete volumeBox;
    delete buttonBox;
    delete lengthLabel;
    delete volumeLabel;
    delete formLayout;
}

//
// Created by Роман on 03.01.2026.
//

#include "Language.h"

Language::Language(QDialog *parent) : QDialog(parent) {
    title = new QLabel();
    title->setText(tr("<h3>Select language</h3>"));
    title->setAlignment(Qt::AlignLeft);

    listWidget = new QComboBox();

    QList<QPair<QString, QString>> languages = {
        {"English", "en_US"},
        {"Русский", "ru_RU"},
        {"Français", "fr_FR"},
        {"Deutsch", "de_DE"},
        {"Español", "es_ES"},
        {"中文", "zh_CN"},
        {"日本語", "ja_JP"},
        {"Italiano", "it_IT"}
    };

    listWidget->clear();
    for (const auto& lang : languages) {
        listWidget->addItem(lang.first, lang.second);
    }

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    connect(buttonBox, &QDialogButtonBox::accepted, this, &Language::slotOkPressed);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &Language::reject);

    languageLabel = new QLabel(tr("Language:"));
    languageLabel->setAlignment(Qt::AlignLeft);

    formLayout = new QFormLayout();
    formLayout->addRow(title);
    formLayout->addRow(languageLabel, listWidget);
    formLayout->addRow(buttonBox);
    setLayout(formLayout);
    setWindowTitle(tr("Language selection"));
}

void Language::slotOkPressed() {
    emit signalLanguage(listWidget->currentData().toString());
    accept();
}

void Language::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setWindowTitle(tr("Language selection"));
        title->setText(tr("<h3>Select language</h3>"));
        languageLabel->setText(tr("Language:"));
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
        buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    } else
        QWidget::changeEvent(event);

}

Language::~Language() {
    delete listWidget;
    delete buttonBox;
    delete title;
    delete languageLabel;
    delete formLayout;
}

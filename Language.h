//
// Created by Роман on 03.01.2026.
//

#ifndef PPD_LANGUAGE_H
#define PPD_LANGUAGE_H

#include <QDialog>
#include <QPushButton>
#include <QEvent>
#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>

class Language : public QDialog {
    Q_OBJECT
public:
    explicit Language(QDialog *parent = nullptr);
    void changeEvent(QEvent *event) override;
    ~Language() override;
private:
    QLabel *title, *languageLabel;
    QComboBox *listWidget;
    QFormLayout *formLayout;
    QDialogButtonBox *buttonBox;
public slots:
    void slotOkPressed();
signals:
    void signalLanguage(QString);
};


#endif //PPD_LANGUAGE_H

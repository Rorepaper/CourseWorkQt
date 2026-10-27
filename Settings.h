//
// Created by Роман on 24.11.2025.
//

#ifndef PPD_SETTINGS_H
#define PPD_SETTINGS_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QEvent>
#include <QPushButton>

class Settings : public QDialog {
Q_OBJECT

public:
    explicit Settings(QDialog *parent = nullptr);
    void changeEvent(QEvent *event) override;
    ~Settings() override;
private:
    QLabel *title, *lengthLabel, *volumeLabel;
    QComboBox *volumeBox;
    QSpinBox *lengthBox;
    QFormLayout *formLayout;
    QDialogButtonBox *buttonBox;
public slots:

    void slotAccepted();

    void slotReject();

signals:

    void signalUpdate(int, int);

};


#endif //PPD_SETTINGS_H

#ifndef CHOICE_H
#define CHOICE_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QEvent>

class Choice: public QDialog
{
  Q_OBJECT
private:
    QLabel *title;
    QRadioButton *platformPressure;
    QRadioButton *boxPressure;
    QVBoxLayout  *layout;
    QDialogButtonBox *buttonBox;
public:
    explicit Choice(QWidget *parent=nullptr);
    ~Choice() override;
    void changeEvent(QEvent *event) override;
public slots:
    void slotApply();
    void slotReject();
signals:
    void signalMode(int);

};

#endif // CHOICE_H

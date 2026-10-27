#ifndef NAME_H
#define NAME_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QEvent>
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

class Name : public QDialog {
Q_OBJECT

public:
    explicit Name(QWidget *parent = nullptr);
    void changeEvent(QEvent *event) override;
    ~Name() override;

public slots:
    void slotOk();

signals:
    void signalOk(QString);

private:
    QLabel *lb;
    QLineEdit *lname;
    QVBoxLayout *layout;
    QString userName;
    QDialogButtonBox *buttonBox;

};

#endif // NAME_H

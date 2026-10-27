#include "name.h"

Name::Name(QWidget *parent): QDialog(parent), userName("")
{
    setWindowTitle(tr("Name input"));
    layout = new QVBoxLayout(this);
    lb = new QLabel(tr("<h3>Enter name and surname</h3>"), this);
    lb->setAlignment(Qt::AlignLeft);
    lname = new QLineEdit(this);

    QRegularExpression regExp("^[A-ZА-ЯЁ][a-zа-яё]*(?:[- ][A-ZА-ЯЁ][a-zа-яё]*)*$");
    static auto *validator = new QRegularExpressionValidator(regExp, this);
    lname->setValidator(validator);


    buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    connect(buttonBox, &QDialogButtonBox::accepted, this, &Name::slotOk);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &Name::reject);

    layout->addWidget(lb);
    layout->addWidget(lname);
    layout->addWidget(buttonBox);
    setLayout(layout);
    setFixedSize(300, 100);

}

void Name::slotOk()
{
    if(lname->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Name can't be empty!"));
    }
    else
    {
        userName = lname->text();
        accept();
        emit signalOk(userName);
    }
}

void Name::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setWindowTitle(tr("Name input"));
        lb->setText(tr("<h3>Enter name and surname</h3>"));
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
        buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    } else
        QWidget::changeEvent(event);

}

Name::~Name() {
    delete layout;
    delete lb;
    delete lname;
    delete buttonBox;
}

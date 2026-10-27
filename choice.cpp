#include "choice.h"

Choice::Choice(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Mode selection"));
    layout = new QVBoxLayout(this);
    title = new QLabel(tr("<h3>Select mode</h3>"), this);
    title->setAlignment(Qt::AlignLeft);
    platformPressure = new QRadioButton(tr("Platform waterflooding system"));
    boxPressure = new QRadioButton(tr("Subsea waterflooding module"));
    platformPressure->setChecked(true);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    connect(buttonBox, &QDialogButtonBox::accepted, this, &Choice::slotApply);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &Choice::slotReject);

    layout->addWidget(title);
    layout->addSpacing(10);
    layout->addWidget(platformPressure);
    layout->addWidget(boxPressure);
    layout->addSpacing(20);
    layout->addWidget(buttonBox);

    setLayout(layout);
    setFixedSize(220, 150);
}

void Choice::slotReject() {
    reject();
    emit signalMode(2);
}

void Choice::slotApply() {
    accept();
    emit signalMode(boxPressure->isChecked());
}

void Choice::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setWindowTitle(tr("Mode selection"));
        title->setText(tr("<h3>Select mode</h3>"));
        platformPressure->setText(tr("Platform waterflooding system"));
        boxPressure->setText(tr("Subsea waterflooding module"));
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
        buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    } else
        QWidget::changeEvent(event);
}

Choice::~Choice() {
    delete layout;
    delete title;
    delete platformPressure;
    delete boxPressure;
    delete buttonBox;
}

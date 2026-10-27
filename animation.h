//
// Created by Роман on 24.11.2025.
//

#ifndef PPD_ANIMATION_H
#define PPD_ANIMATION_H

#include <QApplication>
#include <QVBoxLayout>
#include <QSettings>
#include <QProcess>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QPainter>
#include <QObject>
#include <QSvgRenderer>
#include <QPdfWriter>
#include <QTimer>
#include <QGradient>
#include <QMouseEvent>
#include <QVector>
#include <QPainterPath>
#include <QMessageBox>
#include <QTranslator>
#include <QFileDialog>
#include "Settings.h"
#include "choice.h"
#include "name.h"
#include "Language.h"

class Animation : public QWidget {
Q_OBJECT

public:
    explicit Animation(QWidget *parent = nullptr);
    ~Animation() override;

    void initWater();
    void switchLanguage();

public slots:
    void saveTXTSlot();
    void infoSlot();
    void slotUpdateAnimation();
    void savePDFSlot();
    void drawContent(QPainter &painter);
    void slotUpdateData(int l, int v);
    void slotSetMode(int m);
    void slotSetName(const QString& name);
    void setLanguage(QString lang);

signals:
    void signalRequestQuit();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QSettings settings;
    QTranslator *translator;
    QLabel *info;
    Name *nameDialog;
    Choice *choice;
    Settings *setDialog;
    Language *langDialog;
    QMenuBar *menuBar;
    QMenu *fileMenu, *settingsMenu;
    QSvgRenderer svgRenderer;
    QTimer *animationTimer;
    QVector<QVector<int>> fluids, particles;
    QVBoxLayout *layout;
    QString fullName, language;
    int length, volume;
    bool boxMode, firstTime;
};


#endif //PPD_ANIMATION_H

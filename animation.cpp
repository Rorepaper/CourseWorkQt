//
// Created by Роман on 24.11.2025.
//

#include "animation.h"

#include <utility>

Animation::Animation(QWidget *parent) : QWidget(parent), settings("PPD", "PPD") {
    setMouseTracking(true);
    translator = new QTranslator(this);
    // Здесь загружаются параметры из предыдущего запуска
    settings.beginGroup("/Settings");
    firstTime = settings.value("/firstTime", true).toBool(); // Первый запуск
    length = settings.value("/length", 50).toInt(); // Протяженность трубопровода
    volume = settings.value("/volume", 20).toInt(); // Объем закачки воды
    boxMode = settings.value("/boxMode", false).toBool(); // Режим системы ППД
    language = settings.value("/language", "en_US").toString(); // Язык программы
    fullName = settings.value("/fullName", "").toString(); // Имя работника
    settings.endGroup();

    // Создание диалоговых окон, являющихся атрибутами класса и подключение слотов
    langDialog = new Language(); // Выбор языка
    connect(langDialog, &Language::signalLanguage, this, &Animation::setLanguage);
    nameDialog = new Name(); // Ввод имени
    connect(nameDialog, &Name::signalOk, this, &Animation::slotSetName);
    setDialog = new Settings(); // Установка параметров
    connect(setDialog, &Settings::signalUpdate, this, &Animation::slotUpdateData);
    choice = new Choice(); // Выбор режима
    connect(choice, &Choice::signalMode, this, &Animation::slotSetMode);

    animationTimer = new QTimer(this);
    animationTimer->setInterval(16);
    animationTimer->start();
    connect(animationTimer, &QTimer::timeout, this, &Animation::slotUpdateAnimation);


    info = new QLabel(this);
    info->setAlignment(Qt::AlignCenter);
    info->move(560, 100);
    info->setStyleSheet(
            "QLabel {"
            "   border: 2px solid white;"
            "   border-radius: 8px;"
            "   padding: 10px;"
            "   background-color: rgba(0, 0, 0, 150);"
            "   color: white;"
            "   font-size: 14px;"
            "}"
    );
    info->setFixedSize(480, 80);


    menuBar = new QMenuBar(this);
    menuBar->setStyleSheet(
            "QMenuBar {"
            "   border-bottom: 2px solid white;"
            "   background-color: rgba(0, 0, 0, 150);"
            "   color: white;"
            "   font-size: 14px;"
            "}"
            "QMenuBar::item {"
            "   padding: 3px 20px;"
            "   background-color: transparent;"
            "   border-radius: 4px;"
            "}"
            "QMenuBar::item:selected {"
            "   background-color: rgba(255, 255, 255, 50);"
            "}"
            "QMenuBar::item:pressed {"
            "   background-color: rgba(255, 255, 255, 100);"
            "}"
    );

    QPalette palette = menuBar->palette();
    palette.setColor(QPalette::Window, QColor(240, 240, 240));
    menuBar->setPalette(palette);

    fileMenu = new QMenu(tr("Save"));
    fileMenu->addAction(tr("As PDF"), this, &Animation::savePDFSlot);
    fileMenu->addAction(tr("As TXT"), this, &Animation::saveTXTSlot);

    settingsMenu = new QMenu(tr("Settings"));
    settingsMenu->addAction(tr("Mode"), choice, &Choice::exec);
    settingsMenu->addAction(tr("Parameters"), setDialog, &Settings::exec);
    settingsMenu->addAction(tr("Language"), langDialog, &Language::exec);
    settingsMenu->addAction(tr("Change Name"), nameDialog, &Name::exec);
    settingsMenu->addAction(tr("About"), this, &Animation::infoSlot);

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(settingsMenu);

    layout = new QVBoxLayout;
    layout->setMenuBar(menuBar);
    setLayout(layout);

    setFixedSize(1200, 800);
    // Функция tr() используется для перевода текста в соответствии с выбранным языком
    setWindowTitle(tr("Water Flooding Systems"));
    // При первом запуске открывается диалог с выбором языка
    if (firstTime) {
        nameDialog->exec();
    } else {
        show();
    }
    // Метод для установки частиц жидкостей в начальное положение
    initWater();
    // Метод для установки языка
    switchLanguage();
}

void Animation::slotSetName(const QString& name) {
    if (name.isEmpty()) {
        if (firstTime) {
            emit signalRequestQuit();
            return;
        }
    } else {
        fullName = name;
        if (firstTime) {
            choice->exec();
        }
    }
}
void Animation::setLanguage(QString lang) {
    language = std::move(lang);
    switchLanguage();
}

void Animation::slotUpdateData(int l, int v) {
    if (l == 0) {
        if (firstTime) {
            emit signalRequestQuit();
            return;
        }
    } else {
        if (firstTime) firstTime = false;
        show();
        length = l;
        volume = v;
    }
    initWater();
}

void Animation::slotSetMode(int m) {
    if (m == 2) {
        if (firstTime) {
            emit signalRequestQuit();
            return;
        }
    } else {
        setDialog->exec();
        boxMode = m;
    }
    initWater();
}

// Слот для сохранения результата в формате PDF
void Animation::savePDFSlot() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"),
                                                    "", "PDF files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }
    // После выбора названия файла через стандартное диалоговое окно
    // запускаем метод для отрисовки графики программы в файл
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageOrientation(QPageLayout::Landscape);


    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&pdfWriter);

    painter.scale(11, 11);
    drawContent(painter);
}
// Для сохранения результата в формате TXT также используется стандартное диалоговое окно
// После чего с помощью QTextStream записывается содержимое в выбранный файл
// С использованием метода tr() происходит перевод текста в соответствии с выбранным языком
void Animation::saveTXTSlot() {
    QString str = QFileDialog::getSaveFileName(this, tr("Save"), "", "TXT (*.txt)");
    if (str.isEmpty()) return;
    QFile file(str);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        QString currentDateTime = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        QString reportTitle = QString("=").repeated(50);
        QString separator = QString("-").repeated(50);

        out << reportTitle << "\n";
        out << tr("WATERFLOODING SYSTEM ANALYSIS REPORT\n");
        out << tr("Date and time of generation: ") << currentDateTime << "\n";
        out << separator << "\n";
        out << tr("Employee: ") << fullName << "\n\n";

        out << tr("RESULT:\n");
        out << separator << "\n";
        out << tr("Waterflooding mode: ")
            << (boxMode ? tr("Subsea waterflooding module") : tr("Platform waterflooding system")) << "\n";
        QString resultText = info->text();
        resultText.replace("<br>", "\n");
        out << resultText << "\n\n";

        out << tr("CONCLUSION:\n");
        out << separator << "\n";
        if (resultText.contains(tr("Platform is more profitable"))) {
            out << tr("Modernization of the existing platform waterflooding system is recommended") << "\n";
            out << tr("with pipeline installation for water transportation.") << "\n";
        } else {
            out << tr("installation of subsea water preparation module (SEABOX) is recommended") << "\n";
            out << tr("in close proximity to injection wells.") << "\n";
        }

        out << reportTitle << "\n";
        file.close();
    }
}

// Вывод информационного окна написан с помощью языка разметки HTML
void Animation::infoSlot() {
    QMessageBox::information(this, tr("About the Program"), tr(
            "<html>"
            "<head><title>About the Program</title></head>"
            "<body>"
            "<h3>About the Program</h3>"
            "<p><b>This program is designed for visual modeling of reservoir pressure maintenance systems (waterflooding)</b> "
            "in two modes: <i>Platform-based waterflooding</i> and <i>Subsea SEABOX module</i>.</p>"

            "<p>The user can visually observe the difference between these two technological solutions, "
            "as well as analyze their economic efficiency depending on distance and injection volume.</p>"

            "<h4>Technical Implementation:</h4>"
            "<ul>"
            "<li>Programming language: C++</li>"
            "<li>Framework: Qt 6</li>"
            "<li>Graphics format: SVG</li>"
            "</ul>"

            "<h4>Key Features:</h4>"
            "<ul>"
            "<li>Interactive model of two waterflooding systems</li>"
            "<li>Dynamic parameter adjustment (pipeline length, injection volume)</li>"
            "<li>Water flow visualization with animation</li>"
            "<li>Save results in text and PDF formats</li>"
            "<li>Interface localization</li>"
            "</ul>"

            "<h4>Theoretical Basis:</h4>"
            "<p>The program is based on a comparative analysis of two approaches to waterflooding organization:</p>"
            "<ol>"
            "<li><b>Traditional:</b> Platform equipment modernization + water pipeline installation</li>"
            "<li><b>Innovative:</b> Subsea SEABOX module at wellheads + umbilical cable</li>"
            "</ol>"

            "<p><b>Developer:</b> Roman Vidyakin<br>"
            "<b>Release Date:</b> 13.12.2025<br>"
            "<b>Version:</b> 1.0</p>"

            "<p><small><i>Note: The model is based on research of economic efficiency of waterflooding systems "
            "for satellite fields. All calculations are predictive in nature.</i></small></p>"
            "</body>"
            "</html>")
    );
}

void Animation::slotUpdateAnimation() {
    // Движение водяных частиц реализовано системой в виде цикла, в котором
    // водяные частицы двигаются в зависимости от текущих показателей
    // расположения водяных частиц и скорости воды
    int speed = volume / 10;
    for (auto &fluid: fluids) {
        if (fluid[1] - speed > 260 && fluid[3] >= 30) {
            fluid[1] -= speed; // Опускание частицы вниз
        } else if ((fluid[1] - speed <= 260) && (fluid[3] - speed > 0)) {
            fluid[3] -= speed; // Сжимание частицы в нижней границе
        } else if ((fluid[1] - speed <= 260) && (fluid[3] - speed <= 0)) {
            fluid[3] = 0; // Перемещение частицы в верхнюю границу
            fluid[1] = 700;
        } else if (fluid[3] <= 30) {
            fluid[1] -= speed; //Появление частицы в верхней границе
            fluid[3] += speed;
        }
    }

    for (auto &particle: particles) {
        if (boxMode) {
            if (particle[3] <= 30 && particle[1]  + speed + 40 <= 700) {
                particle[3] += speed;
            } else if (particle[1] + speed + 40 < 700) {
                particle[1] += speed;
            } else if (particle[3] >= 0 && particle[1] + speed + 40 >= 700) {
                particle[1] += speed;
                particle[3] -= speed;
            } else {
                particle[3] = 0;
                particle[1] = 500;
            }
        } else {
            if (particle[3] <= 30 && particle[0] <= 195) {
                particle[3] += speed;
            } else if (particle[1] + speed < 460 && particle[3] >= 30 && particle[0] <= 195) {
                particle[1] += speed;
            } else if (particle[1] + speed >= 460 && particle[0] + speed < length * 15 + 200 - 30) {
                particle[1] = 460;
                particle[3] = 30;
                particle[0] += speed;
            } else if (particle[1] + speed >= 460 && particle[0] + speed >= length * 15 + 200 - 30 &&
                       particle[1] + speed + 30 <= 700) {
                particle[0] = length * 15 + 200 - 30;
                particle[1] += speed;
            } else if (particle[1] + speed + 30 > 700 && particle[3] - speed > 0) {
                particle[1] += speed;
                particle[3] -= speed;
            } else {
                particle[3] = 0;
                particle[1] = 260;
                particle[0] = 195;
            }
        }
    }
    update();
}

void Animation::drawContent(QPainter &painter) {
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.fillRect(QRect(0, 0, 1200, 800), QColor(155, 178, 202));

    QLinearGradient waterGradient(0, 330, 0, 500);
    waterGradient.setColorAt(0, QColor(109, 157, 250));
    waterGradient.setColorAt(1, QColor(4, 22, 106));

    painter.fillRect(QRect(0, 330, 1200, 170), waterGradient);
    painter.fillRect(QRect(0, 500, 1200, 100), QColor(163, 155, 126));
    painter.fillRect(QRect(0, 600, 1200, 150), QColor(38, 22, 19));
    painter.fillRect(QRect(0, 750, 1200, 50), QColor(163, 155, 126));


    painter.setPen(QPen(QColor(192, 229, 231), 3));
    painter.drawLine(0, 330, 1200, 330);
    painter.setPen(QPen(QColor(24, 14, 12), 5));
    painter.drawLine(0, 500, 1200, 500);
    painter.drawLine(0, 600, 1200, 600);
    painter.drawLine(0, 750, 1200, 750);


    painter.setBrush(QColor(155, 178, 202));
    painter.setPen(QPen(QColor("black"), 6));

    if (svgRenderer.load(QString("://resources/pics/oilPlatform.svg"))) {
        svgRenderer.render(&painter, QRectF(56, 30, 300, 300));
    }

    QPainterPath path;

    path.moveTo(QPoint(150, 260));
    path.quadTo(QPoint(165, 270), QPoint(180, 260));
    path.lineTo(QPoint(180, 700));
    path.quadTo(QPoint(165, 710), QPoint(150, 700));
    path.lineTo(QPoint(150, 260));
    // В перегрузке PaintEvent реализована отрисовка трубопровода
    if (boxMode) {
        // Если режим подводного модуля, то рисуем его
        // Труба
        path.moveTo(QPoint(length * 15 + 200, 500));
        path.lineTo(length * 15 + 200, 700);
        path.quadTo(QPoint(length * 15 - 15 + 200, 710), QPoint(length * 15 - 30 + 200, 700));
        path.lineTo(QPoint(length * 15 - 30 + 200, 500));
        painter.setBrush(QColor(Qt::yellow));
        painter.drawRect(length * 15 + 150, 430, 150, 70);
        painter.setPen(QPen(Qt::red, 12));
        // Кабель
        painter.drawLine(QPoint(210, 262), QPoint(210, 490));
        painter.drawLine(QPoint(210, 490), QPoint(length * 15 - 53 + 200, 490));
    } else {
        // Иначе рисуем трубопровод от платформы
        // Начало трубопровода
        path.moveTo(QPoint(195, 260));
        path.quadTo(QPoint(210, 270), QPoint(225, 260));
        path.lineTo(QPoint(225, 460));
        // Изгиб
        path.lineTo(QPoint(length * 15 + 200, 460));
        path.lineTo(length * 15 + 200, 700);
        // Конец трубопровода
        path.quadTo(QPoint(length * 15 - 15 + 200, 710), QPoint(length * 15 - 30 + 200, 700));
        path.lineTo(QPoint(length * 15 - 30 + 200, 490));
        path.lineTo(QPoint(195, 490));
        path.lineTo(QPoint(195, 260));
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(128, 128, 128));
    painter.drawPath(path);

    for (const auto &fluid: fluids) {
        painter.fillRect(QRect(fluid[0], fluid[1], fluid[2], fluid[3]), QColor(38, 22, 19));
    }
    for (const auto &particle: particles) {
        painter.fillRect(QRect(particle[0], particle[1], particle[2], particle[3]), QColor(97, 231, 254, 160));
    }

    painter.setPen(QPen(Qt::black, 8));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(path);
}

void Animation::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    drawContent(painter);
}
// Перегрузка события нажатия мыши
void Animation::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    // Если мышь нажата в заданном прямоугольнике, то увеличиваем объем закачки
    if (QRect(QPoint(100 + length * 15, 754), QPoint(400 + length * 15, 334)).contains(event->pos())) {
        if (volume < 100) volume += 20; else volume = 20;
        initWater();
    }
}

void Animation::mouseMoveEvent(QMouseEvent *event) {
    if (QRect(QPoint(100 + length * 15, 754), QPoint(400 + length * 15, 334)).contains(event->pos())) {
        setCursor(Qt::PointingHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }

    QWidget::mouseMoveEvent(event);
}

void Animation::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);

    if (event->key() == Qt::Key_Left) {
        if (length - 3 > 5) {
            length -= 3;
        } else {
            length = 5;
        }
        initWater();
    }

    if (event->key() == Qt::Key_Right) {
        if (length + 3 < 50) {
            length += 3;
        } else {
            length = 50;
        }
        initWater();
    }

    if (event->key() == Qt::Key_Up) {
        boxMode = false;
        initWater();
    }

    if (event->key() == Qt::Key_Down) {
        boxMode = true;
        initWater();

    }

}

void Animation::initWater() {
    fluids = {{150, 660, 30, 30},
              {150, 600, 30, 30},
              {150, 540, 30, 30}};
    if (boxMode) {
        particles = {{length * 15 - 15 + 185, 500, 30, 30},
                     {length * 15 - 15 + 185, 560, 30, 30},
                     {length * 15 - 15 + 185, 620, 30, 30}};
    } else {
        particles = {{195, 290, 30, 30},
                     {195, 350, 30, 30},
                     {195, 410, 30, 30}};
    }
    // В initWater() происходит определение коэффициента выгодности метода
    // По графикам из статьи были получены линейные зависимости
    double box = 49.875 * volume + 452.5;
    double upper = 32.5 * length + 1775;
    double a;
    QString coef;
    // В зависимости от выбранного режима, вычисляем коэффициент
    if (boxMode) {
        a = box / upper;
        coef = QString(tr("Subsea module is %2 than platform by %1 times")).arg(a, 0, 'f', 2).arg(
                box > upper ? tr("more profitable") : tr("less profitable"));
    } else {
        a = upper / box;
        coef = QString(tr("Platform is %2 than subsea module by %1 times")).arg(a, 0, 'f', 2).arg(
                box > upper ? tr("less profitable") : tr("more profitable"));
    }
    // Выводим информацию о выбранном режиме
    info->setText(QString(tr("Pipeline length: %1 km.<br>Water injection volume: %2 thousand barrels/day.<br>")).arg(
            length).arg(
            volume) + coef);
    update();
}

void Animation::switchLanguage() {

    QApplication::removeTranslator(translator);
    if (translator->load(QString(":/resources/localization/PPD_%1.qm").arg(language))) {
        QApplication::installTranslator(translator);
    }

    fileMenu->setTitle(tr("Save"));
    fileMenu->actions()[0]->setText(tr("As PDF"));
    fileMenu->actions()[1]->setText(tr("As TXT"));

    settingsMenu->setTitle(tr("Settings"));
    settingsMenu->actions()[0]->setText(tr("Mode"));
    settingsMenu->actions()[1]->setText(tr("Parameters"));
    settingsMenu->actions()[2]->setText(tr("Language"));
    settingsMenu->actions()[3]->setText(tr("Change Name"));
    settingsMenu->actions()[4]->setText(tr("About"));

    setWindowTitle(tr("Water Flooding Systems"));

    initWater();
}

Animation::~Animation() {
    settings.beginGroup("/Settings");
    settings.setValue("/firstTime", firstTime);
    settings.setValue("/length", length);
    settings.setValue("/volume", volume);
    settings.setValue("/boxMode", boxMode);
    settings.setValue("/language", language);
    settings.setValue("/fullName", fullName);
    settings.endGroup();
    delete translator;
    delete langDialog;
    delete nameDialog;
    delete setDialog;
    delete choice;
    delete animationTimer;
    delete info;
    delete layout;
    delete menuBar;
    delete fileMenu;
    delete settingsMenu;
}

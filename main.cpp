#include <QApplication>
#include <QMediaPlayer>
#include <QMediaContent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QFileDialog>
#include <QWidget>
#include <QVideoWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Создаем главное окно приложения
    QWidget playerWindow;

    // Создаем вертикальный layout для управления виджетами
    QVBoxLayout vbox(&playerWindow);

    // Создаем горизонтальный layout для кнопок управления медиа
    QHBoxLayout buttons;

    // Создаем виджет для отображения видео
    auto* videoWidget = new QVideoWidget(&playerWindow);
    vbox.addWidget(videoWidget);

    // Создаем слайдер для перемотки аудио/видео
    QSlider* songSlider = new QSlider(&playerWindow);
    songSlider->setOrientation(Qt::Horizontal);
    vbox.addWidget(songSlider);

    // Создаем кнопки управления медиа
    auto* openFileButton = new QPushButton("Open", &playerWindow);
    auto* playMediaButton = new QPushButton("Play", &playerWindow);
    auto* pauseMediaButton = new QPushButton("Pause", &playerWindow);

    // Добавляем кнопки на горизонтальный layout
    buttons.addWidget(openFileButton);
    buttons.addWidget(playMediaButton);
    buttons.addWidget(pauseMediaButton);

    // Добавляем layout с кнопками в вертикальный layout
    vbox.addLayout(&buttons);

    // Путь к выбранному файлу
    QString filePath;

    // Создаем объект QMediaPlayer для воспроизведения медиа
    auto* player = new QMediaPlayer(&playerWindow);

    // Устанавливаем видеовыход для плеера
    player->setVideoOutput(videoWidget);

    // Флаг для отслеживания состояния воспроизведения
    bool isPlaying = false;

    // Подключаем обработчик события нажатия кнопки "Open"
    QObject::connect(openFileButton, &QPushButton::clicked, [&filePath, player, &isPlaying]() {
        // Открываем диалоговое окно для выбора файла
        filePath = QFileDialog::getOpenFileName(nullptr,
                                                 "open media",
                                                 QDir::currentPath() + "/opt/",
                                                 "Media files (*.mp3, *.mp4)");
        // Останавливаем воспроизведение медиа
        player->stop();
        isPlaying = false;
    });

    // Подключаем обработчик события нажатия кнопки "Play"
    QObject::connect(playMediaButton, &QPushButton::clicked, [&isPlaying, songSlider, player, &filePath]() {
        if (!isPlaying) {
            // Устанавливаем медиафайл для воспроизведения
            player->setMedia(QUrl::fromLocalFile(filePath));
            // Устанавливаем громкость
            player->setVolume(75);

            // Подключаем обработчик события изменения длительности медиа
            QObject::connect(player, &QMediaPlayer::durationChanged, [songSlider](qint64 duration) {
                // Устанавливаем диапазон для слайдера
                songSlider->setMinimum(0);
                songSlider->setMaximum(duration);
            });

            // Подключаем обработчик события изменения позиции воспроизведения
            QObject::connect(player, &QMediaPlayer::positionChanged, [songSlider](qint64 pos) {
                // Устанавливаем текущую позицию слайдера
                songSlider->setValue(pos);
            });

            isPlaying = true;
        }

        // Начинаем воспроизведение медиа
        player->play();
    });

    // Подключаем слайдер к плееру для управления позицией воспроизведения
    QObject::connect(songSlider, &QSlider::valueChanged, player, &QMediaPlayer::setPosition);

    // Подключаем кнопку "Pause" к плееру для приостановки воспроизведения
    QObject::connect(pauseMediaButton, &QPushButton::clicked, player, &QMediaPlayer::pause);

    // Отображаем главное окно приложения
    playerWindow.show();

    // Запускаем цикл обработки событий приложения
    return a.exec();
}


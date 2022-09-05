#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QSerialPort>
#include <QIODevice>
#include <QString>
#include <QByteArray>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
  \brief Утилита для использования\проверки работоспособности устройств семейства Mertech QR Display
*/

namespace mert {

    #define AWAITTIMEREAD 500
    #define AWAITTIMEWRITE 50

    static const uint8_t HEAD[] = {0x02, 0xf2, 0x02};
    static const uint8_t TAIL[] = {0x02, 0xf2, 0x03};
    static const uint8_t CLEAR[] = {0x02, 0xf0, 0x03, 0x43, 0x4c, 0x53, 0x03};
    static const uint8_t SCRON[] = {0x02, 0xf0, 0x03, 0x42, 0x54, 0x4f, 0x4e, 0x03};
    static const uint8_t SCROFF[] = {0x02, 0xf0, 0x03, 0x42, 0x54, 0x4f, 0x46, 0x46, 0x03};

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);

        ~MainWindow();

    private slots:
        /*!
            \brief Меню About
        */
        void on_actionAbout_triggered();

        /*!
            \brief слот подключения к комм\юсб
        */
        void on_pb_CONN_clicked();

        /*!
            \brief слот отключения
        */
        void on_pb_DISC_clicked();

        /*!
            \brief слот отчистки дисплея
        */
        void on_pb_CLR_clicked();

        /*!
            \brief слот Отправки буфера на дисплей
        */
        void on_pb_QR_clicked();

        /*!
            \brief слот Включения дисплея
        */
        void on_pb_ON_clicked();

        /*!
            \brief слот Отключения дисплея
        */
        void on_pb_OFF_clicked();

    private:
        /*!
            \brief Формирование пакета на влючение дисплея
        */
        void scrOn(void);

        /*!
            \brief Формирование пакета на отключение дисплея
        */
        void scrOff(void);

        /*!
            \brief Изменение стиля кнопки CONNECT
        */
        void chngColorConnected(void);

        /*!
            \brief Инит серийного порта
        */
        bool initSerialPort(void);

        /*!
            \brief подключение серийного порта
        */
        bool connectSerial(void);

        bool                    connected_ = false;
        QString                 serial_no_;
        QSerialPort             m_port_;
        Ui::MainWindow*         ui;
    };
}   //namespace mert
#endif // MAINWINDOW_H


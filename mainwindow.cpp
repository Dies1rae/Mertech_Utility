#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <sstream>
#include <cstring>
#include <QTimer>
#include <chrono>

namespace mert {
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow) {
        this->ui->setupUi(this);
        this->setFixedSize(this->size().width(), this->size().height());
        this->chngColorConnected();
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    bool MainWindow::initSerialPort(void) {
        if(this->serial_no_.isEmpty()) {
            return false;
        }
        this->m_port_.setPortName(this->serial_no_);
        this->m_port_.setBaudRate(QSerialPort::Baud115200);
        this->m_port_.setDataBits(QSerialPort::Data8);
        this->m_port_.setStopBits(QSerialPort::OneStop);
        this->m_port_.setParity(QSerialPort::NoParity);
        return true;
    }

    bool MainWindow::connectSerial(void) {
        if(!this->initSerialPort()) {
            this->connected_ = false;
            return this->connected_;
        }
        return this->connected_ ? this->connected_ : this->connected_ = this->m_port_.open(QIODevice::ReadWrite);
    }

    void MainWindow::on_actionAbout_triggered() {
        QMessageBox::information(
            this,
            tr("INFO"),
            tr("Gopotech utility is simple linux interface port\n"
               "for Mertech QR code payment devices created by\n"
               "Poltavskiy Nick aka. Sense_Net")
        );
    }

    void MainWindow::on_pb_CONN_clicked() {
        this->serial_no_ = this->ui->el_SERIAL->text();
        if(!this->connectSerial()) {
            QMessageBox::critical(
                this,
                tr("ERROR"),
                tr("Serial port open error\n")
            );
            this->ui->pb_CONN->setText("CONNECT");
        } else {
            this->ui->pb_CONN->setText("CONNECTED");
            this->on_pb_CLR_clicked();
        }
        this->chngColorConnected();
    }

    void MainWindow::on_pb_DISC_clicked() {
        this->ui->pb_CONN->setText("CONNECT");
        this->connected_ = false;
        this->chngColorConnected();
        this->m_port_.close();
    }

    void MainWindow::on_pb_CLR_clicked() {
        if(!this->connected_) {
            this->connectSerial();
        }
        std::string buff (7, 0);
        for(size_t ptr = 0; ptr < 7; ++ptr) {
            buff[ptr] = (char)CLEAR[ptr];
        }

        this->m_port_.waitForBytesWritten(AWAITTIMEWRITE);
        this->m_port_.write(buff.c_str(), buff.size());
        this->m_port_.waitForReadyRead(AWAITTIMEREAD);
        this->ui->l_BS->setText("0");
    }

    void MainWindow::scrOn(void) {
        if(!this->connected_) {
            this->connectSerial();
        }
        std::string buff (8, 0);
        for(size_t ptr = 0; ptr < 8; ++ptr) {
            buff[ptr] = (char)SCRON[ptr];
        }

        this->m_port_.waitForBytesWritten(AWAITTIMEWRITE);
        this->m_port_.write(buff.c_str(), buff.size());
        this->m_port_.waitForReadyRead(AWAITTIMEREAD);
    }

    void MainWindow::scrOff(void) {
        if(!this->connected_) {
            this->connectSerial();
        }
        std::string buff (9, 0);
        for(size_t ptr = 0; ptr < 9; ++ptr) {
            buff[ptr] = (char)SCROFF[ptr];
        }

        this->m_port_.waitForBytesWritten(AWAITTIMEWRITE);
        this->m_port_.write(buff.c_str(), buff.size());
        this->m_port_.waitForReadyRead(AWAITTIMEREAD);
        this->on_pb_DISC_clicked();
        QMessageBox::information(
            this,
            tr("information"),
            tr("Mertech connection closed and hardware was shutdown\n"
               "Need to restart Mertech display and reconnect\n"
               )
        );
    }

    void MainWindow::on_pb_QR_clicked() {
        std::string buff_str = this->ui->le_QR->text().toStdString();
        if(buff_str.empty()) {
            QMessageBox::critical(
                this,
                tr("ERROR"),
                tr("String is empty\n")
            );
            return;
        } else if(buff_str.size() > 1000) {
            QMessageBox::critical(
                this,
                tr("ERROR"),
                tr("String is too large\n")
            );
            return;
        }

        size_t len = 8 + buff_str.size();
        std::string buffer_qbt(len, 0);

        for(size_t ptr = 0; ptr < 3; ++ptr) {
            buffer_qbt[ptr] = HEAD[ptr];
        }
        buffer_qbt[3] = 0x00;
        buffer_qbt[4] = buff_str.size();
        for(size_t ptr = 0; ptr < buff_str.size(); ++ptr) {
            buffer_qbt[ptr + 5] = buff_str[ptr];
        }
        for(size_t ptr = 0; ptr < 3; ++ptr) {
            buffer_qbt[ptr + 5 + buff_str.size()] = TAIL[ptr];
        }

        this->ui->l_BS->setText(QString::fromStdString(std::to_string(buffer_qbt.size())));

        this->m_port_.waitForBytesWritten(AWAITTIMEWRITE);
        this->m_port_.write(buffer_qbt.c_str(), buffer_qbt.size());
        this->m_port_.waitForReadyRead(AWAITTIMEREAD);

        QTimer::singleShot(std::chrono::seconds(this->ui->sB_timer->value()), this, &MainWindow::on_pb_CLR_clicked);
    }

    void MainWindow::chngColorConnected(void) {
        if(this->connected_) {
            this->ui->pb_CONN->setStyleSheet("QPushButton {background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 186, 162, 255), stop:1 rgba(98, 211, 162, 255))}");
        } else {
            this->ui->pb_CONN->setStyleSheet("QPushButton {background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(205, 92, 92, 255), stop:1 rgba(220, 20, 60, 255))}");
        }
    }

    void mert::MainWindow::on_pb_ON_clicked() {
        this->scrOn();
    }

    void mert::MainWindow::on_pb_OFF_clicked() {
        this->scrOff();
    }
}   //namespace mert






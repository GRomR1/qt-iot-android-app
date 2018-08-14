#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mqtt/qmqttclient.h>
//#include <QtMqtt/QMqttClient>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#define MQTT_HOST "dev.s3t.club"
#define INFLUXDB_HOST "dev.s3t.club"
#define INFLUXDB_PORT "8087"
#define INFLUXDB_USER "admin"
#define INFLUXDB_PASS "ertdfg"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_lampButton_toggled(bool checked);
    void on_lightSensorButton_toggled(bool checked);
    void newMessage(const QMqttMessage &m);
    void connected();
    void on_likeButton_clicked();
    void replyFinish(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QMqttClient *m_client;
    QMqttSubscription *m_subs;
    QNetworkAccessManager *nwam;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QScreen>
#include <QSize>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nwam = new QNetworkAccessManager;
    connect(nwam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinish(QNetworkReply*)));

    QUrlQuery query;
    query.addQueryItem("db","db");
    query.addQueryItem("u",INFLUXDB_USER);
    query.addQueryItem("p",INFLUXDB_PASS);
    query.addQueryItem("q","SELECT sum(value) FROM sensors WHERE topic = '/sensors/like'");
    QString apiUrl(QString("http://")+INFLUXDB_HOST+":"+INFLUXDB_PORT+"/query");
    QUrl url(apiUrl);
    url.setQuery(query);

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("application/x-www-form-urlencoded"));
    nwam->get(request);

    m_client = new QMqttClient(this);
    m_client->setHostname(QString(MQTT_HOST));
    m_client->setPort(1884);
    m_client->connectToHost();
    QObject::connect(m_client, SIGNAL(connected()), this, SLOT(connected()));

    QScreen *screen = QApplication::screens().at(0);
    QSize size(screen->size().width(), screen->size().height());
    qDebug() << "screens size" << size;


    ui->lampButton->setIcon(QIcon(QPixmap(":/light-bulb-off.png")));
    ui->lightSensorButton->setIcon(QIcon(QPixmap(":/light-sensor-on.png")));
    ui->lampButton->setIconSize(QSize(size.width(),size.width()));
    ui->lightSensorButton->setIconSize(QSize(size.height()/7,size.height()/7));

    //  hide useless like button
    ui->likeButton->hide();
    ui->likeLabel->hide();
    //    ui->likeButton->setIcon(QIcon(QPixmap(":/thumbs-up.png")));
    //    ui->likeButton->setIconSize(QSize(size.height()/10,size.height()/10));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lampButton_toggled(bool checked)
{
    qDebug() << "lamp button" << checked;
    if(checked){
        ui->lampButton->setIcon(QIcon(QPixmap(":/light-bulb-on.png")));
        m_client->publish(QMqttTopicName("/sensors/button"),
                          QByteArray("{\"value\":1, \"command\": \"set\", \"user\": \"mobile\"}"));
    }
    else {
        ui->lampButton->setIcon(QIcon(QPixmap(":/light-bulb-off.png")));
        m_client->publish(QMqttTopicName("/sensors/button"),
                          QByteArray("{\"value\":0, \"command\": \"set\", \"user\": \"mobile\"}"));
    }
}

void MainWindow::on_lightSensorButton_toggled(bool checked)
{
    qDebug() << "light button" << checked;
    if(checked){
        ui->lightSensorButton->setIcon(QIcon(QPixmap(":/light-sensor-off.png")));
        m_client->publish(QMqttTopicName("/sensors/disable_light"),
                          QByteArray("{\"value\":1, \"command\": \"set\", \"user\": \"mobile\"}"));
    }
    else{
        ui->lightSensorButton->setIcon(QIcon(QPixmap(":/light-sensor-on.png")));
        m_client->publish(QMqttTopicName("/sensors/disable_light"),
                          QByteArray("{\"value\":0, \"command\": \"set\", \"user\": \"mobile\"}"));
    }
}

void MainWindow::newMessage(const QMqttMessage &m)
{
    qDebug() << "messageReceived";
    qDebug() << "message" << m.payload();
    if(m.payload() == "{\"value\":0, \"command\": \"set\", \"user\": \"device\"}")
        this->ui->lampButton->setChecked(false);
    else if (m.payload() == "{\"value\":1, \"command\": \"set\", \"user\": \"device\"}") {
        this->ui->lampButton->setChecked(true);
    }
}


void MainWindow::connected()
{
    qDebug() << "connected";

    m_subs = m_client->subscribe(QMqttTopicFilter("/sensors/lamp"));
    if(m_subs)
        qDebug() << "m_subs" << m_subs->state();
        connect(m_subs, &QMqttSubscription::messageReceived,
                this, &MainWindow::newMessage);
}


void MainWindow::on_likeButton_clicked()
{
    m_client->publish(QMqttTopicName("/sensors/like"),
                      QByteArray("{\"value\":1, \"command\": \"add\", \"user\": \"mobile\"}"));
    auto likes = this->ui->likeLabel->text().toInt();
    this->ui->likeLabel->setText(QString::number(likes+1));
}


void MainWindow::replyFinish(QNetworkReply *reply)
{
//    QString answer = QString::fromUtf8(reply->readAll());
//    qDebug() << "answer" << answer;

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QNetworkReply::NetworkError ne = reply->error();
    qDebug() << "ne" << ne;

    if(document.isObject()){
        QJsonObject json = document.object();
        QJsonArray results = json.value("results").toArray();
        QJsonValue series_json = results.at(0).toObject().value("series");
        if (series_json != QJsonValue::Undefined){
            QJsonArray series = series_json.toArray();
            QJsonArray values = series.at(0).toObject().value("values").toArray();
            QJsonValue time = values.at(0).toArray().at(0);
            QJsonValue value = values.at(0).toArray().at(1);
            qDebug() << time.toString();
            qDebug() << value.toInt(0);

            this->ui->likeLabel->setText(QString::number(value.toInt(0)));
        }
    }

}

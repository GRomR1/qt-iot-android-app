# qt-iot-android-app

An example Android application to control home light (power on/off).
I wrote this project to demostrate my IoT stand in iVolga young people forum.

It is communicating with a private MQTT Server.
A configured ESP8266 module connected with the MQTT Server and listen the commands from the application.
Also all sended commands store in InfluxDB database. And a Grafana system visualizes them.
I use QNetworkAccessManager and QJsonDocument for getting and parsing some data from the InfluxDB server.

The project was fully include internal QtMqtt library. It is not right, but it works.

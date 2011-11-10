#include "downloadwidget.h"

#include <QDesktopServices>
#include <QLabel>
#include <QDebug>

DownloadWidget::DownloadWidget(QWidget *parent, QNetworkAccessManager *qnam) :
    QWidget(parent)
{
    networkAccessManager = qnam;
}

void DownloadWidget::startDownload(QString url, QString fileName, QString fetchUrl) {
    QGridLayout *layout = new QGridLayout(this);
    progressBar = new QProgressBar(this);
    killButton = new QPushButton("Avbryt", this);
    QLabel *label = new QLabel(fetchUrl, this);
    layout->addWidget(label, 0, 0, 1, 2);
    layout->addWidget(progressBar, 1, 0);
    layout->addWidget(killButton, 1, 1);
    connect(killButton, SIGNAL(clicked()), this, SLOT(on_killButtonClicked()));
    setLayout(layout);

    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly);

    networkReply = networkAccessManager->get(QNetworkRequest(QUrl(url)));
    connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(networkReply, SIGNAL(readyRead()), this, SLOT(writeToFile()));
    connect(networkReply, SIGNAL(finished()), this, SLOT(finished()));
}

void DownloadWidget::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    progressBar->setValue((int)((double)bytesReceived/(double)bytesTotal*100.0));
}

void DownloadWidget::writeToFile() {
    file.write(networkReply->read(networkReply->bytesAvailable()));
}

void DownloadWidget::on_killButtonClicked() {
    emit kill();
}

void DownloadWidget::finished() {
    networkReply->deleteLater();
    killButton->setText("Stäng");
}
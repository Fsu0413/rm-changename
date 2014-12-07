#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <QThread>
#include <QStringList>

#include <QNetworkReply>

class Downloader : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QStringList downloadSequence READ downloadSequence)
    Q_PROPERTY(QStringList failedList READ failedList)
    Q_PROPERTY(QString savePath READ savePath WRITE setSavePath)

public:
    virtual void run();

    inline Downloader &operator <<(const QString &filename) {
        m_downloadSequence << filename;
        return *this;
    }

    inline const QStringList &downloadSequence() const { return m_downloadSequence; }
    inline const QStringList &failedList() const { return m_failedList; }
    inline const QString &savePath() const { return m_savePath; }
    inline void setSavePath(const QString &sp) { m_savePath = sp; }

private:
    void downloadSingleFile();

private slots:
    void singleFileFinished();
    void singleFileError(QNetworkReply::NetworkError e);

signals:
    void all_completed();
    void one_completed(const QString &url);
    void one_failed(const QString &url);
    void error();

private:
    QStringList m_downloadSequence;
    QStringList m_failedList;
    QString m_savePath;
    QString m_currentDownloadingFile;
};

#endif
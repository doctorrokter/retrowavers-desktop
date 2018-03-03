#ifndef TRACKSCONTROLLER_H
#define TRACKSCONTROLLER_H

#include <QObject>
#include <QObject>
#include <QVariantMap>
#include "tracksservice.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TracksController : public QObject {
    Q_OBJECT
    Q_PROPERTY(int playerMode READ getPlayerMode WRITE setPlayerMode NOTIFY playerModeChanged)
    Q_PROPERTY(int index READ getIndex NOTIFY indexChanged)
public:
    enum PlayerMode {
        Playlist = 0,
        Favourite
    };
    Q_ENUMS(PlayerMode)

    explicit TracksController();
    explicit TracksController(TracksService* tracksService, QObject* parent = nullptr);
    virtual ~TracksController();

    Q_INVOKABLE bool play(const QVariantMap& track);
    Q_INVOKABLE bool next();
    Q_INVOKABLE bool prev();
    Q_INVOKABLE void like();
    Q_INVOKABLE void removeFavourite(const QVariantMap& track);

    Q_INVOKABLE int getPlayerMode() const;
    Q_INVOKABLE void setPlayerMode(const int& playerMode);

    const int& getIndex() const;

signals:
    void played(const QVariantMap& track);
    void liked(const QString& id);
    void downloaded(const QString& id);
    void downloadProgress(const QString& id, qint64 sent, qint64 total);
    void playerModeChanged(const int& playerMode);
    void favouriteTrackRemoved(const QString& id);
    void indexChanged(const int& index);

private slots:
    void onDownload();
    void onDownloadError(QNetworkReply::NetworkError e);
    void onDownloadProgress(qint64 sent, qint64 total);

private:
    TracksService* m_pTracksService;
    int m_index;
    int m_favIndex;
    int m_playerMode;

    QNetworkAccessManager* m_pNetwork;

    void download(Track* track);

};

#endif // TRACKSCONTROLLER_H

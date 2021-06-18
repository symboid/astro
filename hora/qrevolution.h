
#ifndef __SYMBOID_ASTRO_HORA_QREVOLUTION_H__
#define __SYMBOID_ASTRO_HORA_QREVOLUTION_H__

#include "astro/hora/defs.h"
#include "sdk/controls/qcalctask.h"
#include <QAbstractListModel>
#include <QSharedPointer>
#include "astro/hora/qhoracoords.h"
#include "astro/hora/qhora.h"

class ASTRO_HORA_API QRevolutionListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    QRevolutionListModel(QObject* parent);

private:
    enum Roles
    {
        DateTimeRole = Qt::UserRole,
        RetrogradRole
    };
public:
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    struct RevolutionData
    {
        QSharedPointer<QHoraCoords> mCoords;
        bool mIsRetrograd = false;
    };
    QVector<RevolutionData> mRevolutions;
public:
    void clear();
    void append(const QSharedPointer<QHoraCoords>& coords, bool isRetrograd);
};

class ASTRO_HORA_API QRevolution : public QCalcable
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "Revolution";

public:
    QRevolution(QObject* parent = Q_NULLPTR);
    ~QRevolution();

public slots:
    void calc() override;

public:
    Q_PROPERTY(QHora* hora READ hora WRITE setHora NOTIFY horaChanged)
    QHora* hora() const;
    void setHora(QHora* hora);
private:
    QHora* mHora;
signals:
    void horaChanged();

public:
    Q_PROPERTY(QStringList planetModel READ planetModel CONSTANT)
    const QStringList& planetModel() const;
private:
    QStringList mPlanetModel;

public:
    Q_PROPERTY(int planetIndex READ planetIndex WRITE setPlanetIndex NOTIFY planetIndexChanged)
    Q_PROPERTY(QString planetName READ planetName NOTIFY planetIndexChanged)
    Q_PROPERTY(int defaultRevCount READ defaultRevCount NOTIFY planetIndexChanged)
    int planetIndex() const;
    void setPlanetIndex(int planetIndex);
    QString planetName() const;
    int defaultRevCount() const;
private:
    int mPlanetIndex;
signals:
    void planetIndexChanged();

public:
    Q_PROPERTY(qreal planetLont READ planetLont NOTIFY planetLontChanged)
    qreal planetLont() const;
signals:
    void planetLontChanged();

public:
    Q_PROPERTY(QAbstractListModel* list READ list CONSTANT)
    QAbstractListModel* list() const;
private:
    QRevolutionListModel* mList;
};

#endif // __SYMBOID_ASTRO_HORA_QREVOLUTION_H__

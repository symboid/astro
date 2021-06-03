
#ifndef __SYMBOID_ASTRO_HORA_QECLIPTICTABLEMODEL_H__
#define __SYMBOID_ASTRO_HORA_QECLIPTICTABLEMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraitemsmodel.h"

class ASTRO_HORA_API QEclipticTableModel : public QHoraTableModel
{
    Q_OBJECT

public:
    QEclipticTableModel(QObject* parent = Q_NULLPTR);

protected:
    enum {
        SymbolRole = Qt::UserRole,
        EclLontRole,
        EclLattRole,
        EclSpeedRole,
    };

public:
    QHash<int, QByteArray> roleNames() const override;

public:
    Q_PROPERTY(bool withSpeed MEMBER mWithSpeed WRITE setWithSpeed NOTIFY withSpeedChanged)
protected:
    bool mWithSpeed;
public:
    void setWithSpeed(bool withSpeed);
signals:
    void withSpeedChanged();
};

class ASTRO_HORA_API QHoraPlanetsModel : public QEclipticTableModel
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "HoraPlanetsModel";

public:
    QHoraPlanetsModel(QObject* parent = Q_NULLPTR);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList headerModel() const override;
};

class ASTRO_HORA_API QHoraHousesModel : public QEclipticTableModel
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "HoraHousesModel";

public:
    QHoraHousesModel(QObject* parent = Q_NULLPTR);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList headerModel() const override;
};

#endif // __SYMBOID_ASTRO_HORA_QECLIPTICTABLEMODEL_H__

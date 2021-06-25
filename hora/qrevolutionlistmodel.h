
#ifndef __SYMBOID_ASTRO_HORA_QREVOLUTIONLISTMODEL_H__
#define __SYMBOID_ASTRO_HORA_QREVOLUTIONLISTMODEL_H__

#include "astro/hora/defs.h"
#include "sdk/controls/qcalclistmodel.h"
#include "astro/hora/qrevolution.h"

class ASTRO_HORA_API QRevolutionListModel : public QCalcListModel
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "RevolutionListModel";

public:
    QRevolutionListModel(QObject* parent = Q_NULLPTR);

public:
    Q_PROPERTY(QRevolution* revolutionCalc READ revolutionCalc WRITE setRevolutionCalc NOTIFY revolutionCalcChanged)
    QRevolution* revolutionCalc() const;
    void setRevolutionCalc(QRevolution* revolutionCalc);
private:
    QRevolution* mRevolutionCalc;
signals:
    void revolutionCalcChanged();

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
};


#endif // __SYMBOID_ASTRO_HORA_QREVOLUTIONLISTMODEL_H__


#ifndef __SYMBOID_ASTRO_HORA_QREVOLUTION_H__
#define __SYMBOID_ASTRO_HORA_QREVOLUTION_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhora.h"
#include "sdk/controls/qcalcproperty.h"
#include "astro/hora/qrevolutioncalcmodel.h"

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

    Q_CALC_PROPERTY(qreal, revTzDiff)

    Q_CALC_PROPERTY(int, planetIndex)
    Q_CALC_PROPERTY(int, year)
    Q_CALC_PROPERTY(int, revCount)

public:
    Q_PROPERTY(QStringList planetModel READ planetModel CONSTANT)
    const QStringList& planetModel() const;
private:
    QStringList mPlanetModel;

public:
    Q_PROPERTY(QString planetName READ planetName NOTIFY planetIndexChanged)
    Q_PROPERTY(int defaultRevCount READ defaultRevCount NOTIFY planetIndexChanged)
    QString planetName() const;
    int defaultRevCount() const;

public:
    Q_PROPERTY(qreal planetLont READ planetLont NOTIFY planetLontChanged)
    qreal planetLont() const;
signals:
    void planetLontChanged();

public:
    QVector<QRevolutionData> mRevolutions;
public:
    int revolutionsCount() const;
    const QRevolutionData& revolutionData(int index) const;
};

#endif // __SYMBOID_ASTRO_HORA_QREVOLUTION_H__

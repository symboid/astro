
#include "astro/hora/setup.h"
#include "astro/hora/qhoraitemsmodel.h"

QHoraTableModel::QHoraTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , mHora(nullptr)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
    , mHorzHeaderModel(new HorzHeaderModel(this))
    , mVertHeaderModel(new VertHeaderModel(this))
{
}

QHora* QHoraTableModel::hora() const
{
    return mHora;
}

void QHoraTableModel::setHora(QHora* hora)
{
    if (mHora)
    {
        disconnect(mHora, SIGNAL(changed()), this, SLOT(update()));
    }
    if (mHora != hora)
    {
        if ((mHora = hora))
        {
            connect(mHora, SIGNAL(changed()), this, SLOT(update()));
        }
        emit horaChanged();
    }
}

int QHoraTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return horzHeaderTitles().length();
}

const QStringList& QHoraTableModel::horzHeaderTitles() const
{
    return mHorzHeaderTitles;
}

void QHoraTableModel::setHorzHeaderTitles(const QStringList& horzHeaderTitles)
{
    mHorzHeaderTitles = horzHeaderTitles;
    emit horzHeaderTitlesChanged();
}

void QHoraTableModel::update()
{
    beginResetModel();
    endResetModel();
}

QHoraTableModel::HorzHeaderModel::HorzHeaderModel(QHoraTableModel* parentModel)
    : QAbstractTableModel(parentModel)
    , mParentModel(parentModel)
{
    connect(mParentModel, SIGNAL(horzHeaderTitlesChanged()), this, SIGNAL(modelReset()));
}

int QHoraTableModel::HorzHeaderModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

int QHoraTableModel::HorzHeaderModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return mParentModel->horzHeaderTitles().length();
}

QVariant QHoraTableModel::HorzHeaderModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role);
    return mParentModel->horzHeaderTitles()[index.column()];
}

QHoraTableModel::VertHeaderModel::VertHeaderModel(QHoraTableModel* parentModel)
    : QAbstractListModel(parentModel)
    , mParentModel(parentModel)
{
    connect(mParentModel, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
    connect(mParentModel, SIGNAL(modelReset()), this, SIGNAL(modelReset()));
}

int QHoraTableModel::VertHeaderModel::rowCount(const QModelIndex& parent) const
{
    return mParentModel->rowCount(parent);
}

QVariant QHoraTableModel::VertHeaderModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role)
    return mParentModel->vertHeaderTitle(index.row());
}

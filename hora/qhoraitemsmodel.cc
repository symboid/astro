
#include "astro/hora/setup.h"
#include "astro/hora/qhoraitemsmodel.h"

QHoraTableModel::QHoraTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , mHora(nullptr)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
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
    return horzHeaderModel().size();
}

void QHoraTableModel::update()
{
    beginResetModel();
    endResetModel();
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

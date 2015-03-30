#include "reactivetexthelper.h"

#include <QDebug>
#include <QMetaProperty>


#include <string>
#include <algorithm>
#include <vector>



//class Property : public QObject
//{
//    Q_OBJECT

//public:
//    Q_INVOKABLE void set(QObject *object, const QString &name, QVariant value);
//    Q_INVOKABLE QVariant get(QObject *object, const QString &name) const;
//};
//void Property::set(QObject *object, const QString &name, QVariant value)
//{
//    const auto *metaObject = object->metaObject();
//    const int index = metaObject->indexOfProperty(name.toLatin1());
//    if (index != -1)
//    {
//        const auto metaProperty = metaObject->property(index);
//        metaProperty.write(object, value);
//    }
//    else
//    {
//        qDebug() << "Property dynamic set error: property " << name << " not found";
//    }
//}
//QVariant Property::get(QObject *object, const QString &name) const
//{
//    const auto *metaObject = object->metaObject();
//    const int index = metaObject->indexOfProperty(name.toLatin1());
//    if (index != -1)
//    {
//        const auto metaProperty = metaObject->property(index);
//        return metaProperty.read(object);
//    }
//    else
//    {
//        qDebug() << "Property dynamic set error: property " << name << " not found";
//    }
//}


ReactiveTextHelper::ReactiveTextHelper(QObject *parent):
    QObject(parent),
    m_model(nullptr),
    m_textItem(nullptr)
{
}

ReactiveTextHelper::~ReactiveTextHelper()
{

}


QString ReactiveTextHelper::formatQmlItemText(const QString &text)
{
    const auto &inputText = text.toStdString();

    std::string interpolatedString;
    interpolatedString.reserve(inputText.size());

    size_t currentPos = 0;

    interpolateString(inputText, [&](size_t begin, size_t end, size_t vBegin, size_t vEnd)
        {
            interpolatedString += std::string(inputText, currentPos, begin - currentPos);
            const auto variable = std::string(inputText, vBegin, vEnd-vBegin);

            const auto value = tryLoadVairableModelByName(variable);
            if (!value.isNull())
            {
                interpolatedString += formatQmlTextLink(variable, value.toString().toStdString());

                if (subscribeToPropertyChanges(variable))
                {
                    qDebug() << "Subscribed to property changes: " << QString::fromStdString(variable);
                }
            }
            else
            {
                interpolatedString += variable;
            }

            currentPos = end;
        }, [&](size_t begin, size_t) {
            qDebug() << "Error at: " << begin;
        });

    interpolatedString += std::string(inputText, currentPos);

    return QString::fromStdString(interpolatedString);
}


void ReactiveTextHelper::onReactiveNumberClicked(const QString &numberId)
{
    auto &subscriptions = modelsSubscriptionsSet();

    const auto modelIt = subscriptions.find(numberId.toStdString());
    if (modelIt != subscriptions.end())
    {
        const auto modelInstance = model();
        if (modelInstance != nullptr)
        {
            const auto metaObject = m_model->metaObject();
            const auto propertyIndex = metaObject->indexOfProperty(numberId.toLatin1().data());
            if (propertyIndex != -1)
            {
                const QMetaProperty metaProperty = metaObject->property(propertyIndex);
                const QVariant propertyValue = metaProperty.read(m_model);
                metaProperty.write(m_model, propertyValue.value<int>() + 1);
            }
        }
    }
}


QVariant ReactiveTextHelper::tryLoadVairableModelByName(const std::string &name) const
{
    const auto modelInstance = model();
    if (modelInstance != nullptr)
    {
        const auto metaObject = m_model->metaObject();
        const auto propertyIndex = metaObject->indexOfProperty(name.c_str());
        if (propertyIndex != -1)
        {
            const QMetaProperty metaProperty = metaObject->property(propertyIndex);
            const QVariant propertyValue = metaProperty.read(m_model);
            return propertyValue;
        }
    }

    return QVariant();
}

bool ReactiveTextHelper::subscribeToPropertyChanges(const std::string &name)
{
    bool result = false;

    auto &subscriptions = modelsSubscriptionsSet();

    const auto modelIt = subscriptions.find(name);
    if (modelIt == subscriptions.end())
    {
        const auto modelInstance = model();
        if (modelInstance != nullptr)
        {
            const auto metaObject = m_model->metaObject();
            const auto propertyIndex = metaObject->indexOfProperty(name.c_str());
            if (propertyIndex != -1)
            {
                const QMetaProperty metaProperty = metaObject->property(propertyIndex);
                const auto signalMetaMethod = metaProperty.notifySignal();

                static int thisSlotIndexCached = -1;
                static QMetaMethod thisSlotMetaMethodChached;
                if (thisSlotIndexCached == -1)
                {
                    auto thisSlotIndex = this->metaObject()->indexOfSlot("onTargetModelAnyPropertyChanged()");
                    if (thisSlotIndex != -1)
                        thisSlotIndexCached = thisSlotIndex;
                    thisSlotMetaMethodChached = QMetaMethod(this->metaObject()->method(thisSlotIndexCached));
                }

                result = QObject::connect(modelInstance, signalMetaMethod, this, thisSlotMetaMethodChached);
            }
        }
    }

    if (result)
    {
        subscriptions.insert(name);
    }

    return result;
}


void ReactiveTextHelper::onTargetModelAnyPropertyChanged()
{
    interpolateSourceText();
}


/*static */
std::string ReactiveTextHelper::formatQmlTextLink(const std::string &varModel, const std::string &varValue)
{
    return "<a href=\"" + varModel + "\">" + varValue + "</a>";
}

/*static */
void ReactiveTextHelper::interpolateString(const std::string &inputText, OnVariableFoundCallback callback, OnErrorCallback errorCallback)
{
    Q_UNUSED(errorCallback);

    int opened = 0, closed = 0;
    size_t startPos = 0;

    unsigned currentPosition = 0;
    for (auto currentCharIt = inputText.begin();
                    currentCharIt != inputText.end();
                    ++currentCharIt, ++currentPosition)
    {
        const auto currentChar = *currentCharIt;

        if (currentChar == '{')
            opened += 1;
        else if (currentChar == '}')
            closed += 1;
        else if (opened != 2)
            opened = closed = 0;

        if (opened == 1)
        {
            startPos = currentPosition;
        }
        if (closed == 2)
        {
            opened = 0, closed = 0;

            const auto begin = startPos;
            const auto end = currentPosition + 1;

            callback(begin, end, begin + 2, end - 2);
        }
    }
}

void ReactiveTextHelper::setModel(ReactiveTextModel *value)
{
    m_model = value;
    emit modelChanged();
    interpolateSourceText();
}

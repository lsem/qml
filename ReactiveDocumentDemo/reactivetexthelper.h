#ifndef REACTIVETEXTHELPER_H
#define REACTIVETEXTHELPER_H

#include <QObject>
#include <QVariant>
#include <QDebug>

#include <string>
#include <functional>
#include <set>
#include <map>


class ReactiveTextModel : public QObject
{
    Q_OBJECT
public:

};

class ReactiveTextHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ReactiveTextModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    explicit ReactiveTextHelper(QObject *parent = 0);
    ~ReactiveTextHelper();

    Q_INVOKABLE QString formatQmlItemText(const QString &text);

public slots:
    //void onReactiveNumberHovered(const QString &numberId);
    void onReactiveNumberClicked(const QString &numberId);

signals:
    void modelChanged();
    void textChanged();

private:
    QVariant tryLoadVairableModelByName(const std::string &name) const;
    bool subscribeToPropertyChanges(const std::string &name);

private slots:
    void onTargetModelAnyPropertyChanged();

private:
    typedef std::function<void(size_t begin, size_t end, size_t varBegin, size_t varEnd)> OnVariableFoundCallback;
    typedef std::function<void(size_t begin, size_t end)> OnErrorCallback;
    static void interpolateString(const std::string &inputText, OnVariableFoundCallback callback, OnErrorCallback errorCallback);
    static std::string formatQmlTextLink(const std::string &varModel, const std::string &varValue);

private:
    ReactiveTextModel *model() const { return m_model; }
    void setModel(ReactiveTextModel *value);

    QString text() const {return m_interpolatedText; }
    void setText(QString value) { m_sourceText = value; interpolateSourceText(); }

private:
    void interpolateSourceText() { m_interpolatedText = formatQmlItemText(m_sourceText); emit textChanged(); }

private:
    typedef std::set<std::string> ModelsSubscriptionsSet;

private:
    ModelsSubscriptionsSet &modelsSubscriptionsSet() { return m_modelsSubscription; }

private:
    ReactiveTextModel *m_model;
    QObject *m_textItem;
    QString m_interpolatedText;
    QString m_sourceText;
    ModelsSubscriptionsSet m_modelsSubscription;

};

#endif // REACTIVETEXTHELPER_H

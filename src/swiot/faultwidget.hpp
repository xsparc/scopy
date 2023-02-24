#ifndef SCOPY_FAULTWIDGET_HPP
#define SCOPY_FAULTWIDGET_HPP

#include <QWidget>
#include "ui_faultwidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class faultwidget; }
QT_END_NAMESPACE

class FaultWidget : public QWidget {
Q_OBJECT

public:
    explicit FaultWidget(QWidget *parent = nullptr);
    explicit FaultWidget(unsigned int id, QString name, QString faultExplanation, QWidget *parent = nullptr);

    ~FaultWidget() override;

private:
    Ui::FaultWidget *ui;
public:
    unsigned int getId() const;

    void setId(unsigned int id);

    bool isStored() const;

    void setStored(bool stored);

    bool isActive() const;

    void setActive(bool active);

    const QString &getName() const;

    void setName(const QString &name);

    const QString &getFaultExplanation() const;

    void setFaultExplanation(const QString &faultExplanation);

private:
    bool stored;
    bool active;
    unsigned int id;
    QString name;
    QString faultExplanation;
};


#endif //SCOPY_FAULTWIDGET_HPP

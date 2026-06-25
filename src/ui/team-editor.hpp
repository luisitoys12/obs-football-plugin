#pragma once

#include <QColor>
#include <QDialog>
#include <QString>

class QLineEdit;

class TeamEditorDialog : public QDialog {
    Q_OBJECT

public:
    explicit TeamEditorDialog(QWidget *parent = nullptr);

    QString TeamName() const;
    QString TeamShortName() const;
    QColor PrimaryColor() const;
    QColor SecondaryColor() const;

private slots:
    void PickPrimaryColor();
    void PickSecondaryColor();

private:
    QLineEdit *teamNameEdit = nullptr;
    QLineEdit *teamShortNameEdit = nullptr;
    QColor primaryColor;
    QColor secondaryColor;
};

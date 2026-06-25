#include "team-editor.hpp"

#include <QColorDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

TeamEditorDialog::TeamEditorDialog(QWidget *parent)
    : QDialog(parent),
      primaryColor(0, 102, 204),
      secondaryColor(255, 255, 255)
{
    setWindowTitle("Team Editor");
    setMinimumWidth(320);

    auto *layout = new QVBoxLayout(this);
    auto *form = new QFormLayout();

    teamNameEdit = new QLineEdit(this);
    teamShortNameEdit = new QLineEdit(this);
    teamShortNameEdit->setMaxLength(4);

    auto *primaryBtn = new QPushButton("Primary Color", this);
    auto *secondaryBtn = new QPushButton("Secondary Color", this);

    connect(primaryBtn, &QPushButton::clicked, this, &TeamEditorDialog::PickPrimaryColor);
    connect(secondaryBtn, &QPushButton::clicked, this, &TeamEditorDialog::PickSecondaryColor);

    form->addRow("Team Name", teamNameEdit);
    form->addRow("Short Name", teamShortNameEdit);
    form->addRow(primaryBtn);
    form->addRow(secondaryBtn);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(form);
    layout->addWidget(buttons);
}

QString TeamEditorDialog::TeamName() const
{
    return teamNameEdit->text();
}

QString TeamEditorDialog::TeamShortName() const
{
    return teamShortNameEdit->text();
}

QColor TeamEditorDialog::PrimaryColor() const
{
    return primaryColor;
}

QColor TeamEditorDialog::SecondaryColor() const
{
    return secondaryColor;
}

void TeamEditorDialog::PickPrimaryColor()
{
    QColor picked = QColorDialog::getColor(primaryColor, this, "Primary Color");
    if (picked.isValid())
        primaryColor = picked;
}

void TeamEditorDialog::PickSecondaryColor()
{
    QColor picked = QColorDialog::getColor(secondaryColor, this, "Secondary Color");
    if (picked.isValid())
        secondaryColor = picked;
}

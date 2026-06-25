#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <obs-module.h>

// SettingsDialog — API key, auto-refresh interval, default league
class SettingsDialog : public QDialog {
public:
    explicit SettingsDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle(obs_module_text("FootballPluginSettings"));
        setMinimumWidth(420);

        auto *layout = new QVBoxLayout(this);
        auto *form   = new QFormLayout();

        // API Key
        apiKeyEdit = new QLineEdit(this);
        apiKeyEdit->setPlaceholderText("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
        apiKeyEdit->setEchoMode(QLineEdit::Password);
        form->addRow(obs_module_text("APIKey"), apiKeyEdit);

        // Auto-refresh
        refreshSpin = new QSpinBox(this);
        refreshSpin->setRange(10, 300);
        refreshSpin->setValue(30);
        refreshSpin->setSuffix(" s");
        form->addRow(obs_module_text("AutoRefreshInterval"), refreshSpin);

        // Default league
        leagueCombo = new QComboBox(this);
        leagueCombo->addItem("Liga MX",      262);
        leagueCombo->addItem("Champions",    2);
        leagueCombo->addItem("Premier",      39);
        leagueCombo->addItem("LaLiga",       140);
        leagueCombo->addItem("Copa America", 9);
        leagueCombo->addItem("World Cup",    1);
        form->addRow(obs_module_text("DefaultLeague"), leagueCombo);

        layout->addLayout(form);

        auto *btnSave = new QPushButton(obs_module_text("Save"), this);
        connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);
        layout->addWidget(btnSave);
    }

    QString GetApiKey()      const { return apiKeyEdit->text(); }
    int     GetRefreshSecs() const { return refreshSpin->value(); }
    int     GetLeagueId()    const { return leagueCombo->currentData().toInt(); }

private:
    QLineEdit *apiKeyEdit;
    QSpinBox  *refreshSpin;
    QComboBox *leagueCombo;
};

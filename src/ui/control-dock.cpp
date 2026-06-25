#include "control-dock.hpp"

#include <QGridLayout>
#include <QMainWindow>
#include <QMetaObject>
#include <QVBoxLayout>
#include <obs-frontend-api.h>

namespace {
ControlDock *g_dock = nullptr;
}

void ControlDock::AddToOBS()
{
    if (g_dock)
        return;

    auto *mainWindow = static_cast<QMainWindow *>(obs_frontend_get_main_window());
    if (!mainWindow)
        return;

    g_dock = new ControlDock(mainWindow);
    g_dock->setObjectName("obs-football-control-dock");
    obs_frontend_add_dock(g_dock);
}

ControlDock::ControlDock(QWidget *parent)
    : QDockWidget(QStringLiteral("Football Control"), parent)
{
    scoreboard = new Scoreboard();
    timer = new MatchTimer();

    qtTimer = new QTimer(this);
    connect(qtTimer, &QTimer::timeout, this, &ControlDock::OnTimerTick);
    qtTimer->start(1000);

    SetupUI();
    RefreshTimerDisplay();
}

ControlDock::~ControlDock()
{
    delete scoreboard;
    delete timer;
}

void ControlDock::SetupUI()
{
    auto *root = new QWidget(this);
    auto *layout = new QVBoxLayout(root);
    auto *grid = new QGridLayout();

    lblScore = new QLabel("0 - 0", root);
    lblTimer = new QLabel("00:00", root);
    lblPeriod = new QLabel("1T", root);

    btnGoalHome = new QPushButton("Goal Home", root);
    btnGoalAway = new QPushButton("Goal Away", root);
    btnStartTimer = new QPushButton("Start", root);
    btnPauseTimer = new QPushButton("Pause", root);
    auto *btnReset = new QPushButton("Reset", root);
    auto *btnNextPeriod = new QPushButton("Next Period", root);
    auto *btnFetch = new QPushButton("Fetch Live", root);

    cmbMatches = new QComboBox(root);

    connect(btnGoalHome, &QPushButton::clicked, this, &ControlDock::OnGoalHome);
    connect(btnGoalAway, &QPushButton::clicked, this, &ControlDock::OnGoalAway);
    connect(btnStartTimer, &QPushButton::clicked, this, &ControlDock::OnStartTimer);
    connect(btnPauseTimer, &QPushButton::clicked, this, &ControlDock::OnPauseTimer);
    connect(btnReset, &QPushButton::clicked, this, &ControlDock::OnResetMatch);
    connect(btnNextPeriod, &QPushButton::clicked, this, &ControlDock::OnNextPeriod);
    connect(btnFetch, &QPushButton::clicked, this, &ControlDock::OnFetchLiveMatches);
    connect(cmbMatches, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &ControlDock::OnMatchSelected);

    grid->addWidget(new QLabel("Score", root), 0, 0);
    grid->addWidget(lblScore, 0, 1);
    grid->addWidget(new QLabel("Timer", root), 1, 0);
    grid->addWidget(lblTimer, 1, 1);
    grid->addWidget(new QLabel("Period", root), 2, 0);
    grid->addWidget(lblPeriod, 2, 1);

    layout->addLayout(grid);
    layout->addWidget(btnGoalHome);
    layout->addWidget(btnGoalAway);
    layout->addWidget(btnStartTimer);
    layout->addWidget(btnPauseTimer);
    layout->addWidget(btnReset);
    layout->addWidget(btnNextPeriod);
    layout->addWidget(cmbMatches);
    layout->addWidget(btnFetch);

    setWidget(root);
}

void ControlDock::RefreshTimerDisplay()
{
    lblScore->setText(QString::number(scoreboard->home.goals) + " - " +
                      QString::number(scoreboard->away.goals));
    lblTimer->setText(QString("%1:%2")
                          .arg(timer->GetMinutes(), 2, 10, QChar('0'))
                          .arg(timer->GetSeconds(), 2, 10, QChar('0')));
    lblPeriod->setText(QString::fromStdString(scoreboard->GetPeriodString()));
}

void ControlDock::OnGoalHome()
{
    scoreboard->AddGoal(0, "Manual", timer->GetMinutes());
    RefreshTimerDisplay();
}

void ControlDock::OnGoalAway()
{
    scoreboard->AddGoal(1, "Manual", timer->GetMinutes());
    RefreshTimerDisplay();
}

void ControlDock::OnYellowCardHome()
{
    scoreboard->AddCard(MatchEvent::Type::YellowCard, 0, "Manual", timer->GetMinutes());
}

void ControlDock::OnYellowCardAway()
{
    scoreboard->AddCard(MatchEvent::Type::YellowCard, 1, "Manual", timer->GetMinutes());
}

void ControlDock::OnRedCardHome()
{
    scoreboard->AddCard(MatchEvent::Type::RedCard, 0, "Manual", timer->GetMinutes());
}

void ControlDock::OnRedCardAway()
{
    scoreboard->AddCard(MatchEvent::Type::RedCard, 1, "Manual", timer->GetMinutes());
}

void ControlDock::OnStartTimer()
{
    timer->Start();
}

void ControlDock::OnPauseTimer()
{
    timer->Pause();
}

void ControlDock::OnResetMatch()
{
    timer->Reset();
    scoreboard->Reset();
    RefreshTimerDisplay();
}

void ControlDock::OnFetchLiveMatches()
{
    cmbMatches->clear();
    liveMatches.clear();

    api.FetchLiveMatches(
        [this](std::vector<LiveMatch> matches) {
            QMetaObject::invokeMethod(this, [this, matches = std::move(matches)]() mutable {
                liveMatches = std::move(matches);
                cmbMatches->clear();
                for (const auto &m : liveMatches) {
                    cmbMatches->addItem(QString::fromStdString(m.homeTeam + " vs " + m.awayTeam));
                }
            });
        },
        [this](std::string) {
            QMetaObject::invokeMethod(this, [this]() {
                cmbMatches->clear();
                cmbMatches->addItem("API error");
            });
        });
}

void ControlDock::OnMatchSelected(int index)
{
    if (index < 0 || static_cast<size_t>(index) >= liveMatches.size())
        return;

    api.ApplyToScoreboard(liveMatches[static_cast<size_t>(index)], *scoreboard);
    timer->SetTime(liveMatches[static_cast<size_t>(index)].minute, 0);
    RefreshTimerDisplay();
}

void ControlDock::OnTimerTick()
{
    timer->Tick(1.0f);
    RefreshTimerDisplay();
}

void ControlDock::OnNextPeriod()
{
    scoreboard->period++;
    RefreshTimerDisplay();
}

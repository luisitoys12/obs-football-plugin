#pragma once
#include <QDockWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTimer>
#include <vector>
#include "../scoreboard.hpp"
#include "../timer.hpp"
#include "../api-football.hpp"

class ControlDock : public QDockWidget {
    Q_OBJECT

public:
    static void AddToOBS();
    explicit ControlDock(QWidget *parent = nullptr);
    ~ControlDock() override;

private slots:
    void OnGoalHome();
    void OnGoalAway();
    void OnYellowCardHome();
    void OnYellowCardAway();
    void OnRedCardHome();
    void OnRedCardAway();
    void OnStartTimer();
    void OnPauseTimer();
    void OnResetMatch();
    void OnFetchLiveMatches();
    void OnMatchSelected(int index);
    void OnTimerTick();
    void OnNextPeriod();
    void OnAddSubstitution();
    void OnShowSubtitle();

private:
    void SetupUI();
    void RefreshTimerDisplay();

    Scoreboard   *scoreboard = nullptr;
    MatchTimer   *timer      = nullptr;
    APIFootball   api;

    QLabel       *lblScore;
    QLabel       *lblTimer;
    QLabel       *lblPeriod;
    QPushButton  *btnGoalHome;
    QPushButton  *btnGoalAway;
    QPushButton  *btnStartTimer;
    QPushButton  *btnPauseTimer;
    QPushButton  *btnAddSubstitution;
    QPushButton  *btnShowSubtitle;
    QComboBox    *cmbMatches;
    QLineEdit    *txtSubIn;
    QLineEdit    *txtSubOut;
    QLineEdit    *txtSubtitle;
    QTimer       *qtTimer;

    std::vector<LiveMatch> liveMatches;
};

#include "PhasesControl.hpp"


void PhaseHandler::Init()
{
    for (int i = 0; i < 3; i++)
    {
        sequence_[i]->Start();
    }
}

void PhaseHandler::Start()
{
    sequence_[index_]->Enable();
    sequence_[index_]->SetDuty(duty_);
}


void PhaseHandler::SetDutyStep(float duty)
{
    duty_ = duty;
    Start();
}


void PhaseHandler::SwitchStep()
{
    index_ = (index_ + 1) % 3;
    DisableAll();
    Start();
}

void PhaseHandler::Reset()
{
    index_ = 0;
    DisableAll();
}

void PhaseHandler::DisableAll()
{
    for (int i = 0; i < 3; i++)
    {
        sequence_[i]->Disable();
    }
}

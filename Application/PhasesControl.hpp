#ifndef PHASES_CONTROLLER
#define PHASES_CONTROLLER
#include "stm32f3xx_hal.h"
#include "main.h"

class Phase
{
public:
    Phase(uint16_t channel): channel_(channel)
    {}

    void Start()
    {
        //Starts the PWM signal generation
        if (HAL_TIM_PWM_Start(getTIM1(), channel_) != HAL_OK)
        {
            /* PWM Generation Error */
            Error_Handler();
        }
    }

    void Stop()
    {
        //Starts the PWM signal generation
        if (HAL_TIM_PWM_Stop(getTIM1(), channel_) != HAL_OK)
        {
            /* PWM Generation Error */
            Error_Handler();
        }
    }

    void SetDuty(float duty)
    {
        duty_ = duty;
        if ( enable_)
        {
            ApplyDuty();
        }
    }

    void Enable() { enable_ = true; };
    void Disable() { enable_ = false;  ApplyDuty(); };

    bool IsEnable() { return enable_; }
    float GetDuty() { return duty_; }

private:
    void ApplyDuty()
    {
        uint32_t pulse_width = 0;

        if ( enable_ )
        {
            pulse_width = (uint32_t)(1440 * duty_ / 100);
        }

        switch (channel_)
        {
        case TIM_CHANNEL_1:
            TIM1->CCR1 = pulse_width;
            TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
            break;
        case TIM_CHANNEL_2:
            TIM1->CCR2 = pulse_width;
            TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
            break;
        case TIM_CHANNEL_3:
            TIM1->CCR3 = pulse_width;
            TIM1->CCMR2 |= TIM_CCMR2_OC3PE;
            break;
        case TIM_CHANNEL_4:
            TIM1->CCR4 = pulse_width;
            TIM1->CCMR2 |= TIM_CCMR2_OC4PE;
            break;
        }
    }

    bool enable_ = false;
    uint32_t channel_;
    float duty_;
};


class PhaseHandler
{
public:
    PhaseHandler():
        phaseA_(TIM_CHANNEL_1),
        phaseB_(TIM_CHANNEL_2),
        phaseC_(TIM_CHANNEL_3)
    {
        Reset();
        Init();
    }

    void Init();
    void Start();
    void SwitchStep();
    void SetDutyStep(float duty);
    void Reset();

private:
    void DisableAll();

    Phase phaseA_;
    Phase phaseB_;
    Phase phaseC_;

    Phase* sequence_[3] {&phaseA_, &phaseB_, &phaseC_};
    uint32_t index_ {0};
    float duty_{0.0f};
};

#endif

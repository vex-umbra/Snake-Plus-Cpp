#pragma once
#include <cstdint>

class FixedTimer {
public:
    explicit FixedTimer(uint32_t step_ms);

    void setStep(uint32_t step_ms);
    bool shouldStep(uint32_t now_ms);

private:
    uint32_t step_ = 100;
    uint32_t last_ = 0;
};

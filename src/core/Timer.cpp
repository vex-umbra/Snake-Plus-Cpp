#include "core/Timer.h"

FixedTimer::FixedTimer(uint32_t step_ms) : step_(step_ms) {}

void FixedTimer::setStep(uint32_t step_ms) { step_ = step_ms; }

bool FixedTimer::shouldStep(uint32_t now_ms) {
    if (now_ms - last_ >= step_) {
        last_ = now_ms;
        return true;
    }
    return false;
}

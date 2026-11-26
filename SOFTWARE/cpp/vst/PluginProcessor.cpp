#include "PluginProcessor.hpp"
#include <cmath>

namespace wbs::m406 {

M406PluginProcessor::M406PluginProcessor()
    : engine::SharedPluginFrame("Ward-Beck M406 PWM Compressor", "M406") {
    defaultOversampling_ = engine::OversamplingRate::EightX; // 8x oversampling for 100 kHz carrier accuracy
}

void M406PluginProcessor::prepareModule(double hostRate, double oversampledRate, size_t maxBlockSize, size_t numChannels) {
    for (size_t ch = 0; ch < numChannels; ++ch) {
        outputStage_[ch].prepare(hostRate);
        pwmCore_[ch].prepare(oversampledRate);
    }
    updateParametersFromHost();
}

void M406PluginProcessor::releaseModule() {
    for (size_t ch = 0; ch < numChannels_; ++ch) {
        outputStage_[ch].reset();
    }
}

void M406PluginProcessor::updateParametersFromHost() noexcept {
    const float attackSec = attackMs_ * 0.001f;
    const float releaseSec = releaseMs_ * 0.001f;

    for (size_t ch = 0; ch < numChannels_; ++ch) {
        pwmCore_[ch].setParameters(thresholdDb_, ratio_, attackSec, releaseSec, pwmMode_);
    }
}

void M406PluginProcessor::processModuleSlice(engine::AudioBlock& slice) noexcept {
    if (bypass_) return;

    for (size_t ch = 0; ch < slice.numChannels; ++ch) {
        float* channelData = slice.getChannelPointer(ch);

        for (size_t i = 0; i < slice.numSamples; ++i) {
            float s = channelData[i];

            // 1. Rodger Beck PWM Chopper / Shunt Modulation Core
            float currentGrDb = 0.0f;
            s = pwmCore_[ch].processSample(s, currentGrDb);
            grMeter_[ch] = currentGrDb;

            // 2. Make-up gain
            s *= makeupGainLinear_;

            // 3. Discrete line-amp saturation
            s = outputStage_[ch].processSample(s, 1.0f, 0.15f);

            channelData[i] = s;
        }
    }
}

void M406PluginProcessor::updateMeters(const engine::AudioBlock& audioBlock) noexcept {
    // Meters are updated per-sample in processModuleSlice
}

void M406PluginProcessor::setThresholdDb(float threshDb) noexcept {
    thresholdDb_ = std::clamp(threshDb, -40.0f, 10.0f);
}

void M406PluginProcessor::setRatio(float ratio) noexcept {
    ratio_ = (ratio >= 2.5f) ? 3.0f : 2.0f;
}

void M406PluginProcessor::setAttackMs(float attackMs) noexcept {
    attackMs_ = std::clamp(attackMs, 0.2f, 50.0f);
}

void M406PluginProcessor::setReleaseMs(float releaseMs) noexcept {
    releaseMs_ = std::clamp(releaseMs, 50.0f, 2000.0f);
}

void M406PluginProcessor::setMakeupGainDb(float makeupDb) noexcept {
    makeupGainLinear_ = std::pow(10.0f, std::clamp(makeupDb, 0.0f, 24.0f) / 20.0f);
}

void M406PluginProcessor::setPwmMode(engine::PwmEngineMode mode) noexcept {
    pwmMode_ = mode;
}

void M406PluginProcessor::setBypass(bool bypassed) noexcept {
    bypass_ = bypassed;
}

float M406PluginProcessor::getGainReductionMeter(size_t ch) const noexcept {
    return (ch < engine::kMaxChannels) ? grMeter_[ch] : 0.0f;
}

} // namespace wbs::m406

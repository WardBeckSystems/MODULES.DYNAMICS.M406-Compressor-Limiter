#pragma once
#include "../core/SharedPluginFrame.hpp"
#include "../core/PwmChopperCore.hpp"
#include "../core/NonLinearCore.hpp"

namespace wbs::m406 {

class M406PluginProcessor : public engine::SharedPluginFrame {
public:
    M406PluginProcessor();
    ~M406PluginProcessor() override = default;

    void prepareModule(double hostRate, double oversampledRate, size_t maxBlockSize, size_t numChannels) override;
    void releaseModule() override;
    void updateParametersFromHost() noexcept override;
    void processModuleSlice(engine::AudioBlock& slice) noexcept override;
    void updateMeters(const engine::AudioBlock& audioBlock) noexcept override;

    // Parameter setters
    void setThresholdDb(float threshDb) noexcept;
    void setRatio(float ratio) noexcept; // 2.0f or 3.0f
    void setAttackMs(float attackMs) noexcept;
    void setReleaseMs(float releaseMs) noexcept;
    void setMakeupGainDb(float makeupDb) noexcept;
    void setPwmMode(engine::PwmEngineMode mode) noexcept;
    void setBypass(bool bypassed) noexcept;

    [[nodiscard]] float getGainReductionMeter(size_t ch) const noexcept;

private:
    engine::PwmChopperDynamicsCore pwmCore_[engine::kMaxChannels];
    engine::NonLinearTransformerCore outputStage_[engine::kMaxChannels];

    float thresholdDb_{ -12.0f };
    float ratio_{ 2.0f };
    float attackMs_{ 5.0f };
    float releaseMs_{ 250.0f };
    float makeupGainLinear_{ 1.0f };
    engine::PwmEngineMode pwmMode_{ engine::PwmEngineMode::AuthenticPwmChopper };
    bool bypass_{ false };

    float grMeter_[engine::kMaxChannels]{ 0.0f };
};

} // namespace wbs::m406

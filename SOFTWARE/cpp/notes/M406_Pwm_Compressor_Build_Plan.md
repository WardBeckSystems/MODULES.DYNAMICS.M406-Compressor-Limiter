# Ward-Beck Systems M406 PWM Compressor - VST/AU Plugin Build Plan

**Module Name:** WBS M406 Pulse-Width Modulation Compressor  
**Hardware Reference:** Ward-Beck Systems M406 Dynamic Compressor / Limiter (ca. 1968–1976, Rodger Beck original circuit)  
**Target Architecture:** VST3 / AU / AAX (JUCE 7 / C++20)  
**Shared Engine Dependency:** [`wbs::SharedPluginFrame`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/engine/SharedPluginFrame.hpp), [`wbs::PwmChopperCore`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/engine/PwmChopperCore.hpp), [`wbs::OversamplingEngine`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/engine/OversamplingEngine.hpp), [`wbs::NonLinearCore`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/engine/NonLinearCore.hpp)

---

## 1. Hardware Architecture & Circuit Analysis

In 1968, co-founder Rodger Beck bypassed the non-linear distortion endemic to early Germanium diode bridges and FET gain cells by inventing the **Pulse-Width Modulation (PWM) Chopper Gain Controller**. 

### How Analog PWM Compression Works:
1. **100 kHz Carrier Generator:** A discrete relaxation oscillator generates a precision $100\text{ kHz}$ linear sawtooth/ramp waveform.
2. **Comparator Slicer:** Audio sidechain control voltage ($V_c$) is fed into a differential high-speed comparator along with the $100\text{ kHz}$ ramp.
3. **Duty-Cycle Chopping:** The comparator generates a rectangular pulse train whose duty cycle $D$ varies continuously between **$3\%$ and $97\%$** linearly proportional to $V_c$.
4. **Shunt Switching Transistor:** Fast bipolar switching transistors (e.g. 2N2369) chop the audio signal to ground at $100\text{ kHz}$.
5. **Reconstruction Filter:** A 5-pole active LC/op-amp low-pass reconstruction filter removes the $100\text{ kHz}$ switching harmonics, leaving behind only the original audio multiplied by the average duty cycle:
$$V_{out}(t) = \frac{1}{T_{PWM}} \int_0^{T_{PWM}} V_{switched}(t) dt \approx D \cdot V_{in}(t)$$
Because the switching transistor is either fully saturated (ON) or fully cut off (OFF), it never operates in its non-linear active region. This yields virtually zero harmonic distortion even at deep compression levels, producing the signature transparent, punchy Ward-Beck broadcast sound.

---

## 2. DSP Implementation Architecture

### 2.1 Dual-Engine Architecture: True-PWM vs. Eco-VCA
Digital simulation of a $100\text{ kHz}$ switching carrier requires special care to prevent aliasing. The plugin provides two selectable operational engines:

1. **True-PWM Oversampled Mode (Mastering / Studio Quality):**
   - Utilizes [`wbs::OversamplingEngine`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/engine/OversamplingEngine.hpp) operating at **$8\times$ oversampling** ($352.8\text{ kHz}$ or $384.0\text{ kHz}$).
   - Generates the discrete ramp oscillator and differential comparator at $8\times f_s$.
   - Executes our 4-pole active Butterworth reconstruction filter before downsampling back to baseband.
2. **Eco-VCA Mode (Low-CPU Live Tracking):**
   - Bypasses the discrete carrier generator and directly modulates the signal using the duty-cycle control law:
$$g[n] = D[n] = \text{clamp}\left(1.0 - \frac{V_c[n]}{V_{max}}, 0.03, 0.97\right)$$
   - Requires $< 0.5\%$ CPU per channel while delivering identical gain envelope dynamics.

---

## 3. Parameter Mapping & State Architecture

| ID | Parameter Name | Range | Steps / Type | Default | Unit |
|---|---|---|---|---|---|
| `m406_threshold` | Threshold | $-40.0$ to $+10.0$ | Continuous | $-10.0$ | dB |
| `m406_ratio` | Compression Ratio | 2:1, 3:1 | 2-way switch | 2:1 | Ratio |
| `m406_attack` | Attack Time | $0.2$ to $50.0$ | Continuous (Log) | 5.0 | ms |
| `m406_release` | Release Time | $50.0$ to $2000.0$ | Continuous (Log) | 250.0 | ms |
| `m406_makeup` | Make-Up Gain | $0.0$ to $+24.0$ | Continuous | 0.0 | dB |
| `m406_carrier_mode`| Chopper Engine Mode | True PWM (8x) / Eco VCA | 2-way toggle | True PWM | Enum |
| `m406_sidechain_hpf`| Sidechain Filter | Flat, 80 Hz, 160 Hz | 3 positions | 80 Hz | Enum |
| `m406_mix` | Dry / Wet Blend | $0.0$ to $100.0$ | Continuous | 100.0 | % |
| `m406_bypass` | Hardware Bypass | Active / Bypassed | Toggle | Active | Bool |

---

## 4. C++ Plugin Scaffolding

```cpp
// M406CompressorProcessor.hpp
#pragma once
#include "SharedPluginFrame.hpp"
#include "PwmChopperCore.hpp"
#include "OversamplingEngine.hpp"
#include "NonLinearCore.hpp"

namespace wbs {

class M406CompressorProcessor : public SharedPluginFrame {
public:
    M406CompressorProcessor();
    ~M406CompressorProcessor() override = default;

    const char* getPluginIdentifier() const noexcept override { return "WBS_M406_PWM_Comp"; }
    uint32_t getPluginVersion() const noexcept override { return 0x010000; }

    void prepareHardwareState(double sampleRate, uint32_t maxBlockSize) override;
    void resetHardwareState() override;
    void processAudioCore(AudioBlock<float>& block) override;

private:
    PwmChopperCore pwmCore_[2];
    OversamplingEngine oversampler_;
    NonLinearCore outputStage_;

    float envDetector_[2] = { 0.0f, 0.0f };
    float attackCoeff_ = 0.0f;
    float releaseCoeff_ = 0.0f;

    void updateEnvelopeBallistics(float attackMs, float releaseMs);
};

} // namespace wbs
```

---

## 5. UI/UX Specification

- **Chassis Design:** Heavy 2RU rackmount unit with silver faceplate and engraved black lettering.
- **Meters:**
  - Fast-response analog Gain Reduction meter with calibrated $-20\text{ dB}$ to $0\text{ dB}$ needle ballistics ($300\text{ ms}$ mechanical damping).
  - Output level VU meter.
- **Oscilloscope Monitor:** Real-time mini phosphor scope visualizing the $100\text{ kHz}$ PWM pulse modulation stream.

---

## 6. Verification & Test Plan

1. **Carrier Aliasing Check (True PWM):** Apply $10\text{ kHz}$ full-scale sine tone under $12\text{ dB}$ compression; verify carrier intermodulation alias products stay below $-90\text{ dBFS}$.
2. **Duty-Cycle Linearity:** Sweep sidechain control voltage from min to max; verify gain reduction strictly follows linear duty cycle from $0.0\text{ dB}$ to $-30.4\text{ dB}$ ($20 \log_{10}(0.03)$).
3. **Ballistics Accuracy:** Measure step-response attack time with $1\text{ kHz}$ burst; verify $90\%$ envelope acquisition matches target time $\pm5\%$.

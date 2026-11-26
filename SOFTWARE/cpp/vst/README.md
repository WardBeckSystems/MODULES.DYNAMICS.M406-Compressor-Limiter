# Ward-Beck Systems M406 Pulse-Width Modulation (PWM) Compressor VST/AU Plugin

**Hardware Unit:** Ward-Beck Systems M406 Dynamic Compressor / Limiter (ca. 1968, Rodger Beck original circuit)  
**Format:** VST3, AU, AAX (C++20 / JUCE 7)  
**Core Shared Engine:** [`wbs_core`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/vst/core/README.md)

---

## Technical Specifications
- **Operating Principle:** $100\text{ kHz}$ linear ramp oscillator compared against sidechain control voltage to produce variable duty-cycle ($3\% - 97\%$) shunt pulse chopping.
- **Ratios:** Transparent broadcast ratios $2:1$ and $3:1$.
- **Attack & Release:** $0.2\text{ ms} - 50.0\text{ ms}$ attack; $50\text{ ms} - 2.0\text{ s}$ release.
- **Dual Engine Modes:**
  - **Authentic PWM Chopper:** $8\times$ oversampled domain with 4-pole active Butterworth reconstruction filter.
  - **Eco-VCA Mode:** Continuous mathematical transfer function for zero-latency, ultra-low CPU tracking.

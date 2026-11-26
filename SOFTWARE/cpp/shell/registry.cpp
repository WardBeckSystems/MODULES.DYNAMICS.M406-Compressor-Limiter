// Copyright (c) 2026 Ward-Beck Systems (Ward-Beck.com - Ward-Beck.ca). All rights reserved.
// M406: its part of 2030.env/vst/clap_wrapper/ClapModuleRegistry.cpp (suite index 5).

// --- descriptor ---
static const clap_plugin_descriptor_t s_desc_m406 = {
    CLAP_VERSION,
    "ca.wbsps.m406_pwm_compressor",
    "Ward-Beck M406 PWM Compressor",
    "Ward-Beck Systems Preservation Society",
    "https://wbsps.ca",
    "",
    "https://wbsps.ca/contact",
    "1.0.0",
    "Legendary Rodger Beck 100 kHz pulse-width modulation chopper optical-smooth dynamics compressor",
    kCompressorFeatures
};

// --- parameters ---
std::vector<ParameterInfo> buildM406Params() {
    std::vector<ParameterInfo> p;
    p.push_back({
        0, "Threshold", "Dynamics", -40.0, 10.0, -12.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m406::M406PluginProcessor*>(f)->setThresholdDb(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%+.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        1, "Ratio", "Dynamics", 1.5, 5.0, 2.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m406::M406PluginProcessor*>(f)->setRatio(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.1f:1", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        2, "Attack Time", "Dynamics", 0.2, 50.0, 5.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m406::M406PluginProcessor*>(f)->setAttackMs(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.1f ms", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        3, "Release Time", "Dynamics", 50.0, 2000.0, 250.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m406::M406PluginProcessor*>(f)->setReleaseMs(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.0f ms", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        4, "Makeup Gain", "Output", 0.0, 24.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m406::M406PluginProcessor*>(f)->setMakeupGainDb(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "+%.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        5, "PWM Engine Mode", "Chopper", 0.0, 1.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) {
            static_cast<m406::M406PluginProcessor*>(f)->setPwmMode(val >= 0.5 ? engine::PwmEngineMode::EcoVcaPristine : engine::PwmEngineMode::AuthenticPwmChopper);
        },
        nullptr,
        [](double v) { return v >= 0.5 ? "Eco VCA" : "Authentic PWM Chopper (100kHz)"; },
        nullptr
    });
    p.push_back({
        6, "Bypass", "Master", 0.0, 1.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_BYPASS,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m406::M406PluginProcessor*>(f)->setBypass(val >= 0.5); },
        nullptr,
        [](double v) { return v >= 0.5 ? "Bypassed" : "Active"; },
        nullptr
    });
    return p;
}

// --- instance ---
        case 5:
            inst = new ClapPluginInstance(
                &s_desc_m406, host,
                []() { return std::make_unique<m406::M406PluginProcessor>(); },
                buildM406Params(),
                5
            );
            break;


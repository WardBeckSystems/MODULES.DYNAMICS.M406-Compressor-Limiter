// M406: its block of 2030.env/vst/core/ModuleGuiLayouts.hpp (176 x 770 = the faceplate).

        case 5: { // M406 PWM Compressor (176 x 770, Rodger Beck Chopper PWM Dynamics)
            m.title = "M406 PWM Compressor";
            m.subtitle = "Chopper-Modulated PWM Dynamics (3 Concentric Control Groups)";
            m.modelCode = "M406";
            m.hasEdgewiseMeters = false;

            // Group 1: Output Gain (Makeup, outer winged) & Threshold (inner fluted)
            m.knobs.push_back({ 4, "OUTPUT", "dB", 88.0f, 130.0f, 26.0f, 0.0, 24.0, 0.0, false, KnobStyle::MarconiWingedFluted, Palette::PureBlackBakelite });
            m.knobs.push_back({ 0, "THRESHOLD", "dB", 88.0f, 130.0f, 18.0f, -40.0, 10.0, -12.0, false, KnobStyle::MarconiInnerFluted, Palette::PureBlackBakelite });

            // Group 2: Ratio (outer winged)
            m.knobs.push_back({ 1, "RATIO", ":1", 88.0f, 280.0f, 26.0f, 1.5, 5.0, 2.0, true, KnobStyle::MarconiWingedFluted, Palette::PureBlackBakelite });

            // Group 3: Recovery / Release (outer winged) & Attack (inner fluted)
            m.knobs.push_back({ 3, "RECOVERY", "ms", 88.0f, 430.0f, 26.0f, 50.0, 2000.0, 250.0, false, KnobStyle::MarconiWingedFluted, Palette::PureBlackBakelite });
            m.knobs.push_back({ 2, "ATTACK", "ms", 88.0f, 430.0f, 18.0f, 0.2, 50.0, 5.0, false, KnobStyle::MarconiInnerFluted, Palette::PureBlackBakelite });

            m.buttons.push_back({ 5, "PWM MODE", { 68.0f, 540.0f, 40.0f, 28.0f }, false, ButtonStyle::ToggleSwitch, Palette::Chrome });
            m.buttons.push_back({ 6, "COMP IN", { 68.0f, 610.0f, 40.0f, 40.0f }, true, ButtonStyle::MomentaryPush, Palette::IvoryCream });

            m.themeToggleRect = { 25.0f, 726.0f, 126.0f, 22.0f };
            break;
        }

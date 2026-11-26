// M406: its block of 2030.env/vst/core/CircuitDiagramDefinitions.hpp.

        case 5: // M406 PWM Compressor
            d.modelCode = "M406";
            d.circuitTitle = "Ward-Beck M406 100 kHz PWM Chopper Dynamics Circuit";
            d.theoreticalExplanation =
                "Rodger Beck's revolutionary 1968 Pulse-Width Modulation audio compressor. Audio is chopped at 100 kHz by a high-speed "
                "linear comparator. Modulating the duty-cycle shunts signal to ground, delivering distortion-free, optical-smooth compression.";
            d.nodes = {
                { "IN",     "Line In",       "Transformer In", { 15, 20, 85, 48 }, true, Palette::CopperWire },
                { "SIDE",   "Detector",      "Attack/Release", { 115, 52, 100, 36 }, true, Palette::CircuitBorder },
                { "SAW",    "100kHz Saw",    "Ramp Gen",       { 115, 12, 100, 36 }, true, Palette::TextGold },
                { "CHOP",   "PWM Chopper",   "JFET Modulator", { 230, 20, 110, 48 }, true, Palette::CircuitHighlight },
                { "RECON",  "LC Low-Pass",   "100kHz Sieve",   { 355, 20, 105, 48 }, true, Palette::CircuitBorder },
                { "MAKEUP", "Class-A Makeup","+24 dB Gain",    { 475, 20, 95, 48 }, true, Palette::LedGreen }
            };
            d.wires = {
                { "IN", "CHOP", "Audio Stream" },
                { "IN", "SIDE", "Sidechain Pickoff" },
                { "SIDE", "CHOP", "Modulation Voltage" },
                { "SAW", "CHOP", "100 kHz Carrier" },
                { "CHOP", "RECON", "Chopped Audio" },
                { "RECON", "MAKEUP", "Demodulated Audio" }
            };
            break;

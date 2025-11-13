#include "godot_waveform.h"

void GodotWaveform::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &GodotWaveform::print_type);
}

void GodotWaveform::print_type(const Variant &p_variant) const {
	print_line(vformat("Type: %d", p_variant.get_type()));
}


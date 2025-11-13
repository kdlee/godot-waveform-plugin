#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

class GodotWaveform : public RefCounted {
	GDCLASS(GodotWaveform, RefCounted)

protected:
	static void _bind_methods();

public:
	GodotWaveform() = default;
	~GodotWaveform() override = default;

	void print_type(const Variant &p_variant) const;
};


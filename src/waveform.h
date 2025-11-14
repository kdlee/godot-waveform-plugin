#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

class Waveform : public Node {
	GDCLASS(Waveform, Node)

protected:
	static void _bind_methods();

public:
	Waveform() = default;
	~Waveform() override = default;

	static Waveform *get_singleton();
	static Waveform *singleton;

	PackedVector2Array generate(const Ref<AudioStream> &p_stream, float p_sampling_frequency) const;
};


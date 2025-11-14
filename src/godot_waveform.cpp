#include "godot_waveform.h"

#include <godot_cpp/core/class_db.hpp>

GodotWaveform *GodotWaveform::singleton = nullptr;

void GodotWaveform::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("generate", "stream", "sampling_frequency"), &GodotWaveform::generate);
}

GodotWaveform *GodotWaveform::get_singleton() {
	return singleton;
}

PackedVector2Array GodotWaveform::generate(const Ref<AudioStream> &p_stream, float p_sampling_frequency) const {
	PackedVector2Array min_max_pairs;

	if (p_stream.is_null()) {
		return min_max_pairs;
	}
	if (p_sampling_frequency <= 0.0f) {
		return min_max_pairs;
	}

	Ref<AudioStreamPlayback> playback = p_stream->instantiate_playback();
	if (playback.is_null()) {
		return min_max_pairs;
	}

	playback->start(0.0);

	const AudioServer *audio_server = AudioServer::get_singleton();
	const float mix_rate_hz = audio_server ? audio_server->get_mix_rate() : 48000.0f;
	int64_t frames_per_window = (int64_t)(mix_rate_hz / p_sampling_frequency + 0.5f);
	if (frames_per_window < 1) {
		frames_per_window = 1;
	}

	while (true) {
		const PackedVector2Array frames = playback->mix_audio(1.0f, (int32_t)frames_per_window);
		const int64_t num_frames = frames.size();
		if (num_frames <= 0) {
			break;
		}

		float window_min = 1.0f;
		float window_max = -1.0f;

		const Vector2 *frame_ptr = frames.ptr();
		for (int64_t i = 0; i < num_frames; i++) {
			const Vector2 &stereo = frame_ptr[i];
			if (stereo.x < window_min) {
				window_min = stereo.x;
			}
			if (stereo.y < window_min) {
				window_min = stereo.y;
			}
			if (stereo.x > window_max) {
				window_max = stereo.x;
			}
			if (stereo.y > window_max) {
				window_max = stereo.y;
			}
		}

		min_max_pairs.push_back(Vector2(window_min, window_max));

		if (num_frames < frames_per_window) {
			break;
		}
	}

	playback->stop();
	return min_max_pairs;
}


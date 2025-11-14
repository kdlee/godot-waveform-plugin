#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "godot_waveform.h"

using namespace godot;

static GodotWaveform *godot_waveform_singleton = nullptr;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(GodotWaveform);
	// Create and register singleton instance.
	godot_waveform_singleton = memnew(GodotWaveform);
	GodotWaveform::singleton = godot_waveform_singleton;
	ClassDB::_register_engine_singleton(GodotWaveform::get_class_static(), godot_waveform_singleton);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	// Unregister and free singleton instance.
	if (godot_waveform_singleton) {
		ClassDB::_unregister_engine_singleton(GodotWaveform::get_class_static());
		memdelete(godot_waveform_singleton);
		godot_waveform_singleton = nullptr;
		GodotWaveform::singleton = nullptr;
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT godot_waveform_plugin_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
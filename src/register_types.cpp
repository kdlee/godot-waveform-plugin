#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "waveform.h"

using namespace godot;

static Waveform *waveform_singleton = nullptr;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(Waveform);
	// Create and register singleton instance.
	waveform_singleton = memnew(Waveform);
	Waveform::singleton = waveform_singleton;
	ClassDB::_register_engine_singleton(Waveform::get_class_static(), waveform_singleton);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	// Unregister and free singleton instance.
	if (waveform_singleton) {
		ClassDB::_unregister_engine_singleton(Waveform::get_class_static());
		memdelete(waveform_singleton);
		waveform_singleton = nullptr;
		Waveform::singleton = nullptr;
	}
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT waveform_plugin_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
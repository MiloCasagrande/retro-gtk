/* Retro  GObject libretro wrapper.
 * Copyright (C) 2014  Adrien Plazas
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include "retro-core-interfaces.h"

#include "retro-core-global.h"

gboolean retro_core_set_callback_interfaces (RetroCore *self, RetroEnvironmentCommand cmd, gpointer data) {
	switch (cmd) {
	case RETRO_ENVIRONMENT_COMMAND_GET_RUMBLE_INTERFACE:
		return retro_core_set_rumble_callback (self, (RetroRumbleCallback *) data);
	case RETRO_ENVIRONMENT_COMMAND_GET_SENSOR_INTERFACE:
		return retro_core_set_sensor_callback (self, (RetroSensorCallback *) data);
	case RETRO_ENVIRONMENT_COMMAND_GET_CAMERA_INTERFACE:
		return retro_core_set_camera_callback (self, (RetroCameraCallback *) data);
	case RETRO_ENVIRONMENT_COMMAND_GET_LOG_INTERFACE:
		return retro_core_set_log_callback (self, (RetroLogCallback *) data);
	case RETRO_ENVIRONMENT_COMMAND_GET_PERF_INTERFACE:
		return retro_core_set_performance_callback (self, (RetroPerformanceCallback *) data);
	case RETRO_ENVIRONMENT_COMMAND_GET_LOCATION_INTERFACE:
		return retro_core_set_location_callback (self, (RetroLocationCallback *) data);
	default:
		return FALSE;
	}
}

gboolean retro_core_set_rumble_callback (RetroCore *self, RetroRumbleCallback *cb) {
	RetroCore *global_self = retro_core_get_global_self ();
	gboolean interface_exists = global_self && retro_core_get_rumble_interface (global_self);
	if (!interface_exists) return FALSE;
	
	gboolean real_set_rumble_state (guint port, RetroRumbleEffect effect, guint16 strength) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroRumble *interface = retro_core_get_rumble_interface (global_self);
			return RETRO_RUMBLE_GET_INTERFACE (interface)->set_rumble_state (interface, port, effect, strength);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	cb->set_rumble_state = real_set_rumble_state;
	
	return TRUE;
}

gboolean retro_core_set_sensor_callback (RetroCore *self, RetroSensorCallback *cb) {
	RetroCore *global_self = retro_core_get_global_self ();
	gboolean interface_exists = global_self && retro_core_get_sensor_interface (global_self);
	if (!interface_exists) return FALSE;
	
	gboolean real_set_sensor_state (guint port, RetroSensorAction action, guint rate) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroSensor *interface = retro_core_get_sensor_interface (global_self);
			return RETRO_SENSOR_GET_INTERFACE (interface)->set_sensor_state (interface, port, action, rate);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	gfloat real_get_sensor_input (guint port, guint id) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroSensor *interface = retro_core_get_sensor_interface (global_self);
			return RETRO_SENSOR_GET_INTERFACE (interface)->get_sensor_input (interface, port, id);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	cb->set_sensor_state = real_set_sensor_state;
	cb->get_sensor_input = real_get_sensor_input;
	
	return TRUE;
}

gboolean retro_core_set_camera_callback (RetroCore *self, RetroCameraCallback *cb) {
	RetroCore *global_self = retro_core_get_global_self ();
	gboolean interface_exists = global_self && retro_core_get_camera_interface (global_self);
	if (!interface_exists) return FALSE;
	
	gboolean real_start () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroCamera *interface = retro_core_get_camera_interface (global_self);
			return RETRO_CAMERA_GET_INTERFACE (interface)->start (interface);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	void real_stop () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroCamera *interface = retro_core_get_camera_interface (global_self);
			RETRO_CAMERA_GET_INTERFACE (interface)->stop (interface);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	gfloat real_frame_raw_framebuffer (guint32 *buffer, guint width, guint height, gsize pitch) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroCamera *interface = retro_core_get_camera_interface (global_self);
			RETRO_CAMERA_GET_INTERFACE (interface)->frame_raw_framebuffer (interface, buffer, width, height, pitch);
			return;
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	gfloat real_frame_opengl_texture (guint texture_id, guint texture_target, gfloat *affine) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroCamera *interface = retro_core_get_camera_interface (global_self);
			RETRO_CAMERA_GET_INTERFACE (interface)->frame_opengl_texture (interface, texture_id, texture_target, affine);
			return;
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	void real_initialized () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroCamera *interface = retro_core_get_camera_interface (global_self);
			RETRO_CAMERA_GET_INTERFACE (interface)->initialized (interface);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	void real_deinitialized () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroCamera *interface = retro_core_get_camera_interface (global_self);
			RETRO_CAMERA_GET_INTERFACE (interface)->deinitialized (interface);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	RetroCamera *interface = retro_core_get_camera_interface (global_self);
	
	cb->caps = RETRO_CAMERA_GET_INTERFACE (interface)->get_caps (interface);
	cb->width = RETRO_CAMERA_GET_INTERFACE (interface)->get_width (interface);
	cb->height = RETRO_CAMERA_GET_INTERFACE (interface)->get_height (interface);
	cb->start = real_start;
	cb->stop = real_stop;
	cb->frame_raw_framebuffer = real_frame_raw_framebuffer;
	cb->frame_opengl_texture = real_frame_opengl_texture;
	cb->initialized = real_initialized;
	cb->deinitialized = real_deinitialized;
	
	return TRUE;
}

gboolean retro_core_set_log_callback (RetroCore *self, RetroLogCallback *cb) {
	RetroCore *global_self = retro_core_get_global_self ();
	gboolean interface_exists = global_self && retro_core_get_log_interface (global_self);
	if (!interface_exists) return FALSE;
	
	gboolean real_log (guint level, const char *format, ...) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroLog *interface = retro_core_get_log_interface (global_self);
			
			// Get the arguments, set up the formatted message,
			// pass it to the logging method and free it.
			va_list args;
			va_start (args, format);
			char *message = g_strdup_vprintf (format, args);
			gboolean result = RETRO_LOG_GET_INTERFACE (interface)->log (interface, level, message);
			g_free (message);
			return result;
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	cb->log = real_log;
	
	return TRUE;
}

gboolean retro_core_set_performance_callback (RetroCore *self, RetroPerformanceCallback *cb) {
	RetroCore *global_self = retro_core_get_global_self ();
	gboolean interface_exists = global_self && retro_core_get_performance_interface (global_self);
	if (!interface_exists) return FALSE;
	
	gint64 real_get_time_usec () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroPerformance *interface = retro_core_get_performance_interface (global_self);
			return RETRO_PERFORMANCE_GET_INTERFACE (interface)->get_time_usec (interface);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	guint64 real_get_cpu_features () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroPerformance *interface = retro_core_get_performance_interface (global_self);
			return RETRO_PERFORMANCE_GET_INTERFACE (interface)->get_cpu_features (interface);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	guint64 real_get_perf_counter () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroPerformance *interface = retro_core_get_performance_interface (global_self);
			return RETRO_PERFORMANCE_GET_INTERFACE (interface)->get_perf_counter (interface);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	void real_perf_register (RetroPerformanceCounter *counter) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroPerformance *interface = retro_core_get_performance_interface (global_self);
			RETRO_PERFORMANCE_GET_INTERFACE (interface)->perf_register (interface, counter);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	void real_perf_start (RetroPerformanceCounter *counter) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroPerformance *interface = retro_core_get_performance_interface (global_self);
			RETRO_PERFORMANCE_GET_INTERFACE (interface)->perf_start (interface, counter);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	void real_perf_stop (RetroPerformanceCounter *counter) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroPerformance *interface = retro_core_get_performance_interface (global_self);
			RETRO_PERFORMANCE_GET_INTERFACE (interface)->perf_stop (interface, counter);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	void real_perf_log () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroPerformance *interface = retro_core_get_performance_interface (global_self);
			RETRO_PERFORMANCE_GET_INTERFACE (interface)->perf_log (interface);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	cb->get_time_usec = real_get_time_usec;
	cb->get_cpu_features = real_get_cpu_features;
	cb->get_perf_counter = real_get_perf_counter;
	cb->perf_register = real_perf_register;
	cb->perf_start = real_perf_start;
	cb->perf_stop = real_perf_stop;
	cb->perf_log = real_perf_log;
	
	return TRUE;
}

gboolean retro_core_set_location_callback (RetroCore *self, RetroLocationCallback *cb) {
	RetroCore *global_self = retro_core_get_global_self ();
	gboolean interface_exists = global_self && retro_core_get_location_interface (global_self);
	if (!interface_exists) return FALSE;
	
	gboolean real_start () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroLocation *interface = retro_core_get_location_interface (global_self);
			return RETRO_LOCATION_GET_INTERFACE (interface)->start (interface);
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	void real_stop () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroLocation *interface = retro_core_get_location_interface (global_self);
			RETRO_LOCATION_GET_INTERFACE (interface)->stop (interface);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	gfloat real_get_position (gdouble *lat, gdouble *lon, gdouble *horiz_accuracy, gdouble *vert_accuracy) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroLocation *interface = retro_core_get_location_interface (global_self);
			RETRO_LOCATION_GET_INTERFACE (interface)->get_position (interface, lat, lon, horiz_accuracy, vert_accuracy);
			return;
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	gfloat real_set_interval (guint interval_ms, guint interval_distance) {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroLocation *interface = retro_core_get_location_interface (global_self);
			RETRO_LOCATION_GET_INTERFACE (interface)->set_interval (interface, interval_ms, interval_distance);
			return;
		}
		
		g_assert_not_reached ();
		return 0;
	}
	
	void real_initialized () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroLocation *interface = retro_core_get_location_interface (global_self);
			RETRO_LOCATION_GET_INTERFACE (interface)->initialized (interface);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	void real_deinitialized () {
		RetroCore *global_self = retro_core_get_global_self ();
		if (global_self) {
			RetroLocation *interface = retro_core_get_location_interface (global_self);
			RETRO_LOCATION_GET_INTERFACE (interface)->deinitialized (interface);
			return;
		}
		
		g_assert_not_reached ();
	}
	
	cb->start = real_start;
	cb->stop = real_stop;
	cb->get_position = real_get_position;
	cb->set_interval = real_set_interval;
	cb->initialized = real_initialized;
	cb->deinitialized = real_deinitialized;
	
	return TRUE;
}

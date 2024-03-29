// This file is part of RetroGtk. License: GPLv3

using Clutter;

using Retro;

namespace RetroGtk {

[Deprecated (since = "0.2")]
public class ClutterDisplay : GtkClutter.Embed, Video, Display {
	private Core _core;
	public Core core {
		get { return _core; }
		set {
			if (_core == value) return;

			_core = value;
			pixel_format = PixelFormat.ORGB1555;

			if (_core != null && _core.video_interface != this)
				_core.video_interface = this;
		}
	}

	private Texture texture;

	public Rotation rotation { get; set; default = Rotation.NONE; }
	public bool overscan { get; set; default = false; }
	public bool can_dupe { get; set; default = false; }
	public Retro.PixelFormat pixel_format { get; set; default = PixelFormat.ORGB1555; }

	construct {
		var stage = get_stage ();

		stage.set_background_color (Color.get_static (StaticColor.BLACK));

		texture = new Texture ();
		hide_video ();
		stage.add_actor (texture);

		size_allocate.connect (on_size_allocate);
	}

	[CCode (cname = "video_to_pixbuf", cheader_filename="video-converter.h")]
	static extern Gdk.Pixbuf video_to_pixbuf ([CCode (array_length = false)] uint8[] data, uint width, uint height, size_t pitch, Retro.PixelFormat pixel_format);

	public void render (uint8[] data, uint width, uint height, size_t pitch) {
		if (data == null) return; // Dupe a frame

		Cogl.Texture cogl_tex;

		switch (pixel_format) {
			case PixelFormat.ORGB1555:
				var pb = video_to_pixbuf (data, width, height, pitch, pixel_format);

				Cogl.TextureFlags flags = Cogl.TextureFlags.NO_AUTO_MIPMAP | Cogl.TextureFlags.NO_SLICING | Cogl.TextureFlags.NO_ATLAS;

				cogl_tex = new Cogl.Texture.from_data (
					pb.width, pb.height, flags,
					Cogl.PixelFormat.RGB_888,
					Cogl.PixelFormat.RGB_888,
					pb.rowstride,
					(uchar[]) pb.get_pixels_with_length ()
				);
				break;

			case PixelFormat.XRGB8888:
				Cogl.TextureFlags flags = Cogl.TextureFlags.NO_AUTO_MIPMAP | Cogl.TextureFlags.NO_SLICING | Cogl.TextureFlags.NO_ATLAS;

				cogl_tex = new Cogl.Texture.from_data (
					width, height, flags,
					Cogl.PixelFormat.BGRA_8888,
					Cogl.PixelFormat.RGB_888,
					(uint) pitch, (uchar[]) data
				);
				break;

			case PixelFormat.RGB565:
				Cogl.TextureFlags flags = Cogl.TextureFlags.NO_AUTO_MIPMAP | Cogl.TextureFlags.NO_SLICING | Cogl.TextureFlags.NO_ATLAS;

				cogl_tex = new Cogl.Texture.from_data (
					width, height, flags,
					Cogl.PixelFormat.RGB_565,
					Cogl.PixelFormat.RGB_888,
					(uint) pitch, (uchar[]) data
				);
				break;

			default:
				return;
		}

		texture.set_cogl_texture (cogl_tex);
	}

	private void on_size_allocate (Gtk.Allocation allocation) {
		double display_ratio = (double) core.av_info.aspect_ratio;
		double allocated_ratio = (double) allocation.width / allocation.height;

		bool screen_is_wider = allocated_ratio > display_ratio;

		// Set the size of the display

		float w = 0;
		float h = 0;

		if (screen_is_wider) {
			h = (float) allocation.height;
			w = (float) (h * display_ratio);
		}
		else {
			w = (float) allocation.width;
			h = (float) (w / display_ratio);
		}

		texture.set_size (w, h);

		// Set the position of the display

		float x = 0;
		float y = 0;

		x = (allocation.width - w) / 2;
		y = (allocation.height - h) / 2;

		texture.set_position (x, y);
	}

	public void show_video () {
		texture.visible = true;
	}

	public void hide_video () {
		texture.visible = false;
	}
}

}


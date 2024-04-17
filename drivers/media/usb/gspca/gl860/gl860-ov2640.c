// SPDX-License-Identifier: GPL-2.0-or-later
/* Subdriver for the GL860 chip with the OV2640 sensor
 * Author Olivier LORIN, from Malmostoso's logs
 */

/* Sensor : OV2640 */

#include "gl860.h"

static u8 dat_init1[] = "\x00\x41\x07\x6a\x06\x61\x0d\x6a" "\x10\x10\xc1\x01";

static u8 c61[] = {0x61}; /* expected */
static u8 c51[] = {0x51}; /* expected */
static u8 c50[] = {0x50}; /* expected */
static u8 c28[] = {0x28}; /* expected */
static u8 ca8[] = {0xa8}; /* expected */

static u8 dat_post[] =
	"\x00\x41\x07\x6a\x06\xef\x0d\x6a" "\x10\x10\xc1\x01";

static u8 dat_640[]  = "\xd0\x01\xd1\x08\xd2\xe0\xd3\x02\xd4\x10\xd5\x81";
static u8 dat_800[]  = "\xd0\x01\xd1\x10\xd2\x58\xd3\x02\xd4\x18\xd5\x21";
static u8 dat_1280[] = "\xd0\x01\xd1\x18\xd2\xc0\xd3\x02\xd4\x28\xd5\x01";
static u8 dat_1600[] = "\xd0\x01\xd1\x20\xd2\xb0\xd3\x02\xd4\x30\xd5\x41";

static struct validx tbl_init_at_startup[] = {
	{0x0000, 0x0000}, {0x0010, 0x0010}, {0x0008, 0x00c0}, {0x0001, 0x00c1},
	{0x0001, 0x00c2}, {0x0020, 0x0006}, {0x006a, 0x000d},
	{0x0050, 0x0000}, {0x0041, 0x0000}, {0x006a, 0x0007}, {0x0061, 0x0006},
	{0x006a, 0x000d}, {0x0000, 0x00c0}, {0x0010, 0x0010}, {0x0001, 0x00c1},
	{0x0041, 0x00c2}, {0x0004, 0x00d8}, {0x0012, 0x0004}, {0x0000, 0x0058},
	{0x0041, 0x0000}, {0x0061, 0x0000},
};

static struct validx tbl_common[] = {
	{0x6000, 0x00ff}, {0x60ff, 0x002c}, {0x60df, 0x002e}, {0x6001, 0x00ff},
	{0x6080, 0x0012}, {0x6000, 0x0000}, {0x6000, 0x0045}, {0x6000, 0x0010},
	{0x6035, 0x003c}, {0x6000, 0x0011}, {0x6028, 0x0004}, {0x60e5, 0x0013},
	{0x6088, 0x0014}, {0x600c, 0x002c}, {0x6078, 0x0033}, {0x60f7, 0x003b},
	{0x6000, 0x003e}, {0x6011, 0x0043}, {0x6010, 0x0016}, {0x6082, 0x0039},
	{0x6088, 0x0035}, {0x600a, 0x0022}, {0x6040, 0x0037}, {0x6000, 0x0023},
	{0x60a0, 0x0034}, {0x601a, 0x0036}, {0x6002, 0x0006}, {0x60c0, 0x0007},
	{0x60b7, 0x000d}, {0x6001, 0x000e}, {0x6000, 0x004c}, {0x6081, 0x004a},
	{0x6099, 0x0021}, {0x6002, 0x0009}, {0x603e, 0x0024}, {0x6034, 0x0025},
	{0x6081, 0x0026}, {0x6000, 0x0000}, {0x6000, 0x0045}, {0x6000, 0x0010},
	{0x6000, 0x005c}, {0x6000, 0x0063}, {0x6000, 0x007c}, {0x6070, 0x0061},
	{0x6080, 0x0062}, {0x6080, 0x0020}, {0x6030, 0x0028}, {0x6000, 0x006c},
	{0x6000, 0x006e}, {0x6002, 0x0070}, {0x6094, 0x0071}, {0x60c1, 0x0073},
	{0x6034, 0x003d}, {0x6057, 0x005a}, {0x60bb, 0x004f}, {0x609c, 0x0050},
	{0x6080, 0x006d}, {0x6002, 0x0039}, {0x6033, 0x003a}, {0x60f1, 0x003b},
	{0x6031, 0x003c}, {0x6000, 0x00ff}, {0x6014, 0x00e0}, {0x60ff, 0x0076},
	{0x60a0, 0x0033}, {0x6020, 0x0042}, {0x6018, 0x0043}, {0x6000, 0x004c},
	{0x60d0, 0x0087}, {0x600f, 0x0088}, {0x6003, 0x00d7}, {0x6010, 0x00d9},
	{0x6005, 0x00da}, {0x6082, 0x00d3}, {0x60c0, 0x00f9}, {0x6006, 0x0044},
	{0x6007, 0x00d1}, {0x6002, 0x00d2}, {0x6000, 0x00d2}, {0x6011, 0x00d8},
	{0x6008, 0x00c8}, {0x6080, 0x00c9}, {0x6008, 0x007c}, {0x6020, 0x007d},
	{0x6020, 0x007d}, {0x6000, 0x0090}, {0x600e, 0x0091}, {0x601a, 0x0091},
	{0x6031, 0x0091}, {0x605a, 0x0091}, {0x6069, 0x0091}, {0x6075, 0x0091},
	{0x607e, 0x0091}, {0x6088, 0x0091}, {0x608f, 0x0091}, {0x6096, 0x0091},
	{0x60a3, 0x0091}, {0x60af, 0x0091}, {0x60c4, 0x0091}, {0x60d7, 0x0091},
	{0x60e8, 0x0091}, {0x6020, 0x0091}, {0x6000, 0x0092}, {0x6006, 0x0093},
	{0x60e3, 0x0093}, {0x6005, 0x0093}, {0x6005, 0x0093}, {0x6000, 0x0093},
	{0x6004, 0x0093}, {0x6000, 0x0093}, {0x6000, 0x0093}, {0x6000, 0x0093},
	{0x6000, 0x0093}, {0x6000, 0x0093}, {0x6000, 0x0093}, {0x6000, 0x0093},
	{0x6000, 0x0096}, {0x6008, 0x0097}, {0x6019, 0x0097}, {0x6002, 0x0097},
	{0x600c, 0x0097}, {0x6024, 0x0097}, {0x6030, 0x0097}, {0x6028, 0x0097},
	{0x6026, 0x0097}, {0x6002, 0x0097}, {0x6098, 0x0097}, {0x6080, 0x0097},
	{0x6000, 0x0097}, {0x6000, 0x0097}, {0x60ed, 0x00c3}, {0x609a, 0x00c4},
	{0x6000, 0x00a4}, {0x6011, 0x00c5}, {0x6051, 0x00c6}, {0x6010, 0x00c7},
	{0x6066, 0x00b6}, {0x60a5, 0x00b8}, {0x6064, 0x00b7}, {0x607c, 0x00b9},
	{0x60af, 0x00b3}, {0x6097, 0x00b4}, {0x60ff, 0x00b5}, {0x60c5, 0x00b0},
	{0x6094, 0x00b1}, {0x600f, 0x00b2}, {0x605c, 0x00c4}, {0x6000, 0x00a8},
	{0x60c8, 0x00c0}, {0x6096, 0x00c1}, {0x601d, 0x0086}, {0x6000, 0x0050},
	{0x6090, 0x0051}, {0x6018, 0x0052}, {0x6000, 0x0053}, {0x6000, 0x0054},
	{0x6088, 0x0055}, {0x6000, 0x0057}, {0x6090, 0x005a}, {0x6018, 0x005b},
	{0x6005, 0x005c}, {0x60ed, 0x00c3}, {0x6000, 0x007f}, {0x6005, 0x00da},
	{0x601f, 0x00e5}, {0x6067, 0x00e1}, {0x6000, 0x00e0}, {0x60ff, 0x00dd},
	{0x6000, 0x0005}, {0x6001, 0x00ff}, {0x6000, 0x0000}, {0x6000, 0x0045},
	{0x6000, 0x0010},
};

static struct validx tbl_sensor_settings_common1[] = {
	{0x0041, 0x0000}, {0x006a, 0x0007}, {0x00ef, 0x0006}, {0x006a, 0x000d},
	{0x0000, 0x00c0}, {0x0010, 0x0010}, {0x0001, 0x00c1}, {0x0041, 0x00c2},
	{0x0004, 0x00d8}, {0x0012, 0x0004}, {0x0000, 0x0058}, {0x0041, 0x0000},
	{50, 0xffff},
	{0x0061, 0x0000},
	{0xffff, 0xffff},
	{0x6000, 0x00ff}, {0x6000, 0x007c}, {0x6007, 0x007d},
	{30, 0xffff},
	{0x0040, 0x0000},
};

static struct validx tbl_sensor_settings_common2[] = {
	{0x6001, 0x00ff}, {0x6038, 0x000c},
	{10, 0xffff},
	{0x6000, 0x0011},
};

static struct validx tbl_640[] = {
	{0x6000, 0x00ff}, {0x60f1, 0x00dd}, {0x6004, 0x00e0}, {0x6067, 0x00e1},
	{0x6004, 0x00da}, {0x6000, 0x00ff}, {0x60f1, 0x00dd}, {0x6004, 0x00e0},
	{0x6001, 0x00ff}, {0x6000, 0x0012}, {0x6000, 0x0011}, {0x6011, 0x0017},
	{0x6075, 0x0018}, {0x6001, 0x0019}, {0x6097, 0x001a}, {0x6036, 0x0032},
	{0x60bb, 0x004f}, {0x6057, 0x005a}, {0x609c, 0x0050}, {0x6080, 0x006d},
	{0x6092, 0x0026}, {0x60ff, 0x0020}, {0x6000, 0x0027}, {0x6000, 0x00ff},
	{0x60c8, 0x00c0}, {0x6096, 0x00c1}, {0x6000, 0x008c}, {0x603d, 0x0086},
	{0x6089, 0x0050}, {0x6090, 0x0051}, {0x602c, 0x0052}, {0x6000, 0x0053},
	{0x6000, 0x0054}, {0x6088, 0x0055}, {0x6000, 0x0057}, {0x60a0, 0x005a},
	{0x6078, 0x005b}, {0x6000, 0x005c}, {0x6004, 0x00d3}, {0x6000, 0x00e0},
	{0x60ff, 0x00dd}, {0x60a1, 0x005a},
};

static struct validx tbl_800[] = {
	{0x6000, 0x00ff}, {0x60f1, 0x00dd}, {0x6004, 0x00e0}, {0x6067, 0x00e1},
	{0x6004, 0x00da}, {0x6000, 0x00ff}, {0x60f1, 0x00dd}, {0x6004, 0x00e0},
	{0x6001, 0x00ff}, {0x6040, 0x0012}, {0x6000, 0x0011}, {0x6011, 0x0017},
	{0x6043, 0x0018}, {0x6000, 0x0019}, {0x604b, 0x001a}, {0x6009, 0x0032},
	{0x60ca, 0x004f}, {0x60a8, 0x0050}, {0x6000, 0x006d}, {0x6038, 0x003d},
	{0x60c8, 0x0035}, {0x6000, 0x0022}, {0x6092, 0x0026}, {0x60ff, 0x0020},
	{0x6000, 0x0027}, {0x6000, 0x00ff}, {0x6064, 0x00c0}, {0x604b, 0x00c1},
	{0x6000, 0x008c}, {0x601d, 0x0086}, {0x6082, 0x00d3}, {0x6000, 0x00e0},
	{0x60ff, 0x00dd}, {0x6020, 0x008c}, {0x6001, 0x00ff}, {0x6044, 0x0018},
};

static struct validx tbl_big1[] = {
	{0x0002, 0x00c1}, {0x6000, 0x00ff}, {0x60f1, 0x00dd}, {0x6004, 0x00e0},
	{0x6001, 0x00ff}, {0x6000, 0x0012}, {0x6000, 0x0000}, {0x6000, 0x0045},
	{0x6000, 0x0010}, {0x6000, 0x0011}, {0x6011, 0x0017}, {0x6075, 0x0018},
	{0x6001, 0x0019}, {0x6097, 0x001a}, {0x6036, 0x0032}, {0x60bb, 0x004f},
	{0x609c, 0x0050}, {0x6057, 0x005a}, {0x6080, 0x006d}, {0x6043, 0x000f},
	{0x608f, 0x0003}, {0x6005, 0x007c}, {0x6081, 0x0026}, {0x6000, 0x00ff},
	{0x60c8, 0x00c0}, {0x6096, 0x00c1}, {0x6000, 0x008c},
};

static struct validx tbl_big2[] = {
	{0x603d, 0x0086}, {0x6000, 0x0050}, {0x6090, 0x0051}, {0x602c, 0x0052},
	{0x6000, 0x0053}, {0x6000, 0x0054}, {0x6088, 0x0055}, {0x6000, 0x0057},
	{0x6040, 0x005a}, {0x60f0, 0x005b}, {0x6001, 0x005c}, {0x6082, 0x00d3},
	{0x6000, 0x008e},
};

static struct validx tbl_big3[] = {
	{0x6004, 0x00da}, {0x6000, 0x00e0}, {0x6067, 0x00e1}, {0x60ff, 0x00dd},
	{0x6001, 0x00ff}, {0x6000, 0x00ff}, {0x60f1, 0x00dd}, {0x6004, 0x00e0},
	{0x6001, 0x00ff}, {0x6000, 0x0011}, {0x6000, 0x00ff}, {0x6010, 0x00c7},
	{0x6000, 0x0092}, {0x6006, 0x0093}, {0x60e3, 0x0093}, {0x6005, 0x0093},
	{0x6005, 0x0093}, {0x60ed, 0x00c3}, {0x6000, 0x00a4}, {0x60d0, 0x0087},
	{0x6003, 0x0096}, {0x600c, 0x0097}, {0x6024, 0x0097}, {0x6030, 0x0097},
	{0x6028, 0x0097}, {0x6026, 0x0097}, {0x6002, 0x0097}, {0x6001, 0x00ff},
	{0x6043, 0x000f}, {0x608f, 0x0003}, {0x6000, 0x002d}, {0x6000, 0x002e},
	{0x600a, 0x0022}, {0x6002, 0x0070}, {0x6008, 0x0014}, {0x6048, 0x0014},
	{0x6000, 0x00ff}, {0x6000, 0x00e0}, {0x60ff, 0x00dd},
};

static struct validx tbl_post_unset_alt[] = {
	{0x006a, 0x000d}, {0x6001, 0x00ff}, {0x6081, 0x0026}, {0x6000, 0x0000},
	{0x6000, 0x0045}, {0x6000, 0x0010}, {0x6068, 0x000d},
	{50, 0xffff},
	{0x0021, 0x0000},
};

static int  ov2640_init_at_startup(struct gspca_dev *gspca_dev);
static int  ov2640_configure_alt(struct gspca_dev *gspca_dev);
static int  ov2640_init_pre_alt(struct gspca_dev *gspca_dev);
static int  ov2640_init_post_alt(struct gspca_dev *gspca_dev);
static void ov2640_post_unset_alt(struct gspca_dev *gspca_dev);
static int  ov2640_camera_settings(struct gspca_dev *gspca_dev);
/*==========================================================================*/

void ov2640_init_settings(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;

	sd->vcur.backlight  =  32;
	sd->vcur.brightness =   0;
	sd->vcur.sharpness  =   6;
	sd->vcur.contrast   =   0;
	sd->vcur.gamma      =  32;
	sd->vcur.hue        =   0;
	sd->vcur.saturation = 128;
	sd->vcur.whitebal   =  64;
	sd->vcur.mirror     =   0;
	sd->vcur.flip       =   0;

	sd->vmax.backlight  =  64;
	sd->vmax.brightness = 255;
	sd->vmax.sharpness  =  31;
	sd->vmax.contrast   = 255;
	sd->vmax.gamma      =  64;
	sd->vmax.hue        = 254 + 2;
	sd->vmax.saturation = 255;
	sd->vmax.whitebal   = 128;
	sd->vmax.mirror     = 1;
	sd->vmax.flip       = 1;
	sd->vmax.AC50Hz     = 0;

	sd->dev_camera_settings = ov2640_camera_settings;
	sd->dev_init_at_startup = ov2640_init_at_startup;
	sd->dev_configure_alt   = ov2640_configure_alt;
	sd->dev_init_pre_alt    = ov2640_init_pre_alt;
	sd->dev_post_unset_alt  = ov2640_post_unset_alt;
}

/*==========================================================================*/

static void common(struct gspca_dev *gspca_dev)
{
	fetch_validx(gspca_dev, tbl_common, ARRAY_SIZE(tbl_common));
}

static int ov2640_init_at_startup(struct gspca_dev *gspca_dev)
{
	fetch_validx(gspca_dev, tbl_init_at_startup,
			ARRAY_SIZE(tbl_init_at_startup));

	ctrl_out(gspca_dev, 0x40, 3, 0x0000, 0x0200, 12, dat_init1);

	common(gspca_dev);

	ctrl_in(gspca_dev, 0xc0, 2, 0x0000, 0x0006, 1, c61);

	ctrl_out(gspca_dev, 0x40, 1, 0x00ef, 0x0006, 0, NULL);

	ctrl_in(gspca_dev, 0xc0, 2, 0x0000, 0x0000, 1, c51);

	ctrl_out(gspca_dev, 0x40, 1, 0x0051, 0x0000, 0, NULL);
/*	ctrl_out(gspca_dev, 0x40, 11, 0x0000, 0x0000, 0, NULL); */

	return 0;
}

static int ov2640_init_pre_alt(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;

	sd->mirrorMask = 0;

	sd->vold.backlight  = -1;
	sd->vold.brightness = -1;
	sd->vold.sharpness  = -1;
	sd->vold.contrast   = -1;
	sd->vold.saturation = -1;
	sd->vold.gamma    = -1;
	sd->vold.hue      = -1;
	sd->vold.whitebal = -1;
	sd->vold.mirror = -1;
	sd->vold.flip   = -1;

	ov2640_init_post_alt(gspca_dev);

	return 0;
}

static int ov2640_init_post_alt(struct gspca_dev *gspca_dev)
{
	s32 reso = gspca_dev->cam.cam_mode[(s32) gspca_dev->curr_mode].priv;
	s32 n; /* reserved for FETCH functions */

	ctrl_out(gspca_dev, 0x40, 5, 0x0001, 0x0000, 0, NULL);

	n = fetch_validx(gspca_dev, tbl_sensor_settings_common1,
			ARRAY_SIZE(tbl_sensor_settings_common1));
	ctrl_out(gspca_dev, 0x40, 3, 0x0000, 0x0200, 12, dat_post);
	common(gspca_dev);
	keep_on_fetching_validx(gspca_dev, tbl_sensor_settings_common1,
				ARRAY_SIZE(tbl_sensor_settings_common1), n);

	switch (reso) {
	case IMAGE_640:
		n = fetch_validx(gspca_dev, tbl_640, ARRAY_SIZE(tbl_640));
		ctrl_out(gspca_dev, 0x40, 3, 0x0000, 0x0200, 12, dat_640);
		break;

	case IMAGE_800:
		n = fetch_validx(gspca_dev, tbl_800, ARRAY_SIZE(tbl_800));
		ctrl_out(gspca_dev, 0x40, 3, 0x0000, 0x0200, 12, dat_800);
		break;

	case IMAGE_1600:
	case IMAGE_1280:
		n = fetch_validx(gspca_dev, tbl_big1, ARRAY_SIZE(tbl_big1));

		if (reso == IMAGE_1280) {
			n = fetch_validx(gspca_dev, tbl_big2,
					ARRAY_SIZE(tbl_big2));
		} else {
			ctrl_out(gspca_dev, 0x40, 1, 0x601d, 0x0086, 0, NULL);
			ctrl_out(gspca_dev, 0x40, 1, 0x6001, 0x00d7, 0, NULL);
			ctrl_out(gspca_dev, 0x40, 1, 0x6082, 0x00d3, 0, NULL);
		}

		n = fetch_validx(gspca_dev, tbl_big3, ARRAY_SIZE(tbl_big3));

		if (reso == IMAGE_1280) {
			ctrl_out(gspca_dev, 0x40, 1, 0x6001, 0x00ff, 0, NULL);
			ctrl_out(gspca_dev, 0x40, 3, 0x0000, 0x0200,
					12, dat_1280);
		} else {
			ctrl_out(gspca_dev, 0x40, 1, 0x6020, 0x008c, 0, NULL);
			ctrl_out(gspca_dev, 0x40, 1, 0x6001, 0x00ff, 0, NULL);
			ctrl_out(gspca_dev, 0x40, 1, 0x6076, 0x0018, 0, NULL);
			ctrl_out(gspca_dev, 0x40, 3, 0x0000, 0x0200,
					12, dat_1600);
		}
		break;
	}

	n = fetch_validx(gspca_dev, tbl_sensor_settings_common2,
			ARRAY_SIZE(tbl_sensor_settings_common2));

	ov2640_camera_settings(gspca_dev);

	return 0;
}

static int ov2640_configure_alt(struct gspca_dev *gspca_dev)
{
	s32 reso = gspca_dev->cam.cam_mode[(s32) gspca_dev->curr_mode].priv;

	switch (reso) {
	case IMAGE_640:
		gspca_dev->alt = 3 + 1;
		break;

	case IMAGE_800:
	case IMAGE_1280:
	case IMAGE_1600:
		gspca_dev->alt = 1 + 1;
		break;
	}
	return 0;
}

static int ov2640_camera_settings(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;

	s32 backlight = sd->vcur.backlight;
	s32 bright = sd->vcur.brightness;
	s32 sharp  = sd->vcur.sharpness;
	s32 gam    = sd->vcur.gamma;
	s32 cntr   = sd->vcur.contrast;
	s32 sat    = sd->vcur.saturation;
	s32 hue    = sd->vcur.hue;
	s32 wbal   = sd->vcur.whitebal;
	s32 mirror = (((sd->vcur.mirror > 0) ^ sd->mirrorMask) == 0);
	s32 flip   = (((sd->vcur.flip   > 0) ^ sd->mirrorMask) == 0);

	if (backlight != sd->vold.backlight) {
		/* No sd->vold.backlight=backlight; (to be done again later) */
		if (backlight < 0 || backlight > sd->vmax.backlight)
			backlight = 0;

		ctrl_out(gspca_dev, 0x40, 1, 0x6001                 , 0x00ff,
				0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x601e + backlight     , 0x0024,
				0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x601e + backlight - 10, 0x0025,
				0, NULL);
	}

	if (bright != sd->vold.brightness) {
		sd->vold.brightness = bright;
		if (bright < 0 || bright > sd->vmax.brightness)
			bright = 0;

		ctrl_out(gspca_dev, 0x40, 1, 0x6000         , 0x00ff, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6009         , 0x007c, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6000 + bright, 0x007d, 0, NULL);
	}

	if (wbal != sd->vold.whitebal) {
		sd->vold.whitebal = wbal;
		if (wbal < 0 || wbal > sd->vmax.whitebal)
			wbal = 0;

		ctrl_out(gspca_dev, 0x40, 1, 0x6000       , 0x00ff, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6003       , 0x007c, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6000 + wbal, 0x007d, 0, NULL);
	}

	if (cntr != sd->vold.contrast) {
		sd->vold.contrast = cntr;
		if (cntr < 0 || cntr > sd->vmax.contrast)
			cntr = 0;

		ctrl_out(gspca_dev, 0x40, 1, 0x6000       , 0x00ff, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6007       , 0x007c, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6000 + cntr, 0x007d, 0, NULL);
	}

	if (sat != sd->vold.saturation) {
		sd->vold.saturation = sat;
		if (sat < 0 || sat > sd->vmax.saturation)
			sat = 0;

		ctrl_out(gspca_dev, 0x40, 1, 0x6000      , 0x00ff, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6001      , 0x007c, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6000 + sat, 0x007d, 0, NULL);
	}

	if (sharp != sd->vold.sharpness) {
		sd->vold.sharpness = sharp;
		if (sharp < 0 || sharp > sd->vmax.sharpness)
			sharp = 0;

		ctrl_out(gspca_dev, 0x40, 1, 0x6000        , 0x00ff, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6001        , 0x0092, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x60c0 + sharp, 0x0093, 0, NULL);
	}

	if (hue != sd->vold.hue) {
		sd->vold.hue = hue;
		if (hue < 0 || hue > sd->vmax.hue)
			hue = 0;

		ctrl_out(gspca_dev, 0x40, 1, 0x6000     , 0x00ff, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6002     , 0x007c, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6000 + hue * (hue < 255), 0x007d,
				0, NULL);
		if (hue >= 255)
			sd->swapRB = 1;
		else
			sd->swapRB = 0;
	}

	if (gam != sd->vold.gamma) {
		sd->vold.gamma = gam;
		if (gam < 0 || gam > sd->vmax.gamma)
			gam = 0;

		ctrl_out(gspca_dev, 0x40, 1, 0x6000      , 0x00ff, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6008      , 0x007c, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6000 + gam, 0x007d, 0, NULL);
	}

	if (mirror != sd->vold.mirror || flip != sd->vold.flip) {
		sd->vold.mirror = mirror;
		sd->vold.flip   = flip;

		mirror = 0x80 * mirror;
		ctrl_out(gspca_dev, 0x40, 1, 0x6001, 0x00ff, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6000, 0x8004, 0, NULL);
		ctrl_in(gspca_dev, 0xc0, 2, 0x6000, 0x8004, 1, c28);
		ctrl_out(gspca_dev, 0x40, 1, 0x6028 + mirror, 0x0004, 0, NULL);

		flip = 0x50 * flip + mirror;
		ctrl_out(gspca_dev, 0x40, 1, 0x6001, 0x00ff, 0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x6000, 0x8004, 0, NULL);
		ctrl_in(gspca_dev, 0xc0, 2, 0x6000, 0x8004, 1, ca8);
		ctrl_out(gspca_dev, 0x40, 1, 0x6028 + flip, 0x0004, 0, NULL);

		ctrl_in(gspca_dev, 0xc0, 2, 0x0000, 0x0000, 1, c50);
	}

	if (backlight != sd->vold.backlight) {
		sd->vold.backlight = backlight;

		ctrl_out(gspca_dev, 0x40, 1, 0x6001                 , 0x00ff,
				0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x601e + backlight     , 0x0024,
				0, NULL);
		ctrl_out(gspca_dev, 0x40, 1, 0x601e + backlight - 10, 0x0025,
				0, NULL);
	}

	return 0;
}

static void ov2640_post_unset_alt(struct gspca_dev *gspca_dev)
{
	ctrl_out(gspca_dev, 0x40, 5, 0x0000, 0x0000, 0, NULL);
	msleep(20);
	fetch_validx(gspca_dev, tbl_post_unset_alt,
			ARRAY_SIZE(tbl_post_unset_alt));
}

/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2023, Gold Bevel 2 (Pirates Gold) Gimp Plugin - Beaver

You can test plugin without installing by pasting this syntax into Gimp's GEGL graph (requires plugins sharp bevel and metallic) that do ship with it.

lb:sharpbevel bevelcolorpolicy=true color=#fc72fd bevelcontrol=0.5  blendmode=hardlight smooth=16 sharpen=1 depth=72 elevation=57 size=2
crop

metallic
guichange=colormetal
liquid=0.51900000000000001
solar1=5.2619999999999996
solar2=5.6210000000000004
solar3=6000000000000001
light=0
smooth=2
value=0
color=#f8f2d4
coloropacity=0.9
blend=graph4


id=1
over aux=[ ref=1 noise-rgb correlated=false linear=false independent=false red=0.85  green=0.20 blue=0.20  gaussian=true opacity value=0.27   noise-reduction iterations=2 ]



bloom strength=33 radius=8

levels in-low=0.075
median-blur radius=0

--end of syntax---

Fun fact, during development this plugin it was called "Pirates Gold" as it reminds Beaver (me) of a manual text style that I made back in 2020 based on a Gimp + GMIC discovery of mine. Today it is exciting to see an effect like that one that is automated by GEGL.

Uses blend modes ( Hardlight,  Color Dodge, Plus, Darken, Softlight)

 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_boolean(advanced_options, _("Detailed options"), FALSE)
   description  (_("Show all settings for this plugin."))




enum_start (gold_sharp_bevel)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT, "hardlight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "colordodge",
              N_("Color Dodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS,      "plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN,      "darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT,      "softlight",
              N_("Soft Light"))
enum_end (goldsharpbevel)
/* This ENUM list has a unique name to avoid conflicts with other plugins or Gimp filters*/

property_enum (blendmode, _("Blend Mode of Sharp Bevel's Internal Emboss"),
    goldsharpbevel, gold_sharp_bevel,
    GEGL_BLEND_MODE_TYPE_HARDLIGHT)
   description  (_("Blend mode of Sharp Bevel's internal emboss"))



property_color (color, _("Color Overlay of Metal"), "#f8f2d4")
   description  (_("This filter can be any color not just gold."))

property_double (opacitycolor, _("Opacity of Color. (0% opacity = Silver)"), 0.90)
   description  (_("Opacity of Color. Make opacity 0 for silver"))
   value_range  (0.00, 0.90)

property_int  (size, _("Size of the Bevel"), 3)
  value_range (1, 9)
  ui_range    (1, 9)
  ui_meta     ("unit", "pixel-distance")
  description (_("Median Radius to control the size of the bevel"))

property_double (flatsurface, _("Bevel's Flat Surface control"), 1)
    value_range (1.0, 2.5)
  description (_("Moving this slider up will give the bevel a flat surface. At 1 it will be a default sharp surface."))

property_double (azimuth, _("Azimuth"), 67.0)
    description (_("Light angle (degrees)"))
    value_range (20, 90)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (elevation, _("Elevation"), 35.0)
    description (_("Elevation angle (degrees). This appears to rotate the brightest pixels."))
    value_range (7, 70)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth and or detail"), 24)
    description (_("Brings out depth and or detail of the bevel depending on the blend mode"))
    value_range (8, 45)


property_double (sharpen, _("Sharpen Radius"), 0.0)
    description (_("Unsharp mask radius that helps the bevel apper to have depth and structure to it."))
    value_range (0, 5)
    ui_range (0, 5)
   ui_meta ("visible", "advanced_options")


property_double (liquid, _("Liquidify Metal"), 0.0)
    description (_("In default this is disabled, but when enabled it will smoothly expand the metal and make it some what 'liquid-like'. "))
  value_range (0.0, 0.5)
  ui_steps      (0.0, 0.5)
   ui_meta ("visible", "advanced_options")


property_double (solar1, _("Solarization of Red Channel"), 5.2)
    description (_("Solarization is another name for Gimp's 'alien map' filter effect, which at low values shifts tones. A method which can create a metallic effect when combined with desaturation. "))
  value_range (5.0, 6.0)
  ui_steps      (5.0, 6.0)



property_double  (solar2, _("Solarization of Green Channel"), 5.6)
    description (_("Solarization is another name for Gimp's 'alien map' filter effect, which at low values shifts tones. A method which can create a metallic effect when combined with desaturation. "))
  value_range (4.5, 5.6)
  ui_steps      (4.5, 5.6)


property_double  (solar3, _("Solarization of Blue Channel"), 2.1)
    description (_("Solarization is another name for Gimp's 'alien map' filter effect, which at low values shifts tones. A method which can create a metallic effect when combined with desaturation. "))
  value_range (2.0, 2.6)
  ui_steps      (2.0, 5.0)

property_double (bloom, _("Bloom glow effect"), 29.0)
    description (_("This is Gimp's default 'bloom' filter being called to make the metal glow. It can also lighten the metal"))
    value_range (6.0, 70.0)
    ui_range    (6.0, 70.0)

property_int  (smooth, _("Smooth Metal"), 2)
  description (_("Noise reduction to smooth the metal"))
  value_range (1, 4)
  ui_range    (1, 4)
   ui_meta ("visible", "advanced_options")

property_double (opacitynoise, _("Opacity of noise"), 0.27)
    description (_("Opacity of noise seed. 0 disables."))
    value_range (0.0, 1.00)
    ui_range    (0.0, 0.40)
   ui_meta ("visible", "advanced_options")

property_seed (seed, _("Random seed of noise"), rand)
   ui_meta ("visible", "advanced_options")



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     goldbevel2
#define GEGL_OP_C_SOURCE goldbevel2.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *sharpbevelhardlight;
 GeglNode *sharpbevelcolordodge;
 GeglNode *sharpbevelplus;
 GeglNode *sharpbeveldarken;
 GeglNode *sharpbevelsoftlight;
 GeglNode *metallic;
 GeglNode *bloom;
 GeglNode *crop;
 GeglNode *over;
 GeglNode *idref;
 GeglNode *fix;
 GeglNode *nr;
 GeglNode *inlow;
 GeglNode *noise;
 GeglNode *opacity;
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{


}

static void update_graph (GeglOperation *operation)
{

  GeglNode *gegl = operation->node;
  GeglColor *bakedcolor = gegl_color_new ("#fc72fd");
  GeglColor *bakedcolor2 = gegl_color_new ("#f8f2d4");
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));

  if (!state) return;



  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");


  state->sharpbevelhardlight = gegl_node_new_child (gegl,
                                  "operation", "lb:sharpbevel", "bevelcolorpolicy", TRUE, "color", bakedcolor, "bevelcontrol", 1.5, "blendmode", 0, "elevation", 35.0, "depth", 24,
                                  NULL);

  state->sharpbevelcolordodge = gegl_node_new_child (gegl,
                                  "operation", "lb:sharpbevel", "bevelcolorpolicy", TRUE, "color", bakedcolor, "bevelcontrol", 1.5, "blendmode", 2, "elevation", 35.0, "depth", 24,
                                  NULL);

  state->sharpbevelplus = gegl_node_new_child (gegl,
                                  "operation", "lb:sharpbevel", "bevelcolorpolicy", TRUE, "color", bakedcolor, "bevelcontrol", 1.5, "blendmode", 3, "elevation", 35.0, "depth", 24,
                                  NULL);

  state->sharpbeveldarken = gegl_node_new_child (gegl,
                                  "operation", "lb:sharpbevel", "bevelcolorpolicy", TRUE, "color", bakedcolor, "bevelcontrol", 1.5, "blendmode", 4, "elevation", 35.0, "depth", 24,
                                  NULL);

  state->sharpbevelsoftlight = gegl_node_new_child (gegl,
                                  "operation", "lb:sharpbevel", "bevelcolorpolicy", TRUE, "color", bakedcolor, "bevelcontrol", 1.5, "blendmode", 8, "elevation", 35.0, "depth", 24,
                                  NULL);

  state->metallic = gegl_node_new_child (gegl,
                                  "operation", "lb:metallic", "guichange", 1, "liquid", 0.3, "solar1", 5.2, "solar2", 5.6, "solar3", 2.1, "light", 0.0, "smooth", 2, "color", bakedcolor2,  "blend", 3,
                                  NULL);
/*These, "sharp bevel and metallic" are plugins of mine that are useful on their own. Sharp bevel is listed five times each on a different blend mode (hardlight, colordodge, plus, darken, and softlight*/

  state->bloom = gegl_node_new_child (gegl,
                                  "operation", "gegl:bloom", "strength", 29.0, "radius", 8.0,
                                  NULL);

  state->crop = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  state->over = gegl_node_new_child (gegl,
                                  "operation", "gegl:over",
                                  NULL);

  state->idref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  state->fix = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,  "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);
/*Median blur at 0 radius makes no changes to an image but resets a GEGL graph*/

  state->nr = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction", "iterations", 2,
                                  NULL);

  state->inlow = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", "in-low", 0.075,
                                  NULL);

  state->noise = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-rgb", "correlated", FALSE, "independent", FALSE, "linear", FALSE, "gaussian", FALSE, "red", 0.80, "green", 0.20, "blue", 0.20,
                                  NULL);

  state->opacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 0.27,
                                  NULL);




  GeglNode *sharpbevel = state->sharpbevelhardlight; /* the default */
  switch (o->blendmode) {
    case GEGL_BLEND_MODE_TYPE_HARDLIGHT: sharpbevel = state->sharpbevelhardlight; break;
    case GEGL_BLEND_MODE_TYPE_COLORDODGE: sharpbevel = state->sharpbevelcolordodge; break;
    case GEGL_BLEND_MODE_TYPE_PLUS: sharpbevel = state->sharpbevelplus; break;
    case GEGL_BLEND_MODE_TYPE_DARKEN: sharpbevel = state->sharpbeveldarken; break;
    case GEGL_BLEND_MODE_TYPE_SOFTLIGHT: sharpbevel = state->sharpbevelsoftlight; break;
default: sharpbevel = state->sharpbevelhardlight;

}

  gegl_node_link_many (state->input, sharpbevel, state->crop, state->metallic, state->idref, state->over, state->bloom, state->inlow, state->fix, state->output, NULL);
  gegl_node_link_many (state->idref, state->noise, state->opacity, state->nr,   NULL);
  gegl_node_connect (state->over, "aux", state->nr, "output");
  gegl_node_connect (state->crop, "aux", state->input, "output");

/*Outside of one complex exception, this is actually a very simple GEGL graph. Noise and Opacity are inside a "normal" blend mode called "over", everything else is just basic nodes being listed. Remember though, sharpbevel and metallic are plugins of mine that contain complex node connections of their own within their respected .c files. Same goes for Gimp's native bloom filter. The reason noise is inside the over blend mode is so it can be made semi transparent or disabled
by being made fully transparent and so filter `noise reduction` may run on it alone and not the rest of the graph. Think of the `over` blend mode as a container for other filters so there effects don't leak elsewhere. This is all simple GEGL stuff.

The one complex exception is that the node 'sharpbevel' is set to switch to multiple copies of my node/filter (lb:sharpbevel) that are on different blend modes. This is a blend mode switching process. The user chooses what blend mode they want sharp bevel to be on. */

    gegl_operation_meta_redirect (operation, "solar1", state->metallic, "solar1");
    gegl_operation_meta_redirect (operation, "solar2", state->metallic, "solar2");
    gegl_operation_meta_redirect (operation, "solar3", state->metallic, "solar3");
    gegl_operation_meta_redirect (operation, "color", state->metallic, "color");
    gegl_operation_meta_redirect (operation, "liquid", state->metallic, "liquid");
    gegl_operation_meta_redirect (operation, "opacitycolor", state->metallic, "coloropacity");
    gegl_operation_meta_redirect (operation, "bloom", state->bloom, "strength");
    gegl_operation_meta_redirect (operation, "seed", state->noise, "seed");
    gegl_operation_meta_redirect (operation, "opacitynoise", state->opacity, "value");
    gegl_operation_meta_redirect (operation, "smooth", state->nr, "iterations");

    gegl_operation_meta_redirect (operation, "blendmode", state->sharpbevelhardlight, "blendmode");
    gegl_operation_meta_redirect (operation, "blendmode", state->sharpbevelcolordodge, "blendmode");
    gegl_operation_meta_redirect (operation, "blendmode", state->sharpbevelplus, "blendmode");
    gegl_operation_meta_redirect (operation, "blendmode", state->sharpbeveldarken, "blendmode");
    gegl_operation_meta_redirect (operation, "blendmode", state->sharpbevelsoftlight, "blendmode");

/*Sept 19 dev note, These may all need to be relisted five times for each blend mode of sharp bevel.
 *Sept 20th update. They don't need to be relisted, outside of blendmode*/

    gegl_operation_meta_redirect (operation, "flatsurface", sharpbevel, "bevelcontrol");
    gegl_operation_meta_redirect (operation, "azimuth", sharpbevel, "azimuth");
    gegl_operation_meta_redirect (operation, "elevation", sharpbevel, "elevation");
    gegl_operation_meta_redirect (operation, "depth", sharpbevel, "depth");
    gegl_operation_meta_redirect (operation, "sharpen", sharpbevel, "sharpen");
    gegl_operation_meta_redirect (operation, "size", sharpbevel, "size");
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_meta_class->update = update_graph;
operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:goldbevel",
    "title",       _("Gold Bevel"),
    "reference-hash", "falfgpirat345345egold",
    "description", _("A special filter that calls my existing filters Metallic and Sharp Bevel with other effects to create beveled gold text."),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Gold Bevel..."),
    NULL);
}

#endif

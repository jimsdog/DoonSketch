#ifndef SP_FESPOTLIGHT_H_SEEN
#define SP_FESPOTLIGHT_H_SEEN

/** \file
 * SVG <filter> implementation, see sp-filter.cpp.
 */
/*
 * Authors:
 *   Hugo Rodrigues <haa.rodrigues@gmail.com>
 *   Niko Kiirala <niko@kiirala.com>
 *   Jean-Rene Reinhard <jr@komite.net>
 *
 * Copyright (C) 2006,2007 Authors
 *
 * Released under GNU GPL, read the file 'COPYING' for more information
 */

#include "sp-object.h"

#define SP_TYPE_FESPOTLIGHT (sp_fespotlight_get_type())
#define SP_FESPOTLIGHT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), SP_TYPE_FESPOTLIGHT, SPFeSpotLight))
#define SP_FESPOTLIGHT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), SP_TYPE_FESPOTLIGHT, SPFeSpotLightClass))
#define SP_IS_FESPOTLIGHT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), SP_TYPE_FESPOTLIGHT))
#define SP_IS_FESPOTLIGHT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), SP_TYPE_FESPOTLIGHT))

/* Distant light class */


class SPFeSpotLight;
class SPFeSpotLightClass;

struct SPFeSpotLight : public SPObject {

    /** x coordinate of the light source */
    gfloat x; 
    guint x_set : 1;
    /** y coordinate of the light source */
    gfloat y; 
    guint y_set : 1;
    /** z coordinate of the light source */
    gfloat z; 
    guint z_set : 1;
    /** x coordinate of the point the source is pointing at */
    gfloat pointsAtX;
    guint pointsAtX_set : 1;
    /** y coordinate of the point the source is pointing at */
    gfloat pointsAtY;
    guint pointsAtY_set : 1;
    /** z coordinate of the point the source is pointing at */
    gfloat pointsAtZ;
    guint pointsAtZ_set : 1;
    /** specular exponent (focus of the light) */
    gfloat specularExponent;
    guint specularExponent_set : 1;
    /** limiting cone angle */
    gfloat limitingConeAngle;
    guint limitingConeAngle_set : 1;
    //other fields
};

struct SPFeSpotLightClass {
    SPObjectClass parent_class;
};

GType
sp_fespotlight_get_type();
#endif /* !SP_FESPOTLIGHT_H_SEEN */

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=99 :

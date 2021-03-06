#ifndef __NR_FILTER_BLEND_H__
#define __NR_FILTER_BLEND_H__

/*
 * SVG feBlend renderer
 *
 * "This filter composites two objects together using commonly used
 * imaging software blending modes. It performs a pixel-wise combination
 * of two input images." 
 * http://www.w3.org/TR/SVG11/filters.html#feBlend
 *
 * Authors:
 *   Niko Kiirala <niko@kiirala.com>
 *
 * Copyright (C) 2007 authors
 *
 * Released under GNU GPL, read the file 'COPYING' for more information
 */

#include "display/nr-filter-primitive.h"
#include "display/nr-filter-slot.h"
#include "display/nr-filter-units.h"

namespace Inkscape {
namespace Filters {

enum FilterBlendMode {
    BLEND_NORMAL,
    BLEND_MULTIPLY,
    BLEND_SCREEN,
    BLEND_DARKEN,
    BLEND_LIGHTEN,
    BLEND_ENDMODE
};

class FilterBlend : public FilterPrimitive {
public:
    FilterBlend();
    static FilterPrimitive *create();
    virtual ~FilterBlend();

    virtual int render(FilterSlot &slot, FilterUnits const &units);

    virtual void set_input(int slot);
    virtual void set_input(int input, int slot);
    void set_mode(FilterBlendMode mode);

private:
    FilterBlendMode _blend_mode;
    int _input2;
};


} /* namespace Filters */
} /* namespace Inkscape */




#endif /* __NR_FILTER_BLEND_H__ */
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

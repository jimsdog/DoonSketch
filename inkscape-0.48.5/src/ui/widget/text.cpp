/**
 * \brief Text Widget - A labelled text box, with spin buttons and optional
 *        icon or suffix, for entering arbitrary number values.
 *
 * Authors:
 *   Carl Hetherington <inkscape@carlh.net>
 *   Maximilian Albert <maximilian.albert@gmail.com>
 *
 * Copyright (C) 2004 Carl Hetherington
 *
 * Released under GNU GPL.  Read the file 'COPYING' for more information.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


#include "text.h"

namespace Inkscape {
namespace UI {
namespace Widget {

/**
 * Construct a Text Widget.
 *
 * \param label     Label.
 * \param suffix    Suffix, placed after the widget (defaults to "").
 * \param icon      Icon filename, placed before the label (defaults to "").
 * \param mnemonic  Mnemonic toggle; if true, an underscore (_) in the label
 *                  indicates the next character should be used for the
 *                  mnemonic accelerator key (defaults to false).
 */
Text::Text(Glib::ustring const &label, Glib::ustring const &tooltip,
               Glib::ustring const &suffix,
               Glib::ustring const &icon,
               bool mnemonic)
    : Labelled(label, tooltip, new Gtk::Entry(), suffix, icon, mnemonic),
      setProgrammatically(false)
{
}

/** Get the text in the entry */
const char *
Text::getText() const
{
    g_assert(_widget != NULL);
    return static_cast<Gtk::Entry*>(_widget)->get_text().c_str();
}

/** Sets the text of the text entry */
void
Text::setText(const char* text)
{
    g_assert(_widget != NULL);
    setProgrammatically = true; // callback is supposed to reset back, if it cares
    static_cast<Gtk::Entry*>(_widget)->set_text(text); // FIXME: set correctly
}

/** Signal raised when the spin button's value changes */
Glib::SignalProxy0<void>
Text::signal_activate()
{
    return static_cast<Gtk::Entry*>(_widget)->signal_activate();
}


} // namespace Widget
} // namespace UI
} // namespace Inkscape

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

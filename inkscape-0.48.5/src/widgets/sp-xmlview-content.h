#ifndef __SP_XMLVIEW_CONTENT_H__
#define __SP_XMLVIEW_CONTENT_H__

/*
 * Specialization of GtkTextView for editing XML node text
 *
 * Authors:
 *   MenTaLguY <mental@rydia.net>
 *
 * Copyright (C) 2002 MenTaLguY
 *
 * Released under the GNU GPL; see COPYING for details
 */

#include <config.h>

#include <stdio.h>

#include <gtk/gtk.h>
#include "../xml/repr.h"

#include <glib.h>



#define SP_TYPE_XMLVIEW_CONTENT (sp_xmlview_content_get_type ())
#define SP_XMLVIEW_CONTENT(o) (GTK_CHECK_CAST ((o), SP_TYPE_XMLVIEW_CONTENT, SPXMLViewContent))
#define SP_IS_XMLVIEW_CONTENT(o) (GTK_CHECK_TYPE ((o), SP_TYPE_XMLVIEW_CONTENT))
#define SP_XMLVIEW_CONTENT_CLASS(k) (GTK_CHECK_CLASS_TYPE ((k), SP_TYPE_XMLVIEW_CONTENT))

struct SPXMLViewContent
{
	GtkTextView textview;

	Inkscape::XML::Node * repr;
	gint blocked;
};

struct SPXMLViewContentClass
{
	GtkTextViewClass parent_class;
};

GtkType sp_xmlview_content_get_type (void);
GtkWidget * sp_xmlview_content_new (Inkscape::XML::Node * repr);

#define SP_XMLVIEW_CONTENT_GET_REPR(text) (SP_XMLVIEW_CONTENT (text)->repr)

void sp_xmlview_content_set_repr (SPXMLViewContent * text, Inkscape::XML::Node * repr);



#endif

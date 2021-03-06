


#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "icon-size.h"
#include "ink-action.h"

#include "widgets/button.h"
#include "widgets/icon.h"

static void ink_action_class_init( InkActionClass* klass );
static void ink_action_init( InkAction* action );
static void ink_action_finalize( GObject* obj );
static void ink_action_get_property( GObject* obj, guint propId, GValue* value, GParamSpec * pspec );
static void ink_action_set_property( GObject* obj, guint propId, const GValue *value, GParamSpec* pspec );

static GtkWidget* ink_action_create_menu_item( GtkAction* action );
static GtkWidget* ink_action_create_tool_item( GtkAction* action );

static GtkActionClass* gInkActionParentClass = 0;

struct _InkActionPrivate
{
    gchar* iconId;
    Inkscape::IconSize iconSize;
};

#define INK_ACTION_GET_PRIVATE( o ) ( G_TYPE_INSTANCE_GET_PRIVATE( (o), INK_ACTION_TYPE, InkActionPrivate ) )

GType ink_action_get_type( void )
{
    static GType myType = 0;
    if ( !myType ) {
        static const GTypeInfo myInfo = {
            sizeof( InkActionClass ),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)ink_action_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof( InkAction ),
            0, /* n_preallocs */
            (GInstanceInitFunc)ink_action_init,
            NULL
        };

        myType = g_type_register_static( GTK_TYPE_ACTION, "InkAction", &myInfo, (GTypeFlags)0 );
    }

    return myType;
}

enum {
    PROP_INK_ID = 1,
    PROP_INK_SIZE
};

static void ink_action_class_init( InkActionClass* klass )
{
    if ( klass ) {
        gInkActionParentClass = GTK_ACTION_CLASS( g_type_class_peek_parent( klass ) );
        GObjectClass * objClass = G_OBJECT_CLASS( klass );

        objClass->finalize = ink_action_finalize;
        objClass->get_property = ink_action_get_property;
        objClass->set_property = ink_action_set_property;

        klass->parent_class.create_menu_item = ink_action_create_menu_item;
        klass->parent_class.create_tool_item = ink_action_create_tool_item;
        /*klass->parent_class.connect_proxy = connect_proxy;*/
        /*klass->parent_class.disconnect_proxy = disconnect_proxy;*/

        g_object_class_install_property( objClass,
                                         PROP_INK_ID,
                                         g_param_spec_string( "iconId",
                                                              "Icon ID",
                                                              "The id for the icon",
                                                              "",
                                                              (GParamFlags)(G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT) ) );

        g_object_class_install_property( objClass,
                                         PROP_INK_SIZE,
                                         g_param_spec_int( "iconSize",
                                                           "Icon Size",
                                                           "The size the icon",
                                                           (int)Inkscape::ICON_SIZE_MENU,
                                                           (int)Inkscape::ICON_SIZE_DECORATION,
                                                           (int)Inkscape::ICON_SIZE_SMALL_TOOLBAR,
                                                           (GParamFlags)(G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT) ) );

        g_type_class_add_private( klass, sizeof(InkActionClass) );
    }
}

static void ink_action_init( InkAction* action )
{
    action->private_data = INK_ACTION_GET_PRIVATE( action );
    action->private_data->iconId = 0;
    action->private_data->iconSize = Inkscape::ICON_SIZE_SMALL_TOOLBAR;
}

static void ink_action_finalize( GObject* obj )
{
    InkAction* action = INK_ACTION( obj );

    g_free( action->private_data->iconId );
    g_free( action->private_data );

}

//Any strings passed in should already be localised
InkAction* ink_action_new( const gchar *name,
                           const gchar *label,
                           const gchar *tooltip,
                           const gchar *inkId,
                           Inkscape::IconSize size )
{
    GObject* obj = (GObject*)g_object_new( INK_ACTION_TYPE,
                                           "name", name,
                                           "label", label,
                                           "tooltip", tooltip,
                                           "iconId", inkId,
                                           "iconSize", size,
                                           NULL );

    InkAction* action = INK_ACTION( obj );

    return action;
}

static void ink_action_get_property( GObject* obj, guint propId, GValue* value, GParamSpec * pspec )
{
    InkAction* action = INK_ACTION( obj );
    (void)action;
    switch ( propId ) {
        case PROP_INK_ID:
        {
            g_value_set_string( value, action->private_data->iconId );
        }
        break;

        case PROP_INK_SIZE:
        {
            g_value_set_int( value, action->private_data->iconSize );
        }
        break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID( obj, propId, pspec );
    }
}

void ink_action_set_property( GObject* obj, guint propId, const GValue *value, GParamSpec* pspec )
{
    InkAction* action = INK_ACTION( obj );
    (void)action;
    switch ( propId ) {
        case PROP_INK_ID:
        {
            gchar* tmp = action->private_data->iconId;
            action->private_data->iconId = g_value_dup_string( value );
            g_free( tmp );
        }
        break;

        case PROP_INK_SIZE:
        {
            action->private_data->iconSize = (Inkscape::IconSize)g_value_get_int( value );
        }
        break;

        default:
        {
            G_OBJECT_WARN_INVALID_PROPERTY_ID( obj, propId, pspec );
        }
    }
}

#include <gtk/gtk.h>

static GtkWidget* ink_action_create_menu_item( GtkAction* action )
{
    InkAction* act = INK_ACTION( action );
    GtkWidget* item = 0;

    if ( act->private_data->iconId ) {
        gchar* label = 0;
        g_object_get( G_OBJECT(act), "label", &label, NULL );

        item = gtk_image_menu_item_new_with_mnemonic( label );
        GtkWidget* child = sp_icon_new( Inkscape::ICON_SIZE_MENU, act->private_data->iconId );
        // TODO this work-around is until SPIcon will live properly inside of a popup menu
        if ( SP_IS_ICON(child) ) {
            SPIcon* icon = SP_ICON(child);
            sp_icon_fetch_pixbuf( icon );
            GdkPixbuf* target = icon->pb;
            if ( target ) {
                child = gtk_image_new_from_pixbuf( target );
                gtk_widget_set_sensitive(child, gtk_action_is_sensitive(action));
                gtk_widget_destroy( GTK_WIDGET(icon) );
            }
        }
        gtk_widget_show_all( child );
        gtk_image_menu_item_set_image( GTK_IMAGE_MENU_ITEM(item), child );

        g_free( label );
        label = 0;
    } else {
        item = gInkActionParentClass->create_menu_item( action );
    }

    return item;
}

static GtkWidget* ink_action_create_tool_item( GtkAction* action )
{
    InkAction* act = INK_ACTION( action );
    GtkWidget* item = gInkActionParentClass->create_tool_item(action);

    if ( act->private_data->iconId ) {
        if ( GTK_IS_TOOL_BUTTON(item) ) {
            GtkToolButton* button = GTK_TOOL_BUTTON(item);

            GtkWidget* child = sp_icon_new( act->private_data->iconSize, act->private_data->iconId );
            gtk_tool_button_set_icon_widget( button, child );
        } else {
            // For now trigger a warning but don't do anything else
            GtkToolButton* button = GTK_TOOL_BUTTON(item);
            (void)button;
        }
    }

    // TODO investigate if needed
    gtk_widget_show_all( item );

    return item;
}



/* --------------------------------------------------------------- */
/* --------------------------------------------------------------- */
/* --------------------------------------------------------------- */
/* --------------------------------------------------------------- */


static void ink_toggle_action_class_init( InkToggleActionClass* klass );
static void ink_toggle_action_init( InkToggleAction* action );
static void ink_toggle_action_finalize( GObject* obj );
static void ink_toggle_action_get_property( GObject* obj, guint propId, GValue* value, GParamSpec * pspec );
static void ink_toggle_action_set_property( GObject* obj, guint propId, const GValue *value, GParamSpec* pspec );

static GtkWidget* ink_toggle_action_create_menu_item( GtkAction* action );
static GtkWidget* ink_toggle_action_create_tool_item( GtkAction* action );

static void ink_toggle_action_update_icon( InkToggleAction* action );

static GtkToggleActionClass* gInkToggleActionParentClass = 0;

struct _InkToggleActionPrivate
{
    gchar* iconId;
    Inkscape::IconSize iconSize;
};

#define INK_TOGGLE_ACTION_GET_PRIVATE( o ) ( G_TYPE_INSTANCE_GET_PRIVATE( (o), INK_TOGGLE_ACTION_TYPE, InkToggleActionPrivate ) )

GType ink_toggle_action_get_type( void )
{
    static GType myType = 0;
    if ( !myType ) {
        static const GTypeInfo myInfo = {
            sizeof( InkToggleActionClass ),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)ink_toggle_action_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof( InkToggleAction ),
            0, /* n_preallocs */
            (GInstanceInitFunc)ink_toggle_action_init,
            NULL
        };

        myType = g_type_register_static( GTK_TYPE_TOGGLE_ACTION, "InkToggleAction", &myInfo, (GTypeFlags)0 );
    }

    return myType;
}


static void ink_toggle_action_class_init( InkToggleActionClass* klass )
{
    if ( klass ) {
        gInkToggleActionParentClass = GTK_TOGGLE_ACTION_CLASS( g_type_class_peek_parent( klass ) );
        GObjectClass * objClass = G_OBJECT_CLASS( klass );

        objClass->finalize = ink_toggle_action_finalize;
        objClass->get_property = ink_toggle_action_get_property;
        objClass->set_property = ink_toggle_action_set_property;

        klass->parent_class.parent_class.create_menu_item = ink_toggle_action_create_menu_item;
        klass->parent_class.parent_class.create_tool_item = ink_toggle_action_create_tool_item;
        /*klass->parent_class.connect_proxy = connect_proxy;*/
        /*klass->parent_class.disconnect_proxy = disconnect_proxy;*/

        g_object_class_install_property( objClass,
                                         PROP_INK_ID,
                                         g_param_spec_string( "iconId",
                                                              "Icon ID",
                                                              "The id for the icon",
                                                              "",
                                                              (GParamFlags)(G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT) ) );

        g_object_class_install_property( objClass,
                                         PROP_INK_SIZE,
                                         g_param_spec_int( "iconSize",
                                                           "Icon Size",
                                                           "The size the icon",
                                                           (int)Inkscape::ICON_SIZE_MENU,
                                                           (int)99,
                                                           (int)Inkscape::ICON_SIZE_SMALL_TOOLBAR,
                                                           (GParamFlags)(G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT) ) );

        g_type_class_add_private( klass, sizeof(InkToggleActionClass) );
    }
}

static void ink_toggle_action_init( InkToggleAction* action )
{
    action->private_data = INK_TOGGLE_ACTION_GET_PRIVATE( action );
    action->private_data->iconId = 0;
    action->private_data->iconSize = Inkscape::ICON_SIZE_SMALL_TOOLBAR;
}

static void ink_toggle_action_finalize( GObject* obj )
{
    InkToggleAction* action = INK_TOGGLE_ACTION( obj );

    g_free( action->private_data->iconId );
    g_free( action->private_data );

}

InkToggleAction* ink_toggle_action_new( const gchar *name,
                           const gchar *label,
                           const gchar *tooltip,
                           const gchar *inkId,
                           Inkscape::IconSize size,
                           SPAttributeEnum attr)
{
    GObject* obj = (GObject*)g_object_new( INK_TOGGLE_ACTION_TYPE,
                                           "name", name,
                                           "label", label,
                                           "tooltip", tooltip,
                                           "iconId", inkId,
                                           "iconSize", Inkscape::getRegisteredIconSize(size),
                                           //"SP_ATTR_INKSCAPE", attr, // Why doesn't this work and do I need to use g_object_set_data below?
                                           NULL );

    g_object_set_data(obj, "SP_ATTR_INKSCAPE", GINT_TO_POINTER(attr));
    InkToggleAction* action = INK_TOGGLE_ACTION( obj );

    return action;
}

static void ink_toggle_action_get_property( GObject* obj, guint propId, GValue* value, GParamSpec * pspec )
{
    InkToggleAction* action = INK_TOGGLE_ACTION( obj );
    (void)action;
    switch ( propId ) {
        case PROP_INK_ID:
        {
            g_value_set_string( value, action->private_data->iconId );
        }
        break;

        case PROP_INK_SIZE:
        {
            g_value_set_int( value, action->private_data->iconSize );
        }
        break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID( obj, propId, pspec );
    }
}

void ink_toggle_action_set_property( GObject* obj, guint propId, const GValue *value, GParamSpec* pspec )
{
    InkToggleAction* action = INK_TOGGLE_ACTION( obj );
    (void)action;
    switch ( propId ) {
        case PROP_INK_ID:
        {
            gchar* tmp = action->private_data->iconId;
            action->private_data->iconId = g_value_dup_string( value );
            g_free( tmp );

            ink_toggle_action_update_icon( action );
        }
        break;

        case PROP_INK_SIZE:
        {
            action->private_data->iconSize = (Inkscape::IconSize)g_value_get_int( value );
        }
        break;

        default:
        {
            G_OBJECT_WARN_INVALID_PROPERTY_ID( obj, propId, pspec );
        }
    }
}

static GtkWidget* ink_toggle_action_create_menu_item( GtkAction* action )
{
    GtkWidget* item = gInkToggleActionParentClass->parent_class.create_menu_item(action);

    return item;
}

static GtkWidget* ink_toggle_action_create_tool_item( GtkAction* action )
{
    InkToggleAction* act = INK_TOGGLE_ACTION( action );

    GtkWidget* item = gInkToggleActionParentClass->parent_class.create_tool_item(action);
    if ( GTK_IS_TOOL_BUTTON(item) ) {
        GtkToolButton* button = GTK_TOOL_BUTTON(item);
        if ( act->private_data->iconId ) {
            GtkWidget* child = sp_icon_new( act->private_data->iconSize, act->private_data->iconId );
            GtkWidget* align = gtk_alignment_new( 0.5, 0.5, 0.0, 0.0 );
            gtk_container_add( GTK_CONTAINER(align), child );
            gtk_tool_button_set_icon_widget( button, align );
        } else {
            gchar *label;
            g_object_get (G_OBJECT(action), "short_label", &label, NULL);
            gtk_tool_button_set_label( button, label );
        }
    } else {
        // For now trigger a warning but don't do anything else
        GtkToolButton* button = GTK_TOOL_BUTTON(item);
        (void)button;
    }
    gtk_widget_show_all( item );

    return item;
}


static void ink_toggle_action_update_icon( InkToggleAction* action )
{
    if ( action ) {
        GSList* proxies = gtk_action_get_proxies( GTK_ACTION(action) );
        while ( proxies ) {
            if ( GTK_IS_TOOL_ITEM(proxies->data) ) {
                if ( GTK_IS_TOOL_BUTTON(proxies->data) ) {
                    GtkToolButton* button = GTK_TOOL_BUTTON(proxies->data);

                    GtkWidget* child = sp_icon_new( action->private_data->iconSize, action->private_data->iconId );
                    GtkWidget* align = gtk_alignment_new( 0.5, 0.5, 0.0, 0.0 );
                    gtk_container_add( GTK_CONTAINER(align), child );
                    gtk_widget_show_all( align );
                    gtk_tool_button_set_icon_widget( button, align );
                }
            }

            proxies = g_slist_next( proxies );
        }
    }
}


/* --------------------------------------------------------------- */
/* --------------------------------------------------------------- */
/* --------------------------------------------------------------- */
/* --------------------------------------------------------------- */


static void ink_radio_action_class_init( InkRadioActionClass* klass );
static void ink_radio_action_init( InkRadioAction* action );
static void ink_radio_action_finalize( GObject* obj );
static void ink_radio_action_get_property( GObject* obj, guint propId, GValue* value, GParamSpec * pspec );
static void ink_radio_action_set_property( GObject* obj, guint propId, const GValue *value, GParamSpec* pspec );

static GtkWidget* ink_radio_action_create_menu_item( GtkAction* action );
static GtkWidget* ink_radio_action_create_tool_item( GtkAction* action );

static GtkRadioActionClass* gInkRadioActionParentClass = 0;

struct _InkRadioActionPrivate
{
    gchar* iconId;
    Inkscape::IconSize iconSize;
};

#define INK_RADIO_ACTION_GET_PRIVATE( o ) ( G_TYPE_INSTANCE_GET_PRIVATE( (o), INK_RADIO_ACTION_TYPE, InkRadioActionPrivate ) )

GType ink_radio_action_get_type( void )
{
    static GType myType = 0;
    if ( !myType ) {
        static const GTypeInfo myInfo = {
            sizeof( InkRadioActionClass ),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)ink_radio_action_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof( InkRadioAction ),
            0, /* n_preallocs */
            (GInstanceInitFunc)ink_radio_action_init,
            NULL
        };

        myType = g_type_register_static( GTK_TYPE_RADIO_ACTION, "InkRadioAction", &myInfo, (GTypeFlags)0 );
    }

    return myType;
}


static void ink_radio_action_class_init( InkRadioActionClass* klass )
{
    if ( klass ) {
        gInkRadioActionParentClass = GTK_RADIO_ACTION_CLASS( g_type_class_peek_parent( klass ) );
        GObjectClass * objClass = G_OBJECT_CLASS( klass );

        objClass->finalize = ink_radio_action_finalize;
        objClass->get_property = ink_radio_action_get_property;
        objClass->set_property = ink_radio_action_set_property;

        klass->parent_class.parent_class.parent_class.create_menu_item = ink_radio_action_create_menu_item;
        klass->parent_class.parent_class.parent_class.create_tool_item = ink_radio_action_create_tool_item;
        /*klass->parent_class.connect_proxy = connect_proxy;*/
        /*klass->parent_class.disconnect_proxy = disconnect_proxy;*/

        g_object_class_install_property( objClass,
                                         PROP_INK_ID,
                                         g_param_spec_string( "iconId",
                                                              "Icon ID",
                                                              "The id for the icon",
                                                              "",
                                                              (GParamFlags)(G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT) ) );

        g_object_class_install_property( objClass,
                                         PROP_INK_SIZE,
                                         g_param_spec_int( "iconSize",
                                                           "Icon Size",
                                                           "The size the icon",
                                                           (int)Inkscape::ICON_SIZE_MENU,
                                                           (int)Inkscape::ICON_SIZE_DECORATION,
                                                           (int)Inkscape::ICON_SIZE_SMALL_TOOLBAR,
                                                           (GParamFlags)(G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT) ) );

        g_type_class_add_private( klass, sizeof(InkRadioActionClass) );
    }
}

static void ink_radio_action_init( InkRadioAction* action )
{
    action->private_data = INK_RADIO_ACTION_GET_PRIVATE( action );
    action->private_data->iconId = 0;
    action->private_data->iconSize = Inkscape::ICON_SIZE_SMALL_TOOLBAR;
}

static void ink_radio_action_finalize( GObject* obj )
{
    InkRadioAction* action = INK_RADIO_ACTION( obj );

    g_free( action->private_data->iconId );
    g_free( action->private_data );

}

InkRadioAction* ink_radio_action_new( const gchar *name,
                           const gchar *label,
                           const gchar *tooltip,
                           const gchar *inkId,
                           Inkscape::IconSize size )
{
    GObject* obj = (GObject*)g_object_new( INK_RADIO_ACTION_TYPE,
                                           "name", name,
                                           "label", label,
                                           "tooltip", tooltip,
                                           "iconId", inkId,
                                           "iconSize", Inkscape::getRegisteredIconSize(size),
                                           NULL );

    InkRadioAction* action = INK_RADIO_ACTION( obj );

    return action;
}

static void ink_radio_action_get_property( GObject* obj, guint propId, GValue* value, GParamSpec * pspec )
{
    InkRadioAction* action = INK_RADIO_ACTION( obj );
    (void)action;
    switch ( propId ) {
        case PROP_INK_ID:
        {
            g_value_set_string( value, action->private_data->iconId );
        }
        break;

        case PROP_INK_SIZE:
        {
            g_value_set_int( value, action->private_data->iconSize );
        }
        break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID( obj, propId, pspec );
    }
}

void ink_radio_action_set_property( GObject* obj, guint propId, const GValue *value, GParamSpec* pspec )
{
    InkRadioAction* action = INK_RADIO_ACTION( obj );
    (void)action;
    switch ( propId ) {
        case PROP_INK_ID:
        {
            gchar* tmp = action->private_data->iconId;
            action->private_data->iconId = g_value_dup_string( value );
            g_free( tmp );
        }
        break;

        case PROP_INK_SIZE:
        {
            action->private_data->iconSize = (Inkscape::IconSize)g_value_get_int( value );
        }
        break;

        default:
        {
            G_OBJECT_WARN_INVALID_PROPERTY_ID( obj, propId, pspec );
        }
    }
}

static GtkWidget* ink_radio_action_create_menu_item( GtkAction* action )
{
    GtkWidget* item = gInkRadioActionParentClass->parent_class.parent_class.create_menu_item(action);

    return item;
}

static GtkWidget* ink_radio_action_create_tool_item( GtkAction* action )
{
    InkRadioAction* act = INK_RADIO_ACTION( action );
    GtkWidget* item = gInkRadioActionParentClass->parent_class.parent_class.create_tool_item(action);

    if ( act->private_data->iconId ) {
        if ( GTK_IS_TOOL_BUTTON(item) ) {
            GtkToolButton* button = GTK_TOOL_BUTTON(item);

            GtkWidget* child = sp_icon_new( act->private_data->iconSize, act->private_data->iconId );
            GtkWidget* align = gtk_alignment_new( 0.5, 0.5, 0.0, 0.0 );
            gtk_container_add( GTK_CONTAINER(align), child );
            gtk_tool_button_set_icon_widget( button, align );
        } else {
            // For now trigger a warning but don't do anything else
            GtkToolButton* button = GTK_TOOL_BUTTON(item);
            (void)button;
        }
    }

    // TODO investigate if needed
    gtk_widget_show_all( item );

    return item;
}

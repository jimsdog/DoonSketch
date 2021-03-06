
#ifndef __sp_marshal_MARSHAL_H__
#define __sp_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* NONE:POINTER,UINT (../../src/helper/sp-marshal.list:2) */
extern void sp_marshal_VOID__POINTER_UINT (GClosure     *closure,
                                           GValue       *return_value,
                                           guint         n_param_values,
                                           const GValue *param_values,
                                           gpointer      invocation_hint,
                                           gpointer      marshal_data);
#define sp_marshal_NONE__POINTER_UINT	sp_marshal_VOID__POINTER_UINT

/* BOOLEAN:POINTER (../../src/helper/sp-marshal.list:3) */
extern void sp_marshal_BOOLEAN__POINTER (GClosure     *closure,
                                         GValue       *return_value,
                                         guint         n_param_values,
                                         const GValue *param_values,
                                         gpointer      invocation_hint,
                                         gpointer      marshal_data);

/* BOOLEAN:POINTER,UINT (../../src/helper/sp-marshal.list:4) */
extern void sp_marshal_BOOLEAN__POINTER_UINT (GClosure     *closure,
                                              GValue       *return_value,
                                              guint         n_param_values,
                                              const GValue *param_values,
                                              gpointer      invocation_hint,
                                              gpointer      marshal_data);

/* BOOLEAN:POINTER,POINTER (../../src/helper/sp-marshal.list:5) */
extern void sp_marshal_BOOLEAN__POINTER_POINTER (GClosure     *closure,
                                                 GValue       *return_value,
                                                 guint         n_param_values,
                                                 const GValue *param_values,
                                                 gpointer      invocation_hint,
                                                 gpointer      marshal_data);

/* INT:POINTER,POINTER (../../src/helper/sp-marshal.list:6) */
extern void sp_marshal_INT__POINTER_POINTER (GClosure     *closure,
                                             GValue       *return_value,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      invocation_hint,
                                             gpointer      marshal_data);

/* DOUBLE:POINTER,UINT (../../src/helper/sp-marshal.list:7) */
extern void sp_marshal_DOUBLE__POINTER_UINT (GClosure     *closure,
                                             GValue       *return_value,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      invocation_hint,
                                             gpointer      marshal_data);

G_END_DECLS

#endif /* __sp_marshal_MARSHAL_H__ */


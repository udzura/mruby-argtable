/*
** mrb_argtable.c - Argtable class
**
** Copyright (c) Uchio Kondo
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/array.h"
#include "mruby/data.h"
#include "mruby/string.h"
#include "mruby/hash.h"
#include "mruby/variable.h"
#include "mruby/error.h"
#include "mruby/value.h"
#include "mrb_argtable.h"
#include "argtable2.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DONE mrb_gc_arena_restore(mrb, 0);

#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"

typedef struct mrb_argtable_data {
  void **argtable;
  struct arg_end *argend;
  mrb_value arg_definitions;
  bool show_error;
} mrb_argtable_data;

void mrb_argtable_free(mrb_state *mrb, void *p)
{
  mrb_argtable_data *argtable_s = (mrb_argtable_data *)p;
  arg_freetable(argtable_s->argtable, sizeof(argtable_s->argtable)/sizeof(argtable_s->argtable[0]));
  mrb_free(mrb, argtable_s);
}

#define MRB_ARGTABLE_DATA_TYPE(argtable_s)                               \
  typedef struct argtable_s ## _data {                                   \
    struct argtable_s *definition;                                       \
  } argtable_s;                                                          \
  static const struct mrb_data_type mrb_ ## argtable_s ## _data_type = { \
    #argtable_s, mrb_free,                                               \
  }

MRB_ARGTABLE_DATA_TYPE(arg_lit);
MRB_ARGTABLE_DATA_TYPE(arg_int);
MRB_ARGTABLE_DATA_TYPE(arg_dbl);
MRB_ARGTABLE_DATA_TYPE(arg_str);
/* MRB_ARGTABLE_DATA_TYPE(arg_rex);
   MRB_ARGTABLE_DATA_TYPE(arg_date);
   MRB_ARGTABLE_DATA_TYPE(arg_rem);
 */

#define MRB_ARGTABLE_DATA_TYPE_OF(argtable_s) mrb_ ## argtable_s ## _data_type

#define ARG_MAX 1024

static const struct mrb_data_type mrb_argtable_data_type = {
  "mrb_argtable_data", mrb_argtable_free,
};

static mrb_value mrb_arg_lit_init(mrb_state *mrb, mrb_value self)
{
  arg_lit *data;
  char *shortopts, *longopts, *glossary;

  data = (arg_lit *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &MRB_ARGTABLE_DATA_TYPE_OF(arg_lit);
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "zz!z", &shortopts, &longopts, &glossary);
  data = (arg_lit *)mrb_malloc(mrb, sizeof(arg_lit));
  data->definition = arg_lit0(shortopts, longopts, glossary);
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_arg_lit_count(mrb_state *mrb, mrb_value self)
{
  arg_lit *data;
  data = (arg_lit *)DATA_PTR(self);

  return mrb_fixnum_value(data->definition->count);
}

static mrb_value mrb_arg_int_init(mrb_state *mrb, mrb_value self)
{
  arg_int *data;
  char *shortopts, *longopts, *datatype, *glossary;

  data = (arg_int *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &MRB_ARGTABLE_DATA_TYPE_OF(arg_int);
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "z!z!z!z!", &shortopts, &longopts, &datatype, &glossary);
  data = (arg_int *)mrb_malloc(mrb, sizeof(arg_int));
  data->definition = arg_int0(shortopts, longopts, datatype, glossary);
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_arg_int_val(mrb_state *mrb, mrb_value self)
{
  arg_int *data;
  mrb_int idx = 0;
  data = (arg_int *)DATA_PTR(self);
  mrb_get_args(mrb, "|i", &idx);
  if(idx >= data->definition->count) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid index");
  }

  return mrb_fixnum_value(data->definition->ival[idx]);
}

static mrb_value mrb_arg_int_count(mrb_state *mrb, mrb_value self)
{
  arg_int *data = (arg_int *)DATA_PTR(self);
  return mrb_fixnum_value(data->definition->count);
}

static mrb_value mrb_arg_dbl_init(mrb_state *mrb, mrb_value self)
{
  arg_dbl *data;
  char *shortopts, *longopts, *datatype, *glossary;

  data = (arg_dbl *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &MRB_ARGTABLE_DATA_TYPE_OF(arg_dbl);
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "z!z!z!z!", &shortopts, &longopts, &datatype, &glossary);
  data = (arg_dbl *)mrb_malloc(mrb, sizeof(arg_dbl));
  data->definition = arg_dbl0(shortopts, longopts, datatype, glossary);
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_arg_dbl_val(mrb_state *mrb, mrb_value self)
{
  arg_dbl *data;
  mrb_int idx = 0;
  data = (arg_dbl *)DATA_PTR(self);
  mrb_get_args(mrb, "|i", &idx);
  if(idx >= data->definition->count) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid index");
  }

  return mrb_float_value(mrb, (double)data->definition->dval[idx]);
}

static mrb_value mrb_arg_dbl_count(mrb_state *mrb, mrb_value self)
{
  arg_dbl *data = (arg_dbl *)DATA_PTR(self);
  return mrb_fixnum_value(data->definition->count);
}

static mrb_value mrb_arg_str_init(mrb_state *mrb, mrb_value self)
{
  arg_str *data;
  char *shortopts, *longopts, *datatype, *glossary;
  int catchall_maxsize = 0;

  data = (arg_str *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &MRB_ARGTABLE_DATA_TYPE_OF(arg_str);
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "z!z!z!z!|i", &shortopts, &longopts, &datatype, &glossary, &catchall_maxsize);
  data = (arg_str *)mrb_malloc(mrb, sizeof(arg_str));

  if(shortopts || longopts) {
    data->definition = arg_str0(shortopts, longopts, datatype, glossary);
  } else {
    if(catchall_maxsize != 0) {
      data->definition = arg_strn(NULL, NULL, datatype, 0, catchall_maxsize, glossary);
    } else {
      data->definition = arg_str1(NULL, NULL, datatype, glossary);
    }
  }
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_arg_str_val(mrb_state *mrb, mrb_value self)
{
  arg_str *data;
  mrb_int idx = 0;
  data = (arg_str *)DATA_PTR(self);
  mrb_get_args(mrb, "|i", &idx);
  if(idx >= data->definition->count) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid index");
  }

  return mrb_str_new_cstr(mrb, data->definition->sval[idx]);
}

static mrb_value mrb_arg_str_count(mrb_state *mrb, mrb_value self)
{
  arg_str *data = (arg_str *)DATA_PTR(self);
  return mrb_fixnum_value(data->definition->count);
}

static mrb_value mrb_arg_str_values(mrb_state *mrb, mrb_value self)
{
  arg_str *data;
  data = (arg_str *)DATA_PTR(self);
  mrb_value values = mrb_ary_new(mrb);
  for(int i = 0; i < data->definition->count; ++i) {
    mrb_ary_push(mrb, values, mrb_str_new_cstr(mrb, data->definition->sval[i]));
  }

  return values;
}

static mrb_value mrb_argtable_init(mrb_state *mrb, mrb_value self)
{
  mrb_argtable_data *data;

  data = (mrb_argtable_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_argtable_data_type;
  DATA_PTR(self) = NULL;

  data = (mrb_argtable_data *)mrb_malloc(mrb, sizeof(mrb_argtable_data));
  data->arg_definitions = mrb_ary_new(mrb);
  data->argtable = NULL;
  data->show_error = true; /* This will be updated */
  DATA_PTR(self) = data;

  mrb_value table = mrb_hash_new(mrb);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@table"), table);

  mrb_value options = mrb_ary_new(mrb);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@options"), options);

  return self;
}

static mrb_value mrb_argtable_push(mrb_state *mrb, mrb_value self)
{
  mrb_argtable_data *data;
  mrb_value definition;

  mrb_get_args(mrb, "o", &definition);

  data = (mrb_argtable_data *)DATA_PTR(self);
  mrb_ary_push(mrb, data->arg_definitions, definition);

  return definition;
}

static mrb_value mrb_argtable_compile(mrb_state *mrb, mrb_value self)
{
  mrb_argtable_data *data;
  mrb_value defs;
  void **argtable;
  struct RClass *root = mrb_class_get(mrb, "Argtable");

  data = (mrb_argtable_data *)DATA_PTR(self);
  defs = data->arg_definitions;

  if ( ! mrb_array_p(defs) ) {
    mrb_sys_fail(mrb, "Something is wrong");
    return mrb_fixnum_value(-1);
  }

  int len = RARRAY_LEN(defs);
  argtable = (void **)mrb_malloc(mrb, sizeof(arg_lit)*(len + 1));
  for ( int i = 0; i < len; ++i ) {
    mrb_value obj = mrb_ary_ref(mrb, defs, i);
    if (mrb_obj_is_kind_of(mrb, obj, mrb_class_get_under(mrb, root, "Literal"))){
      arg_lit *d = (arg_lit *)DATA_PTR(obj);
      argtable[i] = d->definition;
    } else if (mrb_obj_is_kind_of(mrb, obj, mrb_class_get_under(mrb, root, "Integer"))){
      arg_int *d = (arg_int *)DATA_PTR(obj);
      argtable[i] = d->definition;
    } else if (mrb_obj_is_kind_of(mrb, obj, mrb_class_get_under(mrb, root, "Double"))){
      arg_dbl *d = (arg_dbl *)DATA_PTR(obj);
      argtable[i] = d->definition;
    } else if (mrb_obj_is_kind_of(mrb, obj, mrb_class_get_under(mrb, root, "String"))){
      arg_str *d = (arg_str *)DATA_PTR(obj);
      argtable[i] = d->definition;
    } else {
      mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid definition type.");
    }
  }

  argtable[len] = arg_end(20);
  data->argend = argtable[len];
  data->argtable = argtable;

  return mrb_fixnum_value(len);
}

static mrb_value mrb_argtable_parse(mrb_state *mrb, mrb_value self)
{
  mrb_argtable_data *data;
  mrb_int argc;
  mrb_value *argv;

  data = (mrb_argtable_data *)DATA_PTR(self);
  if(!data->argtable) {
    mrb_funcall(mrb, self, "compile", 0);
  }

  mrb_get_args(mrb, "a", &argv, &argc);

  char **argv_ = (char **)mrb_malloc(mrb, sizeof(char *) * (argc + 1));
  for(int i = 0; i < argc; i++) {
    mrb_value strv = mrb_convert_type(mrb, argv[i], MRB_TT_STRING, "String", "to_str");
    char *buf = (char *)mrb_string_value_cstr(mrb, &strv);
    argv_[i] = buf;
  }
  argv_[argc] = NULL;

  int nerrors = arg_parse(argc, argv_, data->argtable);

  if (nerrors != 0) {
    if(data->show_error) {
      arg_print_errors(stderr, data->argend, argv_[0]);
    }
  }

  return mrb_fixnum_value(nerrors);
}

static mrb_value mrb_argtable_syntax(mrb_state *mrb, mrb_value self)
{
  mrb_argtable_data *data;
  mrb_bool is_verbose = false;
  mrb_get_args(mrb, "|b", &is_verbose);

  data = (mrb_argtable_data *)DATA_PTR(self);
  if(!data->argtable) {
    mrb_funcall(mrb, self, "compile", 0);
  }

  if (is_verbose) {
    arg_print_syntaxv(stderr, data->argtable, " ");
  } else {
    arg_print_syntax(stderr, data->argtable, " ");
  }

  return mrb_nil_value();
}

static mrb_value mrb_argtable_glossary(mrb_state *mrb, mrb_value self)
{
  mrb_argtable_data *data = (mrb_argtable_data *)DATA_PTR(self);
  if(!data->argtable) {
    mrb_funcall(mrb, self, "compile", 0);
  }
  arg_print_glossary(stderr, data->argtable, " %-25s %s\n");

  return mrb_nil_value();
}

void mrb_mruby_argtable_gem_init(mrb_state *mrb)
{
  struct RClass *argtable,
    *arg_lit_c, *arg_int_c, *arg_dbl_c, *arg_str_c;
  argtable = mrb_define_class(mrb, "Argtable", mrb->object_class);
  mrb_define_method(mrb, argtable, "initialize", mrb_argtable_init, MRB_ARGS_NONE());
  mrb_define_method(mrb, argtable, "__push__",   mrb_argtable_push, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, argtable, "compile",    mrb_argtable_compile, MRB_ARGS_NONE());
  mrb_define_method(mrb, argtable, "parse",      mrb_argtable_parse, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, argtable, "help",       mrb_argtable_syntax, MRB_ARGS_ARG(0, 1));
  mrb_define_method(mrb, argtable, "glossary",   mrb_argtable_glossary, MRB_ARGS_NONE());

  arg_lit_c = mrb_define_class_under(mrb, argtable, "Literal", mrb->object_class);
  mrb_define_method(mrb, arg_lit_c, "initialize", mrb_arg_lit_init,  MRB_ARGS_REQ(3));
  mrb_define_method(mrb, arg_lit_c, "count",      mrb_arg_lit_count, MRB_ARGS_NONE());

  arg_int_c = mrb_define_class_under(mrb, argtable, "Integer", mrb->object_class);
  mrb_define_method(mrb, arg_int_c, "initialize", mrb_arg_int_init, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, arg_int_c, "value",      mrb_arg_int_val,  MRB_ARGS_ARG(0, 1));
  mrb_define_method(mrb, arg_int_c, "count",      mrb_arg_int_count, MRB_ARGS_NONE());

  arg_dbl_c = mrb_define_class_under(mrb, argtable, "Double", mrb->object_class);
  mrb_define_method(mrb, arg_dbl_c, "initialize", mrb_arg_dbl_init, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, arg_dbl_c, "value",      mrb_arg_dbl_val,  MRB_ARGS_ARG(0, 1));
  mrb_define_method(mrb, arg_dbl_c, "count",      mrb_arg_dbl_count, MRB_ARGS_NONE());

  arg_str_c = mrb_define_class_under(mrb, argtable, "String", mrb->object_class);
  mrb_define_method(mrb, arg_str_c, "initialize", mrb_arg_str_init, MRB_ARGS_ARG(4, 1));
  mrb_define_method(mrb, arg_str_c, "value",      mrb_arg_str_val,  MRB_ARGS_ARG(0, 1));
  mrb_define_method(mrb, arg_str_c, "count",      mrb_arg_str_count, MRB_ARGS_NONE());
  mrb_define_method(mrb, arg_str_c, "values",     mrb_arg_str_values,  MRB_ARGS_NONE());

  DONE;
}

void mrb_mruby_argtable_gem_final(mrb_state *mrb)
{
}

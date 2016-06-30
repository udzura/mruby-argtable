/*
** mrb_argtable.c - Argtable class
**
** Copyright (c) ["Uchio Kondo", "Stewart Heitmann"] 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/array.h"
#include "mruby/data.h"
#include "mrb_argtable.h"
#include "argtable3.h"

#include <stdio.h>
#include <string.h>

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct mrb_argtable_data {
  void **argtable;
  mrb_value arg_definitions;
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
  DATA_PTR(self) = data;

  return self;
}

/* argtable sample */

static mrb_value mrb_argtable_hi(mrb_state *mrb, mrb_value self)
{
  struct arg_lit *a = arg_lit0("a", "a-value", "The a value");
  struct arg_end *end = arg_end(20);
  void *argtable[] = {a, end};

  char *argv[3];
  argv[0] = strdup("prog");
  argv[1] = strdup("-a");
  argv[2] = NULL;
  int nerrors = arg_parse(2, argv, argtable);
  if (nerrors == 0) {
    printf("-a = %d\n", a->count);
  } else {
    arg_print_errors(stdout,end,"prog");
  }

  arg_freetable(argtable, sizeof(argtable)/sizeof(argtable[0]));

  return mrb_fixnum_value(nerrors);
}

static mrb_value mrb_argtable_help(mrb_state *mrb, mrb_value self)
{
  struct arg_lit *a = arg_lit0("a", "a-value", "The a value");
  struct arg_end *end = arg_end(20);
  void *argtable[] = {a, end};

  printf("-- syntax\n");
  arg_print_syntax(stderr, argtable, " ");

  printf("\n-- glossary\n");
  arg_print_glossary(stderr, argtable, " %-25s %s\n");

  return mrb_nil_value();
}

void mrb_mruby_argtable_gem_init(mrb_state *mrb)
{
  struct RClass *argtable,
    *arg_lit_c /* , *arg_int_c, *arg_dbl_c, *arg_str_c */;
    argtable = mrb_define_class(mrb, "Argtable", mrb->object_class);
    mrb_define_method(mrb, argtable, "initialize", mrb_argtable_init, MRB_ARGS_NONE());

    /* samples... */
    mrb_define_class_method(mrb, argtable, "sample", mrb_argtable_hi, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, argtable, "help", mrb_argtable_help, MRB_ARGS_NONE());

    arg_lit_c = mrb_define_class_under(mrb, argtable, "Literal", mrb->object_class);
    mrb_define_method(mrb, arg_lit_c, "initialize", mrb_arg_lit_init,  MRB_ARGS_REQ(3));
    mrb_define_method(mrb, arg_lit_c, "count",      mrb_arg_lit_count, MRB_ARGS_NONE());

    DONE;
}

void mrb_mruby_argtable_gem_final(mrb_state *mrb)
{
}

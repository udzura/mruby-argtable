/*
** mrb_argtable.c - Argtable class
**
** Copyright (c) ["Uchio Kondo", "Stewart Heitmann"] 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_argtable.h"
#include "argtable3.h"

#include <stdio.h>
#include <string.h>

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  char *str;
  int len;
} mrb_argtable_data;

static const struct mrb_data_type mrb_argtable_data_type = {
  "mrb_argtable_data", mrb_free,
};

static mrb_value mrb_argtable_init(mrb_state *mrb, mrb_value self)
{
  mrb_argtable_data *data;
  char *str;
  int len;

  data = (mrb_argtable_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_argtable_data_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "s", &str, &len);
  data = (mrb_argtable_data *)mrb_malloc(mrb, sizeof(mrb_argtable_data));
  data->str = str;
  data->len = len;
  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_argtable_hello(mrb_state *mrb, mrb_value self)
{
  mrb_argtable_data *data = DATA_PTR(self);

  return mrb_str_new(mrb, data->str, data->len);
}

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
    struct RClass *argtable;
    argtable = mrb_define_class(mrb, "Argtable", mrb->object_class);
    mrb_define_method(mrb, argtable, "initialize", mrb_argtable_init, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, argtable, "hello", mrb_argtable_hello, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, argtable, "sample", mrb_argtable_hi, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, argtable, "help", mrb_argtable_help, MRB_ARGS_NONE());
    DONE;
}

void mrb_mruby_argtable_gem_final(mrb_state *mrb)
{
}

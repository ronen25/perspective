/******************************************************************************
 *
 * Copyright (c) 2017, the Perspective Authors.
 *
 * This file is part of the Perspective library, distributed under the terms of
 * the Apache License 2.0.  The full license can be found in the LICENSE file.
 *
 */


#include <perspective/config.h>
#include <perspective/table.h>
#include <perspective/test_utils.h>
#include <perspective/context_one.h>
#include <gtest/gtest.h>

using namespace perspective;

TEST(TABLE, simplest_test) {
    t_table tbl(t_schema({"a", "b"}, {DTYPE_INT64, DTYPE_FLOAT64}), 5);
    tbl.init();
    tbl.reserve(5);
}

TEST(SCALAR, scalar_literal_test) {
  auto s1 = 1_ts;
  auto s2 = "hello"_ts;
  auto s3 = 42.0_ts;
  ASSERT_EQ(s1, mktscalar<t_uint64>(1));
  ASSERT_EQ(s2, mktscalar<const char*>("hello"));
  ASSERT_EQ(s3, mktscalar<t_float64>(42.0));
}

TEST(CONTEXT_ONE, null_pivot_test) {
  t_schema sch{{"p", "a"}, {DTYPE_STR, DTYPE_INT64}};
  t_table tbl(sch, {{""_ns, 1_ts}, {"foo"_ts, 1_ts}});
  t_config cfg{{"p"}, {"sum_a", AGGTYPE_SUM, "a"}};
  auto ctx = do_pivot<t_ctx1>(t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
  ctx->get_table()->pprint();
}

TEST(TMP, tmp) {
  t_schema s{{"abc"}, {DTYPE_INT64}};
  std::raise(SIGINT);
}

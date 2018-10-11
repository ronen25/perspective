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
#include <perspective/node_processor.h>
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
  ASSERT_EQ(s1, mktscalar<t_int64>(1));
  ASSERT_EQ(s2, mktscalar<const char*>("hello"));
  ASSERT_EQ(s3, mktscalar<t_float64>(42.0));
}

TEST(CONTEXT_ONE, null_pivot_test_1) {
  t_schema sch{{"p", "a"}, {DTYPE_INT64, DTYPE_INT64}};
  t_table tbl(sch, {{1_ns, 1_ts}, {1_ts, 1_ts}});
  t_config cfg{{"p"}, {"sum_a", AGGTYPE_SUM, "a"}};
  auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
  auto ctx_tbl = ctx->get_table();
  auto got =  ctx_tbl->get_scalvec();
  t_tscalvec expected{2_ts, 1_ns, 1_ts, 1_ts, 1_ts, 1_ns};
  ASSERT_EQ(expected, got);
}

TEST(SCALAR, map_test_1) {
  typedef t_comparator<t_tscalar, DTYPE_STR> t_cmp;
  typedef std::map<t_tscalar, t_uindex, t_cmp> t_map;
  t_map mmap((t_cmp(nullptr)));
  mmap[""_ns] = 1;
  mmap["x"_ts] = 2;
  mmap["y"_ts] = 3;
  ASSERT_EQ(mmap.size(), 3);
}

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
#include <perspective/storage.h>
#include <gtest/gtest.h>

using namespace perspective;

t_tscalar bnull = mknull(DTYPE_BOOL);
t_tscalar snull = mknull(DTYPE_BOOL);
t_tscalar s_true = mktscalar<t_bool>(true);
t_tscalar s_false = mktscalar<t_bool>(false);


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
    auto got = ctx_tbl->get_scalvec();
    t_tscalvec expected{2_ts, 1_ns, 1_ts, 1_ts, 1_ts, 1_ns};
    ASSERT_EQ(expected, got);
}

TEST(STORAGE, constructor) {
  t_lstore s;
  ASSERT_EQ(s.get_init(), false);
  ASSERT_TRUE(s.empty()); 
}

TEST(STORAGE, uninited_get) {
  t_lstore s;
  ASSERT_EQ(nullptr, s.get_ptr(0));
}

TEST(STORAGE, extend_1) {
  t_lstore s;
  s.init();
  s.extend<t_int64>(10);
  ASSERT_EQ(s.size(), sizeof(t_int64) * 10);
}

// TODO add assert eqs here
TEST(CONTEXT_ONE, pivot_1) {
    t_schema sch{{"b", "s", "i"}, {DTYPE_BOOL, DTYPE_STR, DTYPE_INT64}};
    auto t = mktscalar<t_bool>(true);
    auto f = mktscalar<t_bool>(false);
    t_table tbl(
        sch, {{t, "a"_ts, 1_ts}, {f, "b"_ts, 2_ts}, {t, "c"_ts, 3_ts}, {f, "d"_ts, 4_ts}});
    t_config cfg{{"b", "s"}, {"sum_i", AGGTYPE_SUM, "i"}};
    auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
    auto ctx_tbl = ctx->get_table();
    auto got = ctx_tbl->get_scalvec();
    t_tscalvec expected{10_ts, bnull, snull, 4_ts, s_true, snull, 3_ts, bnull, "c"_ts, 1_ts,
        bnull, "a"_ts, 6_ts, s_false, snull, 4_ts, bnull, "d"_ts, 2_ts, bnull, "b"_ts};
    //ASSERT_EQ(expected, got);
}

TEST(CONTEXT_ONE, pivot_2) {
    t_schema sch{{"i", "s", "b"}, {DTYPE_BOOL, DTYPE_STR, DTYPE_INT64}};
    auto t = mktscalar<t_bool>(true);
    auto f = mktscalar<t_bool>(false);
    t_table tbl(
        sch, {{t, "a"_ts, 1_ts}, {f, "b"_ts, 2_ts}, {t, "c"_ts, 3_ts}, {f, "d"_ts, 4_ts}});
    t_config cfg{{"b", "b", "i", "s"}, {"sum_i", AGGTYPE_SUM, "i"}};
    auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
    auto ctx_tbl = ctx->get_table();
    // auto got =  ctx_tbl->get_scalvec();
    // t_tscalvec expected{2_ts, 1_ns, 1_ts, 1_ts, 1_ts, 1_ns};
    // ASSERT_EQ(expected, got);
}

TEST(CONTEXT_ONE, pivot_3) {
    t_schema sch{{"b", "s", "i"}, {DTYPE_BOOL, DTYPE_STR, DTYPE_INT64}};
    auto t = mktscalar<t_bool>(true);
    auto f = mktscalar<t_bool>(false);
    t_table tbl(
        sch, {{t, "a"_ts, 1_ts}, {f, "b"_ts, 2_ts}, {t, "c"_ts, 3_ts}, {f, "d"_ts, 4_ts}});
    t_config cfg{{"b", "i"}, {"sum_i", AGGTYPE_SUM, "i"}};
    auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
    auto ctx_tbl = ctx->get_table();
    // auto got =  ctx_tbl->get_scalvec();
    // t_tscalvec expected{2_ts, 1_ns, 1_ts, 1_ts, 1_ts, 1_ns};
    // ASSERT_EQ(expected, got);
}

TEST(CONTEXT_ONE, pivot_4) {
    t_schema sch{{"b", "s", "i"}, {DTYPE_BOOL, DTYPE_STR, DTYPE_INT64}};
    auto t = mktscalar<t_bool>(true);
    auto f = mktscalar<t_bool>(false);
    t_table tbl(
        sch, {{t, "a"_ts, 1_ts}, {f, "b"_ts, 2_ts}, {t, "c"_ts, 3_ts}, {f, "d"_ts, 4_ts}});
    t_config cfg{{"i", "s"}, {"sum_i", AGGTYPE_SUM, "i"}};
    auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
    auto ctx_tbl = ctx->get_table();
    // auto got =  ctx_tbl->get_scalvec();
    // t_tscalvec expected{2_ts, 1_ns, 1_ts, 1_ts, 1_ts, 1_ns};
    // ASSERT_EQ(expected, got);
}

TEST(SCALAR, map_test_1) {
#define TEST_BODY(DTYPE, CTYPE)                                                                \
    {                                                                                          \
        typedef std::map<t_tscalar, t_uindex, t_comparator<t_tscalar, (DTYPE)>> map;           \
        auto zero = mktscalar<CTYPE>(0);                                                       \
        auto one = mktscalar<CTYPE>(1);                                                        \
        const int num_zeros = 45;                                                              \
        const int num_ones = 55;                                                               \
        map m((t_comparator<t_tscalar, DTYPE>()));                                             \
        m[zero] = 1;                                                                           \
        m[one] = 1;                                                                            \
        for (int idx = 0; idx < num_zeros - 1; ++idx) {                                        \
            m[zero] = m[zero] + 1;                                                             \
        }                                                                                      \
        for (int idx = 0; idx < num_ones - 1; ++idx) {                                         \
            m[one] = m[one] + 1;                                                               \
        }                                                                                      \
        ASSERT_EQ(m[zero], num_zeros);                                                         \
        ASSERT_EQ(m[one], num_ones);                                                           \
    }

    TEST_BODY(DTYPE_INT64, t_int64);
    TEST_BODY(DTYPE_INT32, t_int32);
    TEST_BODY(DTYPE_INT16, t_int16);
    TEST_BODY(DTYPE_INT8, t_int8);
    TEST_BODY(DTYPE_UINT64, t_uint64);
    TEST_BODY(DTYPE_UINT32, t_uint32);
    TEST_BODY(DTYPE_UINT16, t_uint16);
    TEST_BODY(DTYPE_UINT8, t_uint8);
    TEST_BODY(DTYPE_FLOAT64, t_float64);
    TEST_BODY(DTYPE_FLOAT32, t_float32);
    TEST_BODY(DTYPE_BOOL, t_bool);
#undef TEST_BODY
}

TEST(SCALAR, map_test_2) {
    typedef std::map<t_tscalar, t_uindex, t_comparator<t_tscalar, DTYPE_STR>> map;
    auto zero = mktscalar("a");
    auto one = mktscalar("c");
    const int num_zeros = 45;
    const int num_ones = 55;
    map m((t_comparator<t_tscalar, DTYPE_STR>()));
    m[zero] = 1;
    m[one] = 1;
    for (int idx = 0; idx < num_zeros - 1; ++idx) {
        m[zero] = m[zero] + 1;
    }
    for (int idx = 0; idx < num_ones - 1; ++idx) {
        m[one] = m[one] + 1;
    }
    ASSERT_EQ(m[zero], num_zeros);
    ASSERT_EQ(m[one], num_ones);
}

TEST(SCALAR, map_test_3) {
    typedef t_comparator<t_tscalar, DTYPE_STR> t_cmp;
    typedef std::map<t_tscalar, t_uindex, t_cmp> t_map;
    t_map mmap((t_cmp()));
    mmap[""_ns] = 1;
    mmap["x"_ts] = 2;
    mmap["y"_ts] = 3;
    ASSERT_EQ(mmap.size(), 3);
}

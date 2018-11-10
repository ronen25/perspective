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
#include <perspective/none.h>
#include <perspective/gnode.h>
#include <gtest/gtest.h>
#include <limits>

using namespace perspective;

t_tscalar bnull = mknull(DTYPE_BOOL);
t_tscalar snull = mknull(DTYPE_STR);
t_tscalar i64null = mknull(DTYPE_INT64);

t_tscalar s_true = mktscalar<t_bool>(true);
t_tscalar s_false = mktscalar<t_bool>(false);
t_tscalar s_none = mktscalar<t_none>(t_none());
t_tscalar iop = mktscalar<t_uint8>(OP_INSERT);
t_tscalar dop = mktscalar<t_uint8>(OP_DELETE);
t_tscalar cop = mktscalar<t_uint8>(OP_CLEAR);


TEST(TABLE, simplest_test)
{
    t_table tbl(t_schema({"a", "b"}, {DTYPE_INT64, DTYPE_FLOAT64}), 5);
    tbl.init();
    tbl.reserve(5);
}

TEST(SCALAR, scalar_literal_test)
{
    auto s1 = 1_ts;
    auto s2 = "hello"_ts;
    auto s3 = 42.0_ts;
    ASSERT_EQ(s1, mktscalar<t_int64>(1));
    ASSERT_EQ(s2, mktscalar<const char*>("hello"));
    ASSERT_EQ(s3, mktscalar<t_float64>(42.0));
}

TEST(CONTEXT_ONE, null_pivot_test_1)
{
    t_schema sch{{"p", "a"}, {DTYPE_INT64, DTYPE_INT64}};
    t_table tbl(sch, {{1_ns, 1_ts}, {1_ts, 1_ts}});
    t_config cfg{{"p"}, {"sum_a", AGGTYPE_SUM, "a"}};
    auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(
        t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
    auto ctx_tbl = ctx->get_table();
    auto got = ctx_tbl->get_scalvec();
    t_tscalvec expected{2_ts, 1_ns, 1_ts, 1_ts, 1_ts, 1_ns};
    ASSERT_EQ(expected, got);
}

TEST(STORAGE, constructor)
{
    t_lstore s;
    ASSERT_EQ(s.get_init(), false);
    ASSERT_TRUE(s.empty());
}

TEST(STORAGE, uninited_get)
{
    t_lstore s;
    ASSERT_EQ(nullptr, s.get_ptr(0));
}

TEST(STORAGE, extend_1)
{
    t_lstore s;
    s.init();
    s.extend<t_int64>(10);
    ASSERT_EQ(s.size(), sizeof(t_int64) * 10);
}

// TODO add assert eqs here
TEST(CONTEXT_ONE, pivot_1)
{
    t_schema sch{{"b", "s", "i"}, {DTYPE_BOOL, DTYPE_STR, DTYPE_INT64}};
    auto t = mktscalar<t_bool>(true);
    auto f = mktscalar<t_bool>(false);
    t_table tbl(sch,
        {{t, "a"_ts, 1_ts}, {f, "b"_ts, 2_ts}, {t, "c"_ts, 3_ts},
            {f, "d"_ts, 4_ts}});
    t_config cfg{{"b", "s"}, {"sum_i", AGGTYPE_SUM, "i"}};
    auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(
        t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
    auto ctx_tbl = ctx->get_table();
    auto got = ctx_tbl->get_scalvec();
    t_tscalvec expected{10_ts, bnull, snull, 4_ts, s_true, snull, 3_ts, bnull,
        "c"_ts, 1_ts, bnull, "a"_ts, 6_ts, s_false, snull, 4_ts, bnull, "d"_ts,
        2_ts, bnull, "b"_ts};
    // ASSERT_EQ(expected, got);
}

TEST(CONTEXT_ONE, pivot_2)
{
    t_schema sch{{"i", "s", "b"}, {DTYPE_BOOL, DTYPE_STR, DTYPE_INT64}};
    auto t = mktscalar<t_bool>(true);
    auto f = mktscalar<t_bool>(false);
    t_table tbl(sch,
        {{t, "a"_ts, 1_ts}, {f, "b"_ts, 2_ts}, {t, "c"_ts, 3_ts},
            {f, "d"_ts, 4_ts}});
    t_config cfg{{"b", "b", "i", "s"}, {"sum_i", AGGTYPE_SUM, "i"}};
    auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(
        t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
    auto ctx_tbl = ctx->get_table();
    // auto got =  ctx_tbl->get_scalvec();
    // t_tscalvec expected{2_ts, 1_ns, 1_ts, 1_ts, 1_ts, 1_ns};
    // ASSERT_EQ(expected, got);
}

TEST(CONTEXT_ONE, pivot_3)
{
    t_schema sch{{"b", "s", "i"}, {DTYPE_BOOL, DTYPE_STR, DTYPE_INT64}};
    auto t = mktscalar<t_bool>(true);
    auto f = mktscalar<t_bool>(false);
    t_table tbl(sch,
        {{t, "a"_ts, 1_ts}, {f, "b"_ts, 2_ts}, {t, "c"_ts, 3_ts},
            {f, "d"_ts, 4_ts}});
    t_config cfg{{"b", "i"}, {"sum_i", AGGTYPE_SUM, "i"}};
    auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(
        t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
    auto ctx_tbl = ctx->get_table();
    // auto got =  ctx_tbl->get_scalvec();
    // t_tscalvec expected{2_ts, 1_ns, 1_ts, 1_ts, 1_ts, 1_ns};
    // ASSERT_EQ(expected, got);
}

TEST(CONTEXT_ONE, pivot_4)
{
    t_schema sch{{"b", "s", "i"}, {DTYPE_BOOL, DTYPE_STR, DTYPE_INT64}};
    auto t = mktscalar<t_bool>(true);
    auto f = mktscalar<t_bool>(false);
    t_table tbl(sch,
        {{t, "a"_ts, 1_ts}, {f, "b"_ts, 2_ts}, {t, "c"_ts, 3_ts},
            {f, "d"_ts, 4_ts}});
    t_config cfg{{"i", "s"}, {"sum_i", AGGTYPE_SUM, "i"}};
    auto ctx = do_pivot<t_ctx1, t_int32, DTYPE_INT32>(
        t_do_pivot::PIVOT_NON_PKEYED, tbl, cfg);
    auto ctx_tbl = ctx->get_table();
    // auto got =  ctx_tbl->get_scalvec();
    // t_tscalvec expected{2_ts, 1_ns, 1_ts, 1_ts, 1_ts, 1_ns};
    // ASSERT_EQ(expected, got);
}

TEST(SCALAR, map_test_1)
{
#define TEST_BODY(DTYPE, CTYPE)                                                \
    {                                                                          \
        typedef std::map<t_tscalar, t_uindex,                                  \
            t_comparator<t_tscalar, (DTYPE)>>                                  \
            map;                                                               \
        auto zero = mktscalar<CTYPE>(0);                                       \
        auto one = mktscalar<CTYPE>(1);                                        \
        const int num_zeros = 45;                                              \
        const int num_ones = 55;                                               \
        map m((t_comparator<t_tscalar, DTYPE>()));                             \
        m[zero] = 1;                                                           \
        m[one] = 1;                                                            \
        for (int idx = 0; idx < num_zeros - 1; ++idx)                          \
        {                                                                      \
            m[zero] = m[zero] + 1;                                             \
        }                                                                      \
        for (int idx = 0; idx < num_ones - 1; ++idx)                           \
        {                                                                      \
            m[one] = m[one] + 1;                                               \
        }                                                                      \
        ASSERT_EQ(m[zero], num_zeros);                                         \
        ASSERT_EQ(m[one], num_ones);                                           \
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

TEST(SCALAR, map_test_2)
{
    typedef std::map<t_tscalar, t_uindex, t_comparator<t_tscalar, DTYPE_STR>>
        map;
    auto zero = mktscalar("a");
    auto one = mktscalar("c");
    const int num_zeros = 45;
    const int num_ones = 55;
    map m((t_comparator<t_tscalar, DTYPE_STR>()));
    m[zero] = 1;
    m[one] = 1;
    for (int idx = 0; idx < num_zeros - 1; ++idx)
    {
        m[zero] = m[zero] + 1;
    }
    for (int idx = 0; idx < num_ones - 1; ++idx)
    {
        m[one] = m[one] + 1;
    }
    ASSERT_EQ(m[zero], num_zeros);
    ASSERT_EQ(m[one], num_ones);
}

TEST(SCALAR, map_test_3)
{
    typedef t_comparator<t_tscalar, DTYPE_STR> t_cmp;
    typedef std::map<t_tscalar, t_uindex, t_cmp> t_map;
    t_map mmap((t_cmp()));
    mmap[""_ns] = 1;
    mmap["x"_ts] = 2;
    mmap["y"_ts] = 3;
    ASSERT_EQ(mmap.size(), 3);
}

TEST(SCALAR, scalar_repr)
{
    EXPECT_EQ(s_none.repr(), "none:v:");
    EXPECT_EQ(mktscalar<t_int64>(0).repr(), "i64:v:0");
    EXPECT_EQ(mktscalar<t_int32>(0).repr(), "i32:v:0");
    EXPECT_EQ(mktscalar<t_int16>(0).repr(), "i16:v:0");
    EXPECT_EQ(mktscalar<t_int8>(0).repr(), "i8:v:0");
    EXPECT_EQ(mktscalar<t_uint64>(0).repr(), "u64:v:0");
    EXPECT_EQ(mktscalar<t_uint32>(0).repr(), "u32:v:0");
    EXPECT_EQ(mktscalar<t_uint16>(0).repr(), "u16:v:0");
    EXPECT_EQ(mktscalar<t_uint8>(0).repr(), "u8:v:0");
    EXPECT_EQ(mktscalar<t_float64>(0).repr(), "f64:v:0");
    EXPECT_EQ(mktscalar<t_float32>(0).repr(), "f32:v:0");
    EXPECT_EQ(mktscalar<t_date>(t_date(0)).repr(), "date:v:0-00-00");
    EXPECT_EQ(
        mktscalar<t_time>(t_time(0)).repr(), "time:v:1970-01-01 00:00:00.000");
    EXPECT_EQ(mktscalar<const char*>("").repr(), "str:v:");

    EXPECT_EQ(mknull(DTYPE_NONE).repr(), "none:i:null");
    EXPECT_EQ(mknull(DTYPE_INT64).repr(), "i64:i:null");
    EXPECT_EQ(mknull(DTYPE_INT32).repr(), "i32:i:null");
    EXPECT_EQ(mknull(DTYPE_INT16).repr(), "i16:i:null");
    EXPECT_EQ(mknull(DTYPE_INT8).repr(), "i8:i:null");
    EXPECT_EQ(mknull(DTYPE_UINT64).repr(), "u64:i:null");
    EXPECT_EQ(mknull(DTYPE_UINT32).repr(), "u32:i:null");
    EXPECT_EQ(mknull(DTYPE_UINT16).repr(), "u16:i:null");
    EXPECT_EQ(mknull(DTYPE_UINT8).repr(), "u8:i:null");
    EXPECT_EQ(mknull(DTYPE_FLOAT64).repr(), "f64:i:null");
    EXPECT_EQ(mknull(DTYPE_FLOAT32).repr(), "f32:i:null");
    EXPECT_EQ(mknull(DTYPE_DATE).repr(), "date:i:null");
    EXPECT_EQ(mknull(DTYPE_TIME).repr(), "time:i:null");
    EXPECT_EQ(mknull(DTYPE_STR).repr(), "str:i:null");
}

TEST(SCALAR, scalar_str)
{
    EXPECT_TRUE(mktscalar("a") < mktscalar("b"));
    EXPECT_TRUE(mktscalar("a") == mktscalar("a"));
}

TEST(SCALAR, nan_test)
{
    EXPECT_TRUE(
        mktscalar<t_float64>(std::numeric_limits<t_float64>::quiet_NaN())
            .is_nan());
    EXPECT_TRUE(
        mktscalar<t_float32>(std::numeric_limits<t_float64>::quiet_NaN())
            .is_nan());
}

TEST(SCALAR, difference)
{
    EXPECT_EQ(
        mktscalar<t_float64>(42).difference(mknull(DTYPE_FLOAT64)).to_double(),
        42.0);
    EXPECT_EQ(
        mknull(DTYPE_FLOAT64).difference(mktscalar<t_float64>(42)).to_double(),
        -42.0);
}

class GnodeI64 : public ::testing::Test {
public:
    typedef std::function<t_table_sptr(const t_schema&, const std::vector<t_tscalvec>&)> t_tblfactory;
    GnodeI64() {
        m_ischema = t_schema{
                {"psp_op", "psp_pkey", "x"}, {DTYPE_UINT8, DTYPE_INT64, DTYPE_INT64}};
        m_oschema = {{"psp_pkey", "x"}, {DTYPE_INT64, DTYPE_INT64}};
        m_g = t_gnode::build(m_ischema);
    }

    t_table_sptr it(const std::vector<t_tscalvec>& recs) const {
    return make_table(m_ischema, recs);
    }

    t_table_sptr ot(const std::vector<t_tscalvec>& recs) const {
        return make_table(m_oschema, recs);
    }

    t_table_sptr make_table(const t_schema& s, const std::vector<t_tscalvec>& recs ) const {
        return std::make_shared<t_table>(s, recs);
    }

    t_table_sptr step(const std::vector<t_tscalvec>& recs) {
        return m_g->tstep(it(recs));
    }

protected:
    t_schema m_ischema;
    t_schema m_oschema;
    t_gnode_sptr m_g;

};

TEST_F(GnodeI64, clear)
{
    EXPECT_EQ(*step({{iop, 1_ts, 1_ts}}), *ot({{1_ts, 1_ts}}));
    EXPECT_EQ(*step({{iop, 1_ts, mkclear(DTYPE_INT64)}}), *ot({{1_ts, mknull(DTYPE_INT64)}}));
}

TEST_F(GnodeI64, row_delete)
{
    EXPECT_EQ(*step({{iop, 1_ts, 1_ts}}), *ot({{1_ts, 1_ts}}));
    EXPECT_EQ(*step({{dop, 1_ts, i64null}}), *ot({}));
}

TEST_F(GnodeI64, test_1)
{
    EXPECT_EQ(*step({{iop, 1_ts, 1_ts}}), *ot({{1_ts, 1_ts}}));
    EXPECT_EQ(*step({{iop, 1_ts, i64null}}), *ot({{1_ts, 1_ts}}));
}

TEST_F(GnodeI64, test_2)
{
    EXPECT_EQ(*step({{iop, 1_ts, 1_ts}}), *ot({{1_ts, 1_ts}}));
    EXPECT_EQ(*step({{dop, 1_ts, i64null},
                     {iop, 1_ts, 5_ts}}), *ot({{1_ts, 5_ts}}));
}

TEST_F(GnodeI64, test_3) {
    EXPECT_EQ(*step({{iop, 1_ts, i64null}}), *ot({{1_ts, i64null}}));
    EXPECT_EQ(*step({{iop, 1_ts, i64null}}), *ot({{1_ts, i64null}}));
}

TEST_F(GnodeI64, test_5) {
    EXPECT_EQ(*step({{iop, 1_ts, i64null}}), *ot({{1_ts, i64null}}));
    EXPECT_EQ(*step({{iop, 1_ts, 5_ts}}), *ot({{1_ts, 5_ts}}));
}

TEST_F(GnodeI64, test_6) {
    EXPECT_EQ(*step({{iop, 1_ts, 5_ts}}), *ot({{1_ts, 5_ts}}));
    EXPECT_EQ(*step({{iop, 1_ts, i64null}}), *ot({{1_ts, 5_ts}}));
}

TEST_F(GnodeI64, test_7) {
    EXPECT_EQ(*step({{iop, 1_ts, 5_ts}}), *ot({{1_ts, 5_ts}}));
    EXPECT_EQ(*step({{iop, 1_ts, 6_ts}}), *ot({{1_ts, 6_ts}}));
}
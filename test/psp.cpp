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

t_dtypevec numeric_dtypes{
    DTYPE_INT64,
    DTYPE_INT32,
    DTYPE_INT16,
    DTYPE_INT8,
    DTYPE_UINT64,
    DTYPE_UINT32,
    DTYPE_UINT16,
    DTYPE_UINT8,
    DTYPE_FLOAT64,
    DTYPE_FLOAT32,
};

t_dtypevec common_dtypes{
    DTYPE_NONE,
    DTYPE_INT64,
    DTYPE_INT32,
    DTYPE_INT16,
    DTYPE_INT8,
    DTYPE_UINT64,
    DTYPE_UINT32,
    DTYPE_UINT16,
    DTYPE_UINT8,
    DTYPE_FLOAT64,
    DTYPE_FLOAT32,
    DTYPE_BOOL,
    DTYPE_TIME,
    DTYPE_DATE,
    DTYPE_STR,
};

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

TEST(SCALAR, none_test)
{
    EXPECT_TRUE(mknone().is_none());
    EXPECT_TRUE(std::all_of(common_dtypes.begin(), common_dtypes.end(),
        [](t_dtype t) { return t == DTYPE_NONE || !mknull(t).is_none(); }));
    EXPECT_TRUE(std::all_of(common_dtypes.begin(), common_dtypes.end(),
        [](t_dtype t) { return t == DTYPE_NONE || !mkclear(t).is_none(); }));
}

TEST(SCALAR, gt_test)
{
    EXPECT_TRUE(mktscalar<t_int64>(2) > mktscalar<t_int64>(1));
    EXPECT_TRUE(mktscalar<t_float64>(2) > mktscalar<t_float64>(1));
    EXPECT_TRUE(mktscalar<const char*>("b") > mktscalar<const char*>("a"));
}

TEST(SCALAR, gteq_test)
{
    EXPECT_TRUE(mktscalar<t_int64>(2) >= mktscalar<t_int64>(1));
    EXPECT_TRUE(mktscalar<t_float64>(2) >= mktscalar<t_float64>(1));
    EXPECT_TRUE(mktscalar<const char*>("b") >= mktscalar<const char*>("a"));

    EXPECT_TRUE(mktscalar<t_int64>(2) >= mktscalar<t_int64>(2));
    EXPECT_TRUE(mktscalar<t_float64>(2) >= mktscalar<t_float64>(2));
    EXPECT_TRUE(mktscalar<const char*>("b") >= mktscalar<const char*>("b"));
}

TEST(SCALAR, lteq_test)
{
    EXPECT_TRUE(mktscalar<t_int64>(1) <= mktscalar<t_int64>(2));
    EXPECT_TRUE(mktscalar<t_float64>(1) <= mktscalar<t_float64>(2));
    EXPECT_TRUE(mktscalar<const char*>("a") <= mktscalar<const char*>("b"));

    EXPECT_TRUE(mktscalar<t_int64>(2) <= mktscalar<t_int64>(2));
    EXPECT_TRUE(mktscalar<t_float64>(2) <= mktscalar<t_float64>(2));
    EXPECT_TRUE(mktscalar<const char*>("b") <= mktscalar<const char*>("b"));
}

TEST(SCALAR, is_numeric)
{
    EXPECT_TRUE(std::all_of(numeric_dtypes.begin(), numeric_dtypes.end(),
        [](t_dtype t) { return mknull(t).is_numeric(); }));
    EXPECT_FALSE(mktscalar<const char*>("hello").is_numeric());
    EXPECT_FALSE(s_none.is_numeric());
}

TEST(SCALAR, canonical_test)
{
    EXPECT_TRUE(std::all_of(numeric_dtypes.begin(), numeric_dtypes.end(),
        [](t_dtype t) { return t_tscalar::canonical(t).to_double() == 0; }));
    // TODO
    //EXPECT_EQ(mktscalar<const char*>(""), t_tscalar::canonical(DTYPE_STR));
}

TEST(SCALAR, abs_test)
{
    EXPECT_EQ(mktscalar<t_int64>(5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int64>(-5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int32>(5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int32>(-5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int16>(5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int16>(-5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int8>(5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int8>(-5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_float64>(5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_float64>(-5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_float32>(5).abs().to_double(), 5);
    EXPECT_EQ(mktscalar<t_float32>(-5).abs().to_double(), 5);
}

TEST(SCALAR, negate)
{
    EXPECT_EQ(mktscalar<t_int64>(5).negate().to_double(), -5);
    EXPECT_EQ(mktscalar<t_int64>(-5).negate().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int32>(5).negate().to_double(), -5);
    EXPECT_EQ(mktscalar<t_int32>(-5).negate().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int16>(5).negate().to_double(), -5);
    EXPECT_EQ(mktscalar<t_int16>(-5).negate().to_double(), 5);
    EXPECT_EQ(mktscalar<t_int8>(5).negate().to_double(), -5);
    EXPECT_EQ(mktscalar<t_int8>(-5).negate().to_double(), 5);
    EXPECT_EQ(mktscalar<t_float64>(5).negate().to_double(), -5);
    EXPECT_EQ(mktscalar<t_float64>(-5).negate().to_double(), 5);
    EXPECT_EQ(mktscalar<t_float32>(5).negate().to_double(), -5);
    EXPECT_EQ(mktscalar<t_float32>(-5).negate().to_double(), 5);
}

TEST(SCALAR, begins_with)
{
    EXPECT_TRUE(
        mktscalar<const char*>("").begins_with(mktscalar<const char*>("")));
    EXPECT_TRUE(mktscalar<const char*>("abc").begins_with(
        mktscalar<const char*>("ab")));
    EXPECT_FALSE(mktscalar<const char*>("ab").begins_with(
        mktscalar<const char*>("abc")));
    EXPECT_TRUE(mktscalar<const char*>("aBc").begins_with(
        mktscalar<const char*>("Ab")));
    EXPECT_FALSE(s_none.begins_with(mktscalar<const char*>("aBc")));
    EXPECT_FALSE(mktscalar<const char*>("aBc").begins_with(s_none));
}

TEST(SCALAR, ends_with)
{
    EXPECT_TRUE(
        mktscalar<const char*>("").ends_with(mktscalar<const char*>("")));
    EXPECT_TRUE(
        mktscalar<const char*>("abc").ends_with(mktscalar<const char*>("bc")));
    EXPECT_FALSE(
        mktscalar<const char*>("bc").ends_with(mktscalar<const char*>("abc")));
    EXPECT_TRUE(
        mktscalar<const char*>("aBc").ends_with(mktscalar<const char*>("bC")));
    EXPECT_FALSE(s_none.ends_with(mktscalar<const char*>("aBc")));
    EXPECT_FALSE(mktscalar<const char*>("aBc").ends_with(s_none));
}

TEST(SCALAR, contains)
{
    EXPECT_TRUE(
        mktscalar<const char*>("").contains(mktscalar<const char*>("")));
    EXPECT_TRUE(
        mktscalar<const char*>("abc").contains(mktscalar<const char*>("ab")));
    EXPECT_FALSE(
        mktscalar<const char*>("ab").contains(mktscalar<const char*>("abc")));
    EXPECT_TRUE(
        mktscalar<const char*>("aBc").contains(mktscalar<const char*>("Ab")));
    EXPECT_FALSE(s_none.contains(mktscalar<const char*>("aBc")));
    EXPECT_FALSE(mktscalar<const char*>("aBc").contains(s_none));
    EXPECT_TRUE(
        mktscalar<const char*>("aBcd").contains(mktscalar<const char*>("bc")));
}

class BaseTest : public ::testing::Test
{
public:
    typedef std::vector<t_tscalvec> t_tbldata;
    typedef std::pair<t_tbldata, t_tbldata> t_stepdata;
    typedef std::vector<t_stepdata> t_testdata;

protected:
    virtual t_table_sptr get_step_otable() = 0;

    void
    run(const t_testdata& d)
    {
        for (const auto& sd : d)
        {
            t_table itbl(m_ischema, sd.first);
            t_table expected_otbl(m_oschema, sd.second);
            m_g->_send_and_process(itbl);
            auto otbl = get_step_otable();
            EXPECT_EQ(*otbl, expected_otbl);
        }
    }

    t_gnode_sptr m_g;
    t_schema m_ischema;
    t_schema m_oschema;
};

class I64GTest : public BaseTest
{

public:
    I64GTest()
    {
        m_ischema = t_schema{{"psp_op", "psp_pkey", "x"},
            {DTYPE_UINT8, DTYPE_INT64, DTYPE_INT64}};
        m_oschema = {{"psp_pkey", "x"}, {DTYPE_INT64, DTYPE_INT64}};
        m_g = t_gnode::build(m_ischema);
        null = mknull(DTYPE_INT64);
        clear = mkclear(DTYPE_INT64);
    }

    virtual t_table_sptr
    get_step_otable()
    {
        return m_g->get_sorted_pkeyed_table();
    }

protected:
    t_tscalar null;
    t_tscalar clear;
};

class I64Ctx1Test : public I64GTest
{

public:
    I64Ctx1Test()
    {
        m_oschema = {{"sum_x", "x"}, {DTYPE_INT64, DTYPE_INT64}};
        t_config cfg{{"x"}, {"sum_x", AGGTYPE_SUM, "x"}};
        m_ctx = std::make_shared<t_ctx1>(m_ischema, cfg);
        m_g->register_context("ctx", m_ctx);
    }

    virtual t_table_sptr
    get_step_otable()
    {
        return m_ctx->get_table();
    }

protected:
    t_ctx1_sptr m_ctx;
};


// clang-format off
TEST_F(I64GTest, test_1) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, clear}},
            {{1_ts, null}}
        }
    };

    run(data);
}

TEST_F(I64GTest, test_2) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, 1_ts}}
        },
        {
            {{dop, 1_ts, null}},
            {}
        }
    };

    run(data);
}

TEST_F(I64GTest, test_3) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, null}},
            {{1_ts, 1_ts}}
        }
    };

    run(data);
}

TEST_F(I64GTest, test_4) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, 1_ts}}
        },
        {
            {{dop, 1_ts, null},
            {iop, 1_ts, 2_ts}},
            {{1_ts, 2_ts}}
        }
    };

    run(data);
}

TEST_F(I64GTest, test_5) {

    t_testdata data{
        {
            {{iop, 1_ts, null}},
            {{1_ts, null}}
        },
        {
            {{iop, 1_ts, null}},
            {{1_ts, null}}
        }
    };

    run(data);
}

TEST_F(I64GTest, test_6) {

    t_testdata data{
        {
            {{iop, 1_ts, null}},
            {{1_ts, null}}
        },
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, 1_ts}}
        }
    };

    run(data);
}

TEST_F(I64GTest, test_7) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, null}},
            {{1_ts, 1_ts}}
        }
    };

    run(data);
}

TEST_F(I64GTest, test_8) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, 2_ts}},
            {{1_ts, 2_ts}}
        }
    };

    run(data);
}

TEST_F(I64GTest, test_9) {

    t_testdata data{
        {
            {{dop, 1_ts, null}},
            {}
        }
    };

    run(data);
}

TEST_F(I64Ctx1Test, test_1) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, null},
            {1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, null},
            {1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, clear}},
            {{1_ts, null},
            {1_ts, 1_ts}}
        }
    };

    run(data);
}

TEST_F(I64Ctx1Test, test_2) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, null},
             {1_ts, 1_ts}}},
        {
            {{dop, 1_ts, null}},
            {{0_ts, null}}
        }
    };

    run(data);
}

TEST_F(I64Ctx1Test, test_3) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, null},
             {1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, null}},
            {{1_ts, null},
             {1_ts, 1_ts}}
        }
    };

    run(data);
}

TEST_F(I64Ctx1Test, test_4) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, null},
             {1_ts, 1_ts}}
        },
        {
            {{dop, 1_ts, null},
            {iop, 1_ts, 2_ts}},
            {{2_ts, null},
             {2_ts, 2_ts}}
        }
    };

    run(data);
}

TEST_F(I64Ctx1Test, test_5) {

    t_testdata data{
        {
            {{iop, 1_ts, null}},
            {{0_ts, null},
             {0_ts, null}}
        },
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, null},
             {1_ts, 1_ts}}
        }
    };

    run(data);
}

TEST_F(I64Ctx1Test, test_6) {

   t_testdata data{
       {
           {{iop, 1_ts, null}},
           {{0_ts, null},
            {0_ts, null}}
       },
       {
           {{iop, 1_ts, 1_ts}},
           {{1_ts, null},
            {1_ts, 1_ts}}
       }
   };


   run(data);
}

TEST_F(I64Ctx1Test, test_7) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, null},
             {1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, null}},
            {{1_ts, null},
             {1_ts, 1_ts}}
        }
    };

    run(data);
}

TEST_F(I64Ctx1Test, test_8) {

    t_testdata data{
        {
            {{iop, 1_ts, 1_ts}},
            {{1_ts, null},
            {1_ts, 1_ts}}
        },
        {
            {{iop, 1_ts, 2_ts}},
            {{2_ts, null},
             {2_ts, 2_ts}}
        }
    };

    run(data);
}

TEST_F(I64Ctx1Test, test_9) {

    t_testdata data{
        {
            {{dop, 1_ts, null}},
            {{null, null}}
        }
    };

    run(data);
}

// clang-format on


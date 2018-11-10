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

template <typename DATA_T, perspective::t_dtype DTYPE_T>
struct test_traits
{
    typedef DATA_T t_data;
    static perspective::t_dtype dtype;
    static perspective::t_tscalar null;
    static perspective::t_tscalar clear;
    static perspective::t_tscalar zero;
    static perspective::t_tscalar v1;
    static perspective::t_tscalar v2;
};

// String specializations
template <>
perspective::t_tscalar
    test_traits<perspective::t_str, perspective::DTYPE_STR>::zero;

template <>
perspective::t_tscalar
    test_traits<perspective::t_str, perspective::DTYPE_STR>::v1;

template <>
perspective::t_tscalar
    test_traits<perspective::t_str, perspective::DTYPE_STR>::v2;

// Date specializations
template <>
perspective::t_tscalar
    test_traits<perspective::t_date, perspective::DTYPE_DATE>::zero;

template <>
perspective::t_tscalar
    test_traits<perspective::t_date, perspective::DTYPE_DATE>::v1;

template <>
perspective::t_tscalar
    test_traits<perspective::t_date, perspective::DTYPE_DATE>::v2;

// Time specializations
template <>
perspective::t_tscalar
    test_traits<perspective::t_time, perspective::DTYPE_TIME>::zero;

template <>
perspective::t_tscalar
    test_traits<perspective::t_time, perspective::DTYPE_TIME>::v1;

template <>
perspective::t_tscalar
    test_traits<perspective::t_time, perspective::DTYPE_TIME>::v2;

template <typename DATA_T, perspective::t_dtype DTYPE_T>
perspective::t_dtype test_traits<DATA_T, DTYPE_T>::dtype = DTYPE_T;

template <typename DATA_T, perspective::t_dtype DTYPE_T>
perspective::t_tscalar test_traits<DATA_T, DTYPE_T>::null
    = perspective::mknull(DTYPE_T);

template <typename DATA_T, perspective::t_dtype DTYPE_T>
perspective::t_tscalar test_traits<DATA_T, DTYPE_T>::clear
    = perspective::mkclear(DTYPE_T);

template <typename DATA_T, perspective::t_dtype DTYPE_T>
perspective::t_tscalar test_traits<DATA_T, DTYPE_T>::zero
    = perspective::mktscalar<DATA_T>(0);

template <typename DATA_T, perspective::t_dtype DTYPE_T>
perspective::t_tscalar test_traits<DATA_T, DTYPE_T>::v1
    = perspective::mktscalar<DATA_T>(1);

template <typename DATA_T, perspective::t_dtype DTYPE_T>
perspective::t_tscalar test_traits<DATA_T, DTYPE_T>::v2
    = perspective::mktscalar<DATA_T>(2);

// String specializations
template <>
t_tscalar test_traits<t_str, DTYPE_STR>::zero = mktscalar<const char*>("");

template <>
t_tscalar test_traits<t_str, DTYPE_STR>::v1 = mktscalar<const char*>("1");

template <>
t_tscalar test_traits<t_str, DTYPE_STR>::v2 = mktscalar<const char*>("2");

// Date specializations
template <>
t_tscalar test_traits<t_date, DTYPE_DATE>::zero = mktscalar(t_date());

template <>
t_tscalar test_traits<t_date, DTYPE_DATE>::v1 = mktscalar(t_date(2018, 1, 1));

template <>
t_tscalar test_traits<t_date, DTYPE_DATE>::v2 = mktscalar(t_date(2018, 2, 2));

// Time specializations
template <>
t_tscalar test_traits<t_time, DTYPE_TIME>::zero = mktscalar(t_time(0));

template <>
t_tscalar test_traits<t_time, DTYPE_TIME>::v1 = mktscalar(t_time(100000));

template <>
t_tscalar test_traits<t_time, DTYPE_TIME>::v2 = mktscalar(t_time(200000));

typedef test_traits<t_int64, DTYPE_INT64> tr_i64;
typedef test_traits<t_int64, DTYPE_INT32> tr_i32;
typedef test_traits<t_uint64, DTYPE_UINT64> tr_u64;
typedef test_traits<t_float64, DTYPE_FLOAT64> tr_float64;
typedef test_traits<t_date, DTYPE_DATE> tr_date;
typedef test_traits<t_time, DTYPE_TIME> tr_time;
typedef test_traits<t_str, DTYPE_STR> tr_str;

using testing::Types;

#include "test_types.h"

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


template <typename TEST_TRAIT_T>
class GnodeTest : public ::testing::Test
{

public:
    typedef std::function<t_table_sptr(
        const t_schema&, const std::vector<t_tscalvec>&)>
        t_tblfactory;
    GnodeTest()
    {
        m_ischema = t_schema{{"psp_op", "psp_pkey", "x"},
            {DTYPE_UINT8, PKEY::dtype, VALUE::dtype}};
        m_oschema = {{"psp_pkey", "x"}, {PKEY::dtype, VALUE::dtype}};
        m_g = t_gnode::build(m_ischema);
    }

    t_table_sptr
    it(const std::vector<t_tscalvec>& recs) const
    {
        return make_table(m_ischema, recs);
    }

    t_table_sptr
    ot(const std::vector<t_tscalvec>& recs) const
    {
        return make_table(m_oschema, recs);
    }

    t_table_sptr
    make_table(const t_schema& s, const std::vector<t_tscalvec>& recs) const
    {
        return std::make_shared<t_table>(s, recs);
    }

    t_table_sptr
    step(const std::vector<t_tscalvec>& recs)
    {
        return m_g->tstep(it(recs));
    }

protected:
    typedef typename TEST_TRAIT_T::pkey PKEY;
    typedef typename TEST_TRAIT_T::value VALUE;
    t_schema m_ischema;
    t_schema m_oschema;
    t_gnode_sptr m_g;
};

TYPED_TEST_CASE(GnodeTest, all_test_traits);

TYPED_TEST(GnodeTest, clear)
{
    typedef TypeParam T;
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::v1}}),
        *this->ot({{T::pkey::v1, T::value::v1}}));
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::clear}}),
        *this->ot({{T::pkey::v1, T::value::null}}));
}

TYPED_TEST(GnodeTest, row_delete)
{
    typedef TypeParam T;
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::v1}}),
        *this->ot({{T::pkey::v1, T::value::v1}}));
    EXPECT_EQ(*this->step({{dop, T::pkey::v1, T::value::null}}), *this->ot({}));
}

TYPED_TEST(GnodeTest, test_1)
{
    typedef TypeParam T;
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::v1}}),
        *this->ot({{T::pkey::v1, T::value::v1}}));
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::null}}),
        *this->ot({{T::pkey::v1, T::value::v1}}));
}

TYPED_TEST(GnodeTest, test_2)
{
    typedef TypeParam T;
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::v1}}),
        *this->ot({{T::pkey::v1, T::value::v1}}));
    EXPECT_EQ(*this->step({{dop, T::pkey::v1, T::value::null},
                  {iop, T::pkey::v1, T::value::v2}}),
        *this->ot({{T::pkey::v1, T::value::v2}}));
}

TYPED_TEST(GnodeTest, test_3)
{
    typedef TypeParam T;
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::null}}),
        *this->ot({{T::pkey::v1, T::value::null}}));
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::null}}),
        *this->ot({{T::pkey::v1, T::value::null}}));
}

TYPED_TEST(GnodeTest, test_5)
{
    typedef TypeParam T;
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::null}}),
        *this->ot({{T::pkey::v1, T::value::null}}));
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::v1}}),
        *this->ot({{T::pkey::v1, T::value::v1}}));
}

TYPED_TEST(GnodeTest, test_6)
{
    typedef TypeParam T;
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::v1}}),
        *this->ot({{T::pkey::v1, T::value::v1}}));
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::null}}),
        *this->ot({{T::pkey::v1, T::value::v1}}));
}

TYPED_TEST(GnodeTest, test_7)
{
    typedef TypeParam T;
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::v1}}),
        *this->ot({{T::pkey::v1, T::value::v1}}));
    EXPECT_EQ(*this->step({{iop, T::pkey::v1, T::value::v2}}),
        *this->ot({{T::pkey::v1, T::value::v2}}));
}

TYPED_TEST(GnodeTest, test_8)
{
    typedef TypeParam T;
    EXPECT_EQ(*this->step({{dop, T::pkey::v1, T::value::null}}), *this->ot({}));
}

#ifndef __CASELIB__
#define __CASELIB__

#if defined(BOOST_PP_VARIADICS) && BOOST_PP_VARIADICS == 0
#error "case.h requires variadic boost preprocessor macros, but boost preprocessor has already been included without variadic support. Either include case.h before boost proprocessor, or insert '#define BOOST_PP_VARIADICS 1' before any boost preprocessor headers"
#endif

#define BOOST_PP_VARIADICS 1
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/for.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/punctuation/paren.hpp>

#define CASELIB_APPEND_PAREN(...) ((__VA_ARGS__)) BOOST_PP_LPAREN()
#define CASELIB_SPLIT_INTO_TUPLE(data) CASELIB_APPEND_PAREN data BOOST_PP_RPAREN()

#define CASELIB_SINGLE_CASE(r, args, case) \
    CASELIB_SINGLE_CASE_TUPLE(r, args, CASELIB_SPLIT_INTO_TUPLE(case))
#define CASELIB_SINGLE_CASE_TUPLE(r, args, case) \
    CASELIB_SINGLE_CASE_SPLIT(r, args, BOOST_PP_SEQ_ELEM(0,case), BOOST_PP_SEQ_ELEM(1,case))
#define CASELIB_SINGLE_CASE_SPLIT(r, args, case_args, case_val) \
    CASELIB_SINGLE_CASE_INTO_IF(r, \
                                BOOST_PP_FOR_##r((case_args, args (nil), 0), \
                                                 CASELIB_SINGLE_CASE_FOR_PRED, \
                                                 CASELIB_SINGLE_CASE_FOR_OP, \
                                                 CASELIB_SINGLE_CASE_FOR_MACRO), \
                                case_val)
#define CASELIB_SINGLE_CASE_FOR_MACRO(r, x) \
    CASELIB_SINGLE_CASE_FOR_MACRO_I(r, BOOST_PP_TUPLE_ELEM(3, 0, x), \
                                       BOOST_PP_TUPLE_ELEM(3, 1, x), \
                                       BOOST_PP_TUPLE_ELEM(3, 2, x))
#define CASELIB_SINGLE_CASE_FOR_MACRO_I(r, case_args, seq, i) \
    CASELIB_SINGLE_CASE_MACRO(r, case_args, i, BOOST_PP_SEQ_HEAD(seq))
#define CASELIB_SINGLE_CASE_FOR_PRED(r, x) \
    BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(3, 1, x)))
#define CASELIB_SINGLE_CASE_FOR_OP(r, x) \
    CASELIB_SINGLE_CASE_FOR_OP_I(BOOST_PP_TUPLE_ELEM(3, 0, x), \
                                 BOOST_PP_TUPLE_ELEM(3, 1, x), \
                                 BOOST_PP_TUPLE_ELEM(3, 2, x))
#define CASELIB_SINGLE_CASE_FOR_OP_I(case_args, args, i) \
    (case_args, BOOST_PP_SEQ_TAIL(args), BOOST_PP_INC(i))
#define CASELIB_SINGLE_CASE_MACRO(r, case_args, i, arg) \
    BOOST_PP_IF(i, &&,) \
    arg == BOOST_PP_TUPLE_ELEM(i, case_args)

#define CASELIB_SINGLE_CASE_INTO_IF(r, conditions, val) \
    (conditions) ? (_case_lib_ignore_assign val) :

#define CASELIB_LAST_CASE(r, args, case) case

#define CASELIB_CASE_SEQ(args, seq, len) \
    BOOST_PP_FOR((BOOST_PP_DEC(len), args, seq (nil), 0), \
                 CASELIB_CASE_SEQ_FOR_PRED, \
                 CASELIB_CASE_SEQ_FOR_OP, \
                 CASELIB_CASE_SEQ_FOR_MACRO)
#define CASELIB_CASE_SEQ_FOR_MACRO(r, x) \
    CASELIB_CASE_SEQ_FOR_MACRO_I(r, BOOST_PP_TUPLE_ELEM(4, 0, x), \
                                    BOOST_PP_TUPLE_ELEM(4, 1, x), \
                                    BOOST_PP_TUPLE_ELEM(4, 2, x), \
                                    BOOST_PP_TUPLE_ELEM(4, 3, x))
#define CASELIB_CASE_SEQ_FOR_MACRO_I(r, lasti, args, seq, i) \
    CASELIB_CASE_SEQ_MACRO(r, lasti, args, i, BOOST_PP_SEQ_HEAD(seq))
#define CASELIB_CASE_SEQ_FOR_PRED(r, x) \
    BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(4, 2, x)))
#define CASELIB_CASE_SEQ_FOR_OP(r, x) \
    CASELIB_CASE_SEQ_FOR_OP_I(BOOST_PP_TUPLE_ELEM(4, 0, x), \
                              BOOST_PP_TUPLE_ELEM(4, 1, x), \
                              BOOST_PP_TUPLE_ELEM(4, 2, x), \
                              BOOST_PP_TUPLE_ELEM(4, 3, x))
#define CASELIB_CASE_SEQ_FOR_OP_I(lasti, args, seq, i) \
    (lasti, args, BOOST_PP_SEQ_TAIL(seq), BOOST_PP_INC(i))

#define CASELIB_CASE_SEQ_MACRO(r, lasti, args, i, elem) \
    BOOST_PP_IF(BOOST_PP_NOT_EQUAL(i, lasti), CASELIB_SINGLE_CASE, CASELIB_LAST_CASE)(r, args, elem)



#define CASE(ARGS, ...) \
    CASELIB_CASE_SEQ(BOOST_PP_TUPLE_TO_SEQ(ARGS), \
                     BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__), \
                     BOOST_PP_VARIADIC_SIZE(__VA_ARGS__))

// Required for swallowing the equals sign
namespace {
    struct {
        template<typename T>
        T operator=(T&& val) const {
            return std::forward<T>(val);
        }
    } _case_lib_ignore_assign;
}

#endif

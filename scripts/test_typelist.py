from itertools import product

all = [
'tr_i64',
'tr_i32',
'tr_u64',
'tr_float64',
'tr_date',
'tr_time',
'tr_str',
]


def create_types(alist):
    all_types = []
    for t1, t2 in product(alist, alist):
        s = 'struct t_%s_%s {\n\ttypedef %s pkey;\n\ttypedef %s value;\n};\n' % (t1, t2, t1, t2)
        all_types.append('t_' + t1 + '_' + t2)
        print(s)

    print('\ntypedef Types<')
    for tp in all_types:
        print(tp + ',')
    print('> all_test_traits;')

create_types(all)
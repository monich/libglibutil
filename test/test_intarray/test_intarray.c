/*
 * Copyright (C) 2017 Jolla Ltd.
 * Contact: Slava Monich <slava.monich@jolla.com>
 *
 * You may use this file under the terms of BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the name of Jolla Ltd nor the names of its contributors may
 *      be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "test_common.h"

#include "gutil_intarray.h"
#include "gutil_ints.h"

static TestOpt test_opt;

/*==========================================================================*
 * NULL tolerance
 *==========================================================================*/

static
void
test_intarray_null(
    void)
{
    gutil_int_array_unref(NULL);
    g_assert(!gutil_int_array_ref(NULL));
    g_assert(!gutil_int_array_free(NULL, FALSE));
    g_assert(!gutil_int_array_free_to_ints(NULL));
    g_assert(!gutil_int_array_append(NULL, 0));
    g_assert(!gutil_int_array_append_vals(NULL, NULL, 0));
    g_assert(!gutil_int_array_prepend(NULL, 0));
    g_assert(!gutil_int_array_prepend_vals(NULL, NULL, 0));
    g_assert(!gutil_int_array_insert(NULL, 0, 0));
    g_assert(!gutil_int_array_insert_vals(NULL, 0, NULL, 0));
    g_assert(!gutil_int_array_set_count(NULL, 0));
    g_assert(!gutil_int_array_remove_index(NULL, 0));
    g_assert(!gutil_int_array_remove_index_fast(NULL, 0));
    g_assert(!gutil_int_array_remove_range(NULL, 0, 0));
    gutil_int_array_sort_ascending(NULL);
    gutil_int_array_sort_descending(NULL);
}

/*==========================================================================*
 * Basic
 *==========================================================================*/

static
void
test_intarray_basic(
    void)
{
    static const int vals[] = { 0, 1, 2 };
    GUtilIntArray* a = gutil_int_array_new();
    GUtilInts* ints;

    g_assert(gutil_int_array_append(a, 0) == a);
    g_assert(a->count == 1);
    g_assert(a->data[0] == 0);

    g_assert(gutil_int_array_append(a, 1) == a);
    g_assert(a->count == 2);
    g_assert(a->data[0] == 0);
    g_assert(a->data[1] == 1);

    g_assert(gutil_int_array_prepend(a, 2) == a);
    g_assert(a->count == 3);
    g_assert(a->data[0] == 2);
    g_assert(a->data[1] == 0);
    g_assert(a->data[2] == 1);

    gutil_int_array_sort_ascending(a);
    g_assert(a->data[0] == 0);
    g_assert(a->data[1] == 1);
    g_assert(a->data[2] == 2);

    gutil_int_array_sort_descending(a);
    g_assert(a->data[0] == 2);
    g_assert(a->data[1] == 1);
    g_assert(a->data[2] == 0);

    g_assert(gutil_int_array_remove_index_fast(a, 0) == a);
    g_assert(a->count == 2);
    g_assert(a->data[0] == 0);
    g_assert(a->data[1] == 1);

    g_assert(gutil_int_array_remove_range(a, 1, 2) == a);
    g_assert(a->count == 1);
    g_assert(a->data[0] == 0);

    /* This removes nothing (position out of range) */
    g_assert(gutil_int_array_remove_range(a, 1, 2) == a);
    g_assert(a->count == 1);
    g_assert(a->data[0] == 0);

    /* This too (zero count) */
    g_assert(gutil_int_array_remove_range(a, 0, 0) == a);
    g_assert(a->count == 1);
    g_assert(a->data[0] == 0);

    g_assert(gutil_int_array_remove_range(a, 0, 1) == a);
    g_assert(a->count == 0);

    g_assert(gutil_int_array_insert(a, 0, 0) == a);
    g_assert(gutil_int_array_insert(a, 0, 1) == a);
    g_assert(a->count == 2);
    g_assert(a->data[0] == 1);
    g_assert(a->data[1] == 0);

    g_assert(gutil_int_array_insert(a, 2, 2) == a);
    g_assert(a->count == 3);
    g_assert(a->data[0] == 1);
    g_assert(a->data[1] == 0);
    g_assert(a->data[2] == 2);

    g_assert(gutil_int_array_set_count(a, 2) == a);
    g_assert(a->count == 2);
    g_assert(a->data[0] == 1);
    g_assert(a->data[1] == 0);

    g_assert(gutil_int_array_remove_index(a, 0) == a);
    g_assert(a->count == 1);
    g_assert(a->data[0] == 0);

    g_assert(gutil_int_array_remove_index(a, 1) == a);
    g_assert(a->count == 1);
    g_assert(a->data[0] == 0);

    gutil_int_array_ref(a);
    gutil_int_array_free(a, TRUE);
    g_assert(!a->data);
    g_assert(!a->count);
    gutil_int_array_unref(a);

    a = gutil_int_array_sized_new(0);
    g_assert(!gutil_int_array_free(a, FALSE));

    a = gutil_int_array_sized_new(0);
    g_assert(!gutil_int_array_free_to_ints(a));

    a = gutil_int_array_sized_new(G_N_ELEMENTS(vals));
    g_assert(gutil_int_array_append_vals(a, vals, G_N_ELEMENTS(vals)) == a);
    g_assert(a->count == G_N_ELEMENTS(vals));
    g_assert(!memcmp(a->data, vals, sizeof(vals)));
    ints = gutil_int_array_free_to_ints(a);
    g_assert(gutil_ints_get_count(ints) == G_N_ELEMENTS(vals));
    g_assert(!memcmp(gutil_ints_get_data(ints, NULL), vals, sizeof(vals)));
    gutil_ints_unref(ints);
}

/*==========================================================================*
 * Common
 *==========================================================================*/

#define TEST_PREFIX "/intarray/"

int main(int argc, char* argv[])
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func(TEST_PREFIX "null", test_intarray_null);
    g_test_add_func(TEST_PREFIX "basic", test_intarray_basic);
    test_init(&test_opt, argc, argv);
    return g_test_run();
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */

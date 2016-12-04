/*
 * Copyright (C) 2015-2016 Jolla Ltd.
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
 *   3. Neither the name of the Jolla Ltd nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
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

#include "gutil_strv.h"
#include "gutil_log.h"

static TestOpt test_opt;

/*==========================================================================*
 * Basic
 *==========================================================================*/

static
void
test_basic(
    void)
{
    char** sv = g_strsplit("a,b", ",", 0);

    g_assert(gutil_strv_length(NULL) == 0);
    g_assert(gutil_strv_length(sv) == 2);
    g_assert(!g_strcmp0(gutil_strv_at(sv, 0), "a"));
    g_assert(!g_strcmp0(gutil_strv_at(sv, 1), "b"));
    g_assert(!gutil_strv_at(sv, 2));
    g_assert(!gutil_strv_at(sv, 3));
    g_assert(!gutil_strv_at(NULL, 0));

    g_strfreev(sv);
}

/*==========================================================================*
 * Equal
 *==========================================================================*/

static
void
test_equal(
    void)
{
    /* gutil_strv_add(NULL, NULL) is a nop */
    char** sv1 = gutil_strv_add(gutil_strv_add(gutil_strv_add(gutil_strv_add(
        gutil_strv_add(gutil_strv_add(NULL, NULL), "a"), "b"), "c"), " "), "");
    char** sv2 = g_strsplit("a,b,c, ,", ",", 0);
    char** sv3 = g_strsplit("a,a,a, ,", ",", 0);
    char** sv4 = g_strsplit("a,b,c,,", ",", 0);
    char** sv5 = g_strsplit("a,b,c,", ",", 0);

    g_assert(gutil_strv_equal(sv1, sv2));
    g_assert(!gutil_strv_equal(sv1, sv3));
    g_assert(!gutil_strv_equal(sv1, sv4));
    g_assert(!gutil_strv_equal(sv1, sv5));

    g_strfreev(sv1);
    g_strfreev(sv2);
    g_strfreev(sv3);
    g_strfreev(sv4);
    g_strfreev(sv5);
}

/*==========================================================================*
 * Find
 *==========================================================================*/

static
void
test_find(
    void)
{
    char** sv = g_strsplit("a,b,b,c", ",", 0);

    g_assert(gutil_strv_contains(sv, "a"));
    g_assert(gutil_strv_contains(sv, "b"));
    g_assert(gutil_strv_contains(sv, "c"));
    g_assert(!gutil_strv_contains(sv, "d"));
    g_assert(gutil_strv_find(sv, "b") == 1);
    g_assert(!gutil_strv_contains(NULL, "a"));
    g_assert(!gutil_strv_contains(NULL, NULL));
    g_strfreev(sv);
}

/*==========================================================================*
 * Remove
 *==========================================================================*/

static
void
test_remove(
    void)
{
    char** sv = g_strsplit("a,b,c", ",", 0);
    char* c = sv[2];

    g_assert(!gutil_strv_remove_at(NULL, 0, FALSE));
    g_assert(gutil_strv_remove_at(sv, 3, FALSE) == sv);
    g_assert(gutil_strv_remove_at(sv, -1, FALSE) == sv);

    sv = gutil_strv_remove_at(sv, 2, FALSE);
    g_assert(!gutil_strv_contains(sv, "c"));

    sv = gutil_strv_remove_at(sv, 0, TRUE);
    g_assert(!gutil_strv_contains(sv, "a"));
    g_assert(gutil_strv_length(sv) == 1);

    g_free(c);
    g_strfreev(sv);
}

/*==========================================================================*
 * Sort
 *==========================================================================*/

static
void
test_sort(
    void)
{
    char** in = g_strsplit("c,a,d,b", ",", 0);
    char** a1 = g_strsplit("a,b,c,d", ",", 0);
    char** d1 = g_strsplit("d,c,b,a", ",", 0);
    char** a2 = gutil_strv_sort(g_strdupv(in), TRUE);
    char** d2 = gutil_strv_sort(g_strdupv(in), FALSE);

    g_assert(gutil_strv_equal(a1, a2));
    g_assert(gutil_strv_equal(d1, d2));
    g_assert(!gutil_strv_sort(NULL, FALSE));

    g_strfreev(a1);
    g_strfreev(a2);
    g_strfreev(d1);
    g_strfreev(d2);
    g_strfreev(in);
}

/*==========================================================================*
 * Common
 *==========================================================================*/

#define TEST_PREFIX "/strv/"

int main(int argc, char* argv[])
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func(TEST_PREFIX "basic", test_basic);
    g_test_add_func(TEST_PREFIX "equal", test_equal);
    g_test_add_func(TEST_PREFIX "find", test_find);
    g_test_add_func(TEST_PREFIX "remove", test_remove);
    g_test_add_func(TEST_PREFIX "sort", test_sort);
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

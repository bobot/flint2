/*
    Copyright (C) 2009 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "ulong_extras.h"

int main(void)
{
    int i, j, result;
    FLINT_TEST_INIT(state);
    

    flint_printf("factor....");
    fflush(stdout);

    /* Test random numbers */
    for (i = 0; i < 1000 * flint_test_multiplier(); i++)
    {
        mp_limb_t n1, n2;
        n_factor_t factors;

        n_factor_init(&factors);

        n1 = n_randtest_not_zero(state);
        n_factor(&factors, n1, 0);

        n2 = UWORD(1);
        for (j = 0; j < factors.num; j++)
        {
            n2 *= n_pow(factors.p[j], factors.exp[j]);
        }

        result = (n1 == n2);
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("n1 = %wu, n2 = %wu\n", n1, n2); 
            fflush(stdout);
            flint_abort();
        }
    }

    {
        mp_limb_t n1, n2;
        n_factor_t factors;

        n_factor_init(&factors);

        n1 = UWORD(4253857039);
        n_factor(&factors, n1, 0);

        n2 = UWORD(1);
        for (j = 0; j < factors.num; j++)
        {
            n2 *= n_pow(factors.p[j], factors.exp[j]);
        }

        result = (n1 == n2);
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("n1 = %wu, n2 = %wu\n", n1, n2); 
            fflush(stdout);
            flint_abort();
        }
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}

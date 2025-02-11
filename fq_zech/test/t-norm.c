/*
    Copyright (C) 2012 Sebastian Pancratz 
    Copyright (C) 2012 Andres Goens
    Copyright (C) 2013 Mike Hansen

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

#include "fq_zech.h"
#include "ulong_extras.h"
#include "long_extras.h"

int
main(void)
{
    int j, i, result;
    fq_zech_ctx_t ctx;
    FLINT_TEST_INIT(state);
    
    flint_printf("norm... ");
    fflush(stdout);

    for (j = 0; j < 50; j++)
    {
        fq_zech_ctx_randtest(ctx, state);

        for (i = 0; i < 200; i++)
        {
            fmpz_t t1, t2;
            fq_nmod_t aa;
            fq_zech_t a;

            fmpz_init(t1);
            fmpz_init(t2);
            
            fq_nmod_init(aa, ctx->fq_nmod_ctx);
            fq_zech_init(a, ctx);

            fq_nmod_randtest(aa, state, ctx->fq_nmod_ctx);
            fq_zech_set_fq_nmod(a, aa, ctx);

            fq_nmod_norm(t1, aa, ctx->fq_nmod_ctx);
            fq_zech_norm(t2, a, ctx);

            result = fmpz_equal(t1, t2);
            if (!result)
            {
                flint_printf("FAIL:\n\n");
                fq_zech_ctx_print(ctx);
                flint_printf("\n");
                flint_printf("aa = ");
                fq_nmod_print_pretty(aa, ctx->fq_nmod_ctx);
                flint_printf("\n");
                flint_printf("N(aa) = "); fmpz_print(t1);
                flint_printf("\na = ");
                fq_zech_print_pretty(a, ctx);
                flint_printf("\n");
                flint_printf("N(a) = "); fmpz_print(t2);
                flint_printf("\n");
                fflush(stdout);
                flint_abort();
            }

            fq_zech_clear(a, ctx);
            fq_nmod_clear(aa, ctx->fq_nmod_ctx);
            fmpz_clear(t1);
            fmpz_clear(t2);
        }

        fq_zech_ctx_clear(ctx);
    }


    FLINT_TEST_CLEANUP(state);
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}

/*
    Copyright (C) 2011 Fredrik Johansson
    Copyright (C) 2013 Mike Hansen
    Copyright (C) 2021 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "fmpz_mod_mat.h"

static void _apply_permutation(slong * AP,
                   fmpz_mod_mat_t A, slong * P, slong n, slong offset)
{
    if (n != 0)
    {
        fmpz ** Atmp;
        slong *APtmp;
        slong i;

        Atmp = flint_malloc(sizeof(fmpz *) * n);
        APtmp = flint_malloc(sizeof(slong) * n);

        for (i = 0; i < n; i++)
            Atmp[i] = A->mat->rows[P[i] + offset];
        for (i = 0; i < n; i++)
            A->mat->rows[i + offset] = Atmp[i];

        for (i = 0; i < n; i++)
            APtmp[i] = AP[P[i] + offset];
        for (i = 0; i < n; i++)
            AP[i + offset] = APtmp[i];

        flint_free(Atmp);
        flint_free(APtmp);
    }
}

slong fmpz_mod_mat_lu_recursive(slong * P, fmpz_mod_mat_t A, int rank_check)
{

    slong i, j, m, n, r1, r2, n1;
    fmpz_mod_mat_t A0, A1, A00, A01, A10, A11;
    slong *P1;

    m = A->mat->r;
    n = A->mat->c;

    if (m < FMPZ_MOD_MAT_LU_RECURSIVE_CUTOFF ||
        n < FMPZ_MOD_MAT_LU_RECURSIVE_CUTOFF)
    {
        r1 = fmpz_mod_mat_lu_classical(P, A, rank_check);
        return r1;
    }

    n1 = n / 2;

    for (i = 0; i < m; i++)
        P[i] = i;

    P1 = flint_malloc(sizeof(slong) * m);
    fmpz_mod_mat_window_init(A0, A, 0, 0, m, n1);
    fmpz_mod_mat_window_init(A1, A, 0, n1, m, n);

    r1 = fmpz_mod_mat_lu(P1, A0, rank_check);

    if (rank_check && (r1 != n1))
    {
        flint_free(P1);
        fmpz_mod_mat_window_clear(A0);
        fmpz_mod_mat_window_clear(A1);
        return 0;
    }

    if (r1 != 0)
    {
        _apply_permutation(P, A, P1, m, 0);
    }

    fmpz_mod_mat_window_init(A00, A, 0, 0, r1, r1);
    fmpz_mod_mat_window_init(A10, A, r1, 0, m, r1);
    fmpz_mod_mat_window_init(A01, A, 0, n1, r1, n);
    fmpz_mod_mat_window_init(A11, A, r1, n1, m, n);

    if (r1 != 0)
    {
        fmpz_mod_mat_solve_tril(A01, A00, A01, 1);
        fmpz_mod_mat_submul(A11, A11, A10, A01);
    }

    r2 = fmpz_mod_mat_lu(P1, A11, rank_check);

    if (rank_check && (r1 + r2 < FLINT_MIN(m, n)))
    {
        r1 = r2 = 0;
    }
    else
    {
        _apply_permutation(P, A, P1, m - r1, r1);

        /* Compress L */
        if (r1 != n1)
        {
            for (i = 0; i < m - r1; i++)
            {
                fmpz * row = A->mat->rows[r1 + i];
                for (j = 0; j < FLINT_MIN(i, r2); j++)
                {
                    fmpz_set(row + r1 + j, row + n1 + j);
                    fmpz_zero(row + n1 + j);
                }
            }
        }
    }

    flint_free(P1);
    fmpz_mod_mat_window_clear(A00);
    fmpz_mod_mat_window_clear(A01);
    fmpz_mod_mat_window_clear(A10);
    fmpz_mod_mat_window_clear(A11);
    fmpz_mod_mat_window_clear(A0);
    fmpz_mod_mat_window_clear(A1);

    return r1 + r2;
}


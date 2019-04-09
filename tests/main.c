/*
** Created by doom on 08/04/19.
*/

#include "unit_tests.h"

ut_declare_group(core);

int main(void)
{
    ut_run_group(ut_get_group(core));
    return 0;
}

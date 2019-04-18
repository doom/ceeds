/*
** Created by doom on 08/04/19.
*/

#include "unit_tests.h"

ut_declare_group(core);
ut_declare_group(list);
ut_declare_group(bitmanip);
ut_declare_group(ascii_set);
ut_declare_group(string_utils);
ut_declare_group(memory);

int main(void)
{
    ut_run_group(ut_get_group(core));
    ut_run_group(ut_get_group(list));
    ut_run_group(ut_get_group(bitmanip));
    ut_run_group(ut_get_group(ascii_set));
    ut_run_group(ut_get_group(string_utils));
    ut_run_group(ut_get_group(memory));
    return 0;
}

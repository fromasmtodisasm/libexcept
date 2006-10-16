
#include <stdlib.h>
#include <libexcept.h>
#include <assert.h>

ExceptionType FruitException = EXCEPTION_TYPE(BaseException);
ExceptionType OrangeException = EXCEPTION_TYPE(FruitException);
ExceptionType AppleException = EXCEPTION_TYPE(FruitException);
ExceptionType SatsumaException = EXCEPTION_TYPE(OrangeException);

/* Basic try.. catch */

void test_basic_catch(void)
{
        int caught = 0;

        except_try {
                except_raise(OrangeException, NULL);
        } except_catch(OrangeException, e) {
                caught = 1;
        } except_end

        assert(caught);
}

void test_types_pt2(int *caught)
{
        *caught = 0;

        except_try {
                except_raise(AppleException, NULL);
        } except_catch(OrangeException, e) {
                *caught = 1;
        } except_end
}

void test_types(void)
{
        int caught;
        
        except_try {
                test_types_pt2(&caught);
        } except_catch(BaseException, e) {

        } except_end

        /* Different type: should not be caught */
       
        assert(!caught);
}

/* test of the exception subtyping system */

void test_subtypes(void)
{
        int caught = 0;

        except_try {
                except_raise(SatsumaException, NULL);
        } except_catch(OrangeException, e) {
                caught = 1;
        } except_end

        assert(caught);
}

/* test the finally {} functionality */

void test_finally_pt2(int *ran_finally)
{
        except_try {
                except_raise(AppleException, NULL);
        } except_finally {
                *ran_finally = 1;
        } except_end
}

void test_finally(void)
{
        int ran_finally;
        int caught;

        /* In normal conditions, finally should be run after the try block */

        ran_finally = 0;

        except_try {

        } except_finally {
                ran_finally = 1;
        } except_end
        
        assert(ran_finally);

        /* If we catch an exception, finally should run */

        ran_finally = 0;

        except_try {
                except_raise(AppleException, NULL);
        } except_catch(AppleException, e) {

        } except_finally {
                ran_finally = 1;
        } except_end

        assert(ran_finally);

        /* If we have try .. finally with no catch, the finally block
         * should run and the exception be passed higher up the stack. */

        caught = 0;
        ran_finally = 0;

        except_try {
                test_finally_pt2(&ran_finally);
        } except_catch(AppleException, e) {
                caught = 1;
        } except_end

        assert(caught && ran_finally);
}

/* test rethrowing an exception after it is caught */

void test_rethrow_pt2(int *caught_1, int *finally_1)
{
        except_try {
                except_raise(AppleException, NULL);
        } except_catch(OrangeException, e) {
                assert(0);
        } except_catch(AppleException, e) {
                *caught_1 = 1;

                except_throw(e);
        } except_finally {
                *finally_1 = 1;
        } except_end

        assert(0);
}

void test_rethrow(void)
{
        int caught_1, caught_2;
        int finally_1, finally_2;

        caught_1 = 0;
        caught_2 = 0;
        finally_1 = 0;
        finally_2 = 0;

        except_try {
                test_rethrow_pt2(&caught_1, &finally_1);
        } except_catch(OrangeException, e) {
                assert(0);
        } except_catch(AppleException, e) {
                caught_2 = 1;
        } except_finally {
                finally_2 = 1;
        } except_end

        assert(caught_1 && caught_2 && finally_1 && finally_2);
}

/* test throw inside finally block */

void test_throw_in_finally_pt2(int *caught)
{
        except_try {
                except_raise(OrangeException, NULL);
        } except_catch(AppleException, e) {
                assert(0);
        } except_catch(OrangeException, e) {
                *caught = 1;
        } except_finally {
                except_raise(AppleException, NULL);
        } except_end

        assert(0);
}

void test_throw_in_finally(void)
{
        int caught_1, caught_2;
        int i;

        for (i=0; i<10; ++i) {
                
                caught_1 = 0;
                caught_2 = 0;

                except_try {
                        test_throw_in_finally_pt2(&caught_1);
                } except_catch(AppleException, e) {
                        caught_2 = 1;
                } except_end;

                assert(caught_1 && caught_2);
        }
}

/* test for except_assert */

void test_assert(void)
{
        int caught = 0;

        except_try {
                except_assert(1 == 2);
        } except_catch(AssertException, e) {
                caught = 1;
        } except_end

        assert(caught);
}

int main(int argc, char *argv[])
{
        test_basic_catch();
        test_types();
        test_subtypes();
        test_finally();
        test_rethrow();
        test_throw_in_finally();
        test_assert();

        return 0;
}


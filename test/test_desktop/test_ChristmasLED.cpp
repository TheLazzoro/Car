#include <unity.h>
#include <ChristmasLED.h>

void test_add_christmas_light(void)
{
	ChristmasLED one(D0);
	ChristmasLED two(D1);
    int expected = 2;
    int actual = ChristmasLED::Count();
    TEST_ASSERT_EQUAL(expected, actual);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_add_christmas_light);
    UNITY_END();

    return 0;
}

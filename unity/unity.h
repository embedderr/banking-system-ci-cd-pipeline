#ifndef UNITY_H
#define UNITY_H

#define UNITY_BEGIN()         UnityBegin(__FILE__)
#define UNITY_END()           UnityEnd()
#define RUN_TEST(test)        UnityDefaultTestRunner(test, #test, __FILE__, __LINE__)

void UnityBegin(const char* file);
int  UnityEnd(void);                    // Changed to return int
void UnityDefaultTestRunner(void (*test)(void), const char* name, const char* file, int line);

#define TEST_ASSERT_EQUAL_INT(expected, actual)   UnityAssertEqualInt((expected), (actual), __FILE__, __LINE__)
#define TEST_ASSERT_EQUAL_FLOAT(expected, actual) UnityAssertEqualFloat((expected), (actual), __FILE__, __LINE__)

void UnityAssertEqualInt(int expected, int actual, const char* file, int line);
void UnityAssertEqualFloat(float expected, float actual, const char* file, int line);

#endif
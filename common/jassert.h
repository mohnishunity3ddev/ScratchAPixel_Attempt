#if !defined J_ASSERT_H
#define J_ASSERT_H

#define Assert(expression)                                                     \
    {                                                                          \
        if (!(expression))                                                     \
        {                                                                      \
            int *P = 0;                                                        \
            *P     = 0;                                                        \
        }                                                                      \
    }

#endif
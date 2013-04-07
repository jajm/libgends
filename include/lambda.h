#ifndef gds_lambda_h_included
#define gds_lambda_h_included

/**
 * Lambda (anonymous) function
 *
 * Example of usage:
 *
 *   void my_func( int (*callback)(int) );
 *
 *   my_func(lambda(int, (int a) {
 *           printf("%d\n", a);
 *           return 0;
 *   }));
 */
#define lambda(ret_type, args_and_code) ({ \
	ret_type gds_anonymous_function args_and_code \
	&gds_anonymous_function; \
})

#endif /* ! gds_lambda_h_included */

/*
 * Copyright 2013 Julian Maurice
 *
 * This file is part of libgends
 *
 * libgends is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libgends is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef gds_lambda_h_included
#define gds_lambda_h_included

/*
 * Lambda (anonymous) function
 *
 * Example of usage:
 *
 *   void my_func( int (*callback)(int) );
 *
 *   my_func(gds_lambda(int, (int a) {
 *           printf("%d\n", a);
 *           return 0;
 *   }));
 */
#define gds_lambda(ret_type, args_and_code) ({ \
	ret_type gds_anonymous_function args_and_code \
	&gds_anonymous_function; \
})

#endif /* ! gds_lambda_h_included */

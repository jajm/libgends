/*****************************************************************************
 * Copyright (C) 2010 Julian Maurice                                         *
 *                                                                           *
 * This file is part of libgends.                                            *
 *                                                                           *
 * libgends is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * libgends is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.         *
 *****************************************************************************/

/*****************************************************************************
 * File                 : error.h                                            *
 * Short description    : Error management                                   *
 * Author               : Julian Maurice                                     *
 * Created on           : 2010-03-01                                         *
 *****************************************************************************
 * An error is a message, and a triplet (filename, line, function) where the *
 * error occurs.                                                             *
 * You can register an error by calling Error, or Error_at, and then print   *
 * with pError.                                                              *
 * Only one error is kept in memory. To display more than one error at a time*
 * you can use ErrorP which place the error message before the preceding     *
 * messages.                                                                 *
 *****************************************************************************/

#ifndef error_h_included
#define error_h_included

#include "basic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Register an error */
void Error_at(const char *file, u32 line, const char *function,
	const char *msg, ...);
#define Error(errmsg, ...) \
	Error_at(__FILE__, __LINE__, __func__, errmsg, ##__VA_ARGS__)
#define ErrorP(errmsg, ...) \
	Error(errmsg "\n -> %s", ##__VA_ARGS__, Error_string())

/* Print the last registered error in the following format:
 * Error in function at file:line: errmsg
 * If ErrorP was used, error message will be:
 * ErrorN in function at file:line: errmsg
 *  -> ErrorN-1 in function at file:line: errmsg
 *  -> ...                                                   */
void pError(void);

/* You can also get only part of the last registered error */
char * Error_msg(void);
char * Error_file(void);
char * Error_function(void);
u32 Error_line(void);
/* Returns the string as printed by pError */
char * Error_string(void);

#ifdef __cplusplus
}
#endif

#endif /* Not error_h_included */


/* C64 NonSense, just playing around with C64 cross-compile tools.
 * Copyright (C) 2020  Dirk "YouDirk" Lehmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef DEFINE_H__
#define DEFINE_H__

#if !defined(GEN_C_HEADER) && !defined(GEN_ASM_HEADER)
#error "Do not include this file outside from *.def.h files!  These" \
  " will be used to generate *.gen.h and *.gen.s files!  Include" \
  " *.gen.[hs] instead."
#endif

#define HASH                            #

#if defined(GEN_C_HEADER)
# define BC /##*

BC ******************************************************************
 *
 * !!! THIS FILE WAS GENERATED !!!
 *
 * Do not change it, change the corresponding %.def.h file instead!
 *
 * *******************************************************************
 *
 */

#  define define(name, value) HASH define name value

#  define define_hex(name, value) define(name, 0x##value)
#  define define_dec(name, value) define(name, value)

#elif defined(GEN_ASM_HEADER)

;; *******************************************************************
;;
;; !!! THIS FILE WAS GENERATED !!!
;;
;; Do not change it, change the corresponding %.def.h file instead!
;;
;; *******************************************************************
;
;

#  define define(name, value) .define name value

#  define define_hex(name, value) define(name, $value)
#  define define_dec(name, value) define(name, value)

#else /* defined(GEN_C_HEADER)  */
#  error "Do not include this file outside from *.def.h files!"
#endif /* defined(GEN_C_HEADER)  */

#endif /* DEFINE_H__  */

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

#  define header_ifndef(name)      HASH ifndef name##_GEN_H__
#  define header_define(name)      HASH define name##_GEN_H__
#  define header_endif(name)       HASH endif    BC name##_GEN_H__  */

#  define _define(name, value)     HASH define name value

#  define define(name, value)      _define(name, (value))
#  define define_hex(name, value)  _define(name, 0x##value)
#  define define_dec(name, value)  _define(name, value)

#  define macro_arg1(name, value)      HASH define name(arg1) (value)
#  define macro_arg1_arg2(name, value) HASH define name(arg1, arg2)  \
                                       (value)

#  define typedef_struct_begin(name)    typedef struct name {
#  define typedef_struct_end(name)      } name;
#  define typedef_struct_nested(other, name)      other name;
#  define typedef_struct_nested_ptr(other, name)  other* name;
#  define typedef_struct_enum(other, name)        other name;
#  define typedef_struct_enum_ptr(other, name)    other* name;
#  define typedef_struct_bool(name)               bool name;
#  define typedef_struct_bool_ptr(name)           bool* name;
#  define typedef_struct_uint8(name)              uint8_t name;
#  define typedef_struct_uint8_ptr(name)          uint8_t* name;
#  define typedef_struct_uint16(name)             uint16_t name;
#  define typedef_struct_uint16_ptr(name)         uint16_t* name;
#  define typedef_struct_uint32(name)             uint32_t name;
#  define typedef_struct_uint32_ptr(name)         uint32_t* name;
#  define typedef_struct_int8(name)               int8_t name;
#  define typedef_struct_int8_ptr(name)           int8_t* name;
#  define typedef_struct_int16(name)              int16_t name;
#  define typedef_struct_int16_ptr(name)          int16_t* name;
#  define typedef_struct_int32(name)              int32_t name;
#  define typedef_struct_int32_ptr(name)          int32_t* name;
#  define typedef_struct_void_ptr(name)           void* name;
#  define typedef_struct_char_ptr(name)           char* name;
#  define typedef_struct_constchar_ptr(name)      const char* name;

#  define typedef_struct_nested_array(other, name, size)             \
                                        other name[size];
#  define typedef_struct_enum_array(other, name, size)               \
                                        other name[size];

#  define typedef_enum_begin(name)                typedef uint8_t name;
#  define typedef_enum_end(name)                  BC enum name  */
#  define typedef_enum_hex(type, name, value) \
                                   _define(name, ((type) (0x##value)))
#  define typedef_enum_dec(type, name, value) \
                                   _define(name, ((type) (value)))

#  define extern_var(type, name)                  extern type name;

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

#  define header_ifndef(name)           .ifnblank name##_GEN_S__
#  define header_define(name)           .define name##_GEN_S__
#  define header_endif(name)            .endif    ; name##_GEN_S__

#  define _define(name, value)          .define name value

#  define define(name, value)           _define(name, value)
#  define define_hex(name, value)       _define(name, $value)
#  define define_dec(name, value)       _define(name, value)

#  define macro_arg1(name, value)       .define name(arg1) value
#  define macro_arg1_arg2(name, value)  .define name(arg1, arg2) value

#  define typedef_struct_begin(name)    .struct name
#  define typedef_struct_end(name)      .endstruct ; struct name
#  define typedef_struct_nested(other, name)      name .tag other
#  define typedef_struct_nested_ptr(other, name)  name .addr  ; other*
#  define typedef_struct_enum(other, name)        name .byte  ; other
#  define typedef_struct_enum_ptr(other, name)    name .addr  ; other*
#  define typedef_struct_bool(name)               name .byte  ; bool
#  define typedef_struct_bool_ptr(name)           name .addr  ; bool*
#  define typedef_struct_uint8(name)              name .byte  ; uint8
#  define typedef_struct_uint8_ptr(name)          name .addr  ; uint8*
#  define typedef_struct_uint16(name)             name .word  ; uint16
#  define typedef_struct_uint16_ptr(name)         name .addr  ; uint16*
#  define typedef_struct_uint32(name)             name .dword ; uint32
#  define typedef_struct_uint32_ptr(name)         name .addr  ; uint32*
#  define typedef_struct_int8(name)               name .byte  ; int8
#  define typedef_struct_int8_ptr(name)           name .addr  ; int8*
#  define typedef_struct_int16(name)              name .word  ; int16
#  define typedef_struct_int16_ptr(name)          name .addr  ; int16*
#  define typedef_struct_int32(name)              name .dword ; int32
#  define typedef_struct_int32_ptr(name)          name .addr  ; int32*
#  define typedef_struct_void_ptr(name)           name .addr  ; void*
#  define typedef_struct_char_ptr(name)           name .addr  ; char*
#  define typedef_struct_constchar_ptr(name)      name .addr ; const char*

#  define typedef_struct_nested_array(other, name, size)             \
                                        name .tag other (size)
#  define typedef_struct_enum_array(other, name, size)               \
                                        name .byte (size) ; other

#  define typedef_enum_begin(name)      _define(name, SIZEOF_BYTE)
#  define typedef_enum_end(name)        ; enum name
#  define typedef_enum_hex(type, name, value)     _define(name, $value)
#  define typedef_enum_dec(type, name, value)     _define(name, value)

#  define extern_var(type, name)                  .import _##name

#else /* defined(GEN_C_HEADER)  */
#  error "Do not include this file outside from *.def.h files!"
#endif /* defined(GEN_C_HEADER)  */

#endif /* DEFINE_H__  */

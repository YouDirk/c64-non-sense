/* C64 NonSense, the C64 Game Engine.
 * Copyright (C) 2020-2021  Dirk "YouDirk" Lehmann
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

#  define prep_error(msg)          HASH error msg

#  define prep_if(cond)            HASH if cond
#  define prep_elif(cond)          HASH elif cond
#  define prep_else(comment)       HASH else BC comment  */
#  define prep_endif(comment)      HASH endif BC comment  */

#  define equals                   ==

#  define _include_def_h(module_h) HASH include #module_h
#  define include_def_h(module)    _include_def_h(module.gen.h)

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
#  define typedef_struct_char(name)               char name;
#  define typedef_struct_constchar(name)          const char name;
#  define typedef_struct_uint15(name)             uint15_t name;
#  define typedef_struct_uint14(name)             uint14_t name;
#  define typedef_struct_uint13(name)             uint13_t name;
#  define typedef_struct_uint12(name)             uint12_t name;
#  define typedef_struct_uint11(name)             uint11_t name;
#  define typedef_struct_uint10(name)             uint10_t name;
#  define typedef_struct_uint9(name)              uint9_t name;
#  define typedef_struct_uint8(name)              uint8_t name;
#  define typedef_struct_uint7(name)              uint7_t name;
#  define typedef_struct_uint6(name)              uint6_t name;
#  define typedef_struct_uint5(name)              uint5_t name;
#  define typedef_struct_uint4(name)              uint4_t name;
#  define typedef_struct_uint3(name)              uint3_t name;
#  define typedef_struct_uint2(name)              uint2_t name;
#  define typedef_struct_uint8_ptr(name)          uint8_t* name;
#  define typedef_struct_uint16(name)             uint16_t name;
#  define typedef_struct_uint16_ptr(name)         uint16_t* name;
#  define typedef_struct_uint32(name)             uint32_t name;
#  define typedef_struct_uint32_ptr(name)         uint32_t* name;
#  define typedef_struct_int15(name)              int15_t name;
#  define typedef_struct_int14(name)              int14_t name;
#  define typedef_struct_int13(name)              int13_t name;
#  define typedef_struct_int12(name)              int12_t name;
#  define typedef_struct_int11(name)              int11_t name;
#  define typedef_struct_int10(name)              int10_t name;
#  define typedef_struct_int9(name)               int9_t name;
#  define typedef_struct_int8(name)               int8_t name;
#  define typedef_struct_int7(name)               int7_t name;
#  define typedef_struct_int6(name)               int6_t name;
#  define typedef_struct_int5(name)               int5_t name;
#  define typedef_struct_int4(name)               int4_t name;
#  define typedef_struct_int3(name)               int3_t name;
#  define typedef_struct_int2(name)               int2_t name;
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

#  define typedef_enum_begin(name)             BC enum name {  */    \
                                               typedef uint8_t name;
#  define typedef_enum_end(name)               BC } enum name;  */
#  define typedef_enum_hex(type, value, name)                        \
                                   _define(name, ((type) (0x##value)))
#  define typedef_enum_dec(type, value, name)                        \
                                   _define(name, ((type) (value)))
#  define typedef_enum_val(type, value, name)                        \
                                   _define(name, ((type) (value)))

#  define extern_var(type, name)                  extern type name;


#  define register_bool_hex(addr_hex, name)                          \
               _define(name, (*(volatile bool*) 0x##addr_hex))
#  define register_bool_ptr_hex(addr_hex, name)                      \
               _define(name, (*(volatile bool**) 0x##addr_hex))
#  define register_char_hex(addr_hex, name)                          \
               _define(name, (*(volatile char*) 0x##addr_hex))
#  define register_constchar_hex(addr_hex, name)                     \
               _define(name, (*(const char*) 0x##addr_hex))
#  define register_uint8_hex(addr_hex, name)                         \
               _define(name, (*(volatile uint8_t*) 0x##addr_hex))
#  define register_uint8_ptr_hex(addr_hex, name)                     \
               _define(name, (*(volatile uint8_t**) 0x##addr_hex))
#  define register_uint16_hex(addr_hex, name)                        \
               _define(name, (*(volatile uint16_t*) 0x##addr_hex))
#  define register_uint16_ptr_hex(addr_hex, name)                    \
               _define(name, (*(volatile uint16_t**) 0x##addr_hex))
#  define register_uint32_hex(addr_hex, name)                        \
               _define(name, (*(volatile uint32_t*) 0x##addr_hex))
#  define register_uint32_ptr_hex(addr_hex, name)                    \
               _define(name, (*(volatile uint32_t**) 0x##addr_hex))
#  define register_int8_hex(addr_hex, name)                          \
               _define(name, (*(volatile int8_t*) 0x##addr_hex))
#  define register_int8_ptr_hex(addr_hex, name)                      \
               _define(name, (*(volatile int8_t**) 0x##addr_hex))
#  define register_int16_hex(addr_hex, name)                         \
               _define(name, (*(volatile int16_t*) 0x##addr_hex))
#  define register_int16_ptr_hex(addr_hex, name)                     \
               _define(name, (*(volatile int16_t**) 0x##addr_hex))
#  define register_int32_hex(addr_hex, name)                         \
               _define(name, (*(volatile int32_t*) 0x##addr_hex))
#  define register_int32_ptr_hex(addr_hex, name)                     \
               _define(name, (*(volatile int32_t**) 0x##addr_hex))
#  define register_void_ptr_hex(addr_hex, name)                      \
               _define(name, (*(volatile void**) 0x##addr_hex))
#  define register_char_ptr_hex(addr_hex, name)                      \
               _define(name, (*(volatile char**) 0x##addr_hex))
#  define register_constchar_ptr_hex(addr_hex, name)                 \
               _define(name, (*(volatile const char**) 0x##addr_hex))

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

#  define prep_error(msg)               .error msg

#  define prep_if(cond)                 .if cond
#  define prep_elif(cond)               .elseif cond
#  define prep_else(comment)            .else ; comment
#  define prep_endif(comment)           .endif ; comment

#  define equals                        =

#  define _include_def_h(module_s)      .include #module_s
#  define include_def_h(module)         _include_def_h(module.gen.s)

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
#  define typedef_struct_char(name)               name .byte  ; char
#  define typedef_struct_constchar(name)          name .byte  ; const char
#  define typedef_struct_uint15(name)             name .word  ; uint15
#  define typedef_struct_uint14(name)             name .word  ; uint14
#  define typedef_struct_uint13(name)             name .word  ; uint13
#  define typedef_struct_uint12(name)             name .word  ; uint12
#  define typedef_struct_uint11(name)             name .word  ; uint11
#  define typedef_struct_uint10(name)             name .word  ; uint10
#  define typedef_struct_uint9(name)              name .word  ; uint9
#  define typedef_struct_uint8(name)              name .byte  ; uint8
#  define typedef_struct_uint7(name)              name .byte  ; uint7
#  define typedef_struct_uint6(name)              name .byte  ; uint6
#  define typedef_struct_uint5(name)              name .byte  ; uint5
#  define typedef_struct_uint4(name)              name .byte  ; uint4
#  define typedef_struct_uint3(name)              name .byte  ; uint3
#  define typedef_struct_uint2(name)              name .byte  ; uint2
#  define typedef_struct_uint8_ptr(name)          name .addr  ; uint8*
#  define typedef_struct_uint16(name)             name .word  ; uint16
#  define typedef_struct_uint16_ptr(name)         name .addr  ; uint16*
#  define typedef_struct_uint32(name)             name .dword ; uint32
#  define typedef_struct_uint32_ptr(name)         name .addr  ; uint32*
#  define typedef_struct_int15(name)              name .word  ; int15
#  define typedef_struct_int14(name)              name .word  ; int14
#  define typedef_struct_int13(name)              name .word  ; int13
#  define typedef_struct_int12(name)              name .word  ; int12
#  define typedef_struct_int11(name)              name .word  ; int11
#  define typedef_struct_int10(name)              name .word  ; int10
#  define typedef_struct_int9(name)               name .word  ; int9
#  define typedef_struct_int8(name)               name .byte  ; int8
#  define typedef_struct_int7(name)               name .byte  ; int7
#  define typedef_struct_int6(name)               name .byte  ; int6
#  define typedef_struct_int5(name)               name .byte  ; int5
#  define typedef_struct_int4(name)               name .byte  ; int4
#  define typedef_struct_int3(name)               name .byte  ; int3
#  define typedef_struct_int2(name)               name .byte  ; int2
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

#  define typedef_enum_begin(name)      _define(name, SIZEOF_BYTE)   \
                                        ; enum name {
#  define typedef_enum_end(name)        ; } enum name;
#  define typedef_enum_hex(type, value, name)     _define(name, $value)
#  define typedef_enum_dec(type, value, name)     _define(name, value)
#  define typedef_enum_val(type, value, name)     _define(name, value)

#  define extern_var(type, name)                  .import _##name

#  define register_bool_hex(addr_hex, name)                          \
                                            define_hex(name, addr_hex)
#  define register_bool_ptr_hex(addr_hex, name)                      \
                                            define_hex(name, addr_hex)
#  define register_char_hex(addr_hex, name)                          \
                                            define_hex(name, addr_hex)
#  define register_constchar_hex(addr_hex, name)                     \
                                            define_hex(name, addr_hex)
#  define register_uint8_hex(addr_hex, name)                         \
                                            define_hex(name, addr_hex)
#  define register_uint8_ptr_hex(addr_hex, name)                     \
                                            define_hex(name, addr_hex)
#  define register_uint16_hex(addr_hex, name)                        \
                                            define_hex(name, addr_hex)
#  define register_uint16_ptr_hex(addr_hex, name)                    \
                                            define_hex(name, addr_hex)
#  define register_uint32_hex(addr_hex, name)                        \
                                            define_hex(name, addr_hex)
#  define register_uint32_ptr_hex(addr_hex, name)                    \
                                            define_hex(name, addr_hex)
#  define register_int8_hex(addr_hex, name)                          \
                                            define_hex(name, addr_hex)
#  define register_int8_ptr_hex(addr_hex, name)                      \
                                            define_hex(name, addr_hex)
#  define register_int16_hex(addr_hex, name)                         \
                                            define_hex(name, addr_hex)
#  define register_int16_ptr_hex(addr_hex, name)                     \
                                            define_hex(name, addr_hex)
#  define register_int32_hex(addr_hex, name)                         \
                                            define_hex(name, addr_hex)
#  define register_int32_ptr_hex(addr_hex, name)                     \
                                            define_hex(name, addr_hex)
#  define register_void_ptr_hex(addr_hex, name)                      \
                                            define_hex(name, addr_hex)
#  define register_char_ptr_hex(addr_hex, name)                      \
                                            define_hex(name, addr_hex)
#  define register_constchar_ptr_hex(addr_hex, name)                 \
                                            define_hex(name, addr_hex)

#else /* defined(GEN_C_HEADER)  */
#  error "Do not include this file outside from *.def.h files!"
#endif /* defined(GEN_C_HEADER)  */

#endif /* DEFINE_H__  */

/*
	description:

		"C functions used to implement type information"

	system: "Gobo Eiffel Compiler"
	copyright: "Copyright (c) 2016-2024, Eric Bezault and others"
	license: "MIT License"
*/

#ifndef GE_TYPES_H
#define GE_TYPES_H
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef GE_EIFFEL_H
#include "ge_eiffel.h"
#endif
#ifndef GE_EXCEPTION_H
#include "ge_exception.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Type annotations.
 * When a type has no annotation, it means a detachable, non-separate, variant type.
 * In all other cases, there will be an annotation.
 */
#define ANNOTATION_MASK			0x007F	/* All possible annotations. */
#define ATTACHED_FLAG			0x0001
#define DETACHABLE_FLAG			0x0002	/* Only present when overriding an attached type. */
#define SEPARATE_FLAG			0x0004
#define VARIANT_FLAG			0x0008	/* Only present when overriding a frozen/poly type. */
#define UNUSABLE_FLAG			0x0010	/* Reserved for backward compatibility for storables. */
#define FROZEN_FLAG				0x0020
#define POLY_FLAG				0x0040

/*
 * Type flags.
 */
#define GE_TYPE_FLAG_SPECIAL		0x0010
#define GE_TYPE_FLAG_TUPLE			0x0020
#define GE_TYPE_FLAG_EXPANDED		0x0040
#define GE_TYPE_FLAG_DEFERRED		0x0080
#define GE_TYPE_FLAG_NONE			0x0100
#define GE_TYPE_FLAG_BASIC_MASK		0x000F /* One of "BOOLEAN", "CHARACTER_8", "CHARACTER_32", "INTEGER_8", "INTEGER_16", "INTEGER_32", "INTEGER_64", "NATURAL_8", "NATURAL_16", "NATURAL_32", "NATURAL_64", "POINTER", "REAL_32", "REAL_64" */
#define GE_TYPE_FLAG_BOOLEAN		0x0001
#define GE_TYPE_FLAG_CHARACTER_8	0x0002
#define GE_TYPE_FLAG_CHARACTER_32	0x0003
#define GE_TYPE_FLAG_INTEGER_8		0x0004
#define GE_TYPE_FLAG_INTEGER_16		0x0005
#define GE_TYPE_FLAG_INTEGER_32		0x0006
#define GE_TYPE_FLAG_INTEGER_64		0x0007
#define GE_TYPE_FLAG_NATURAL_8		0x0008
#define GE_TYPE_FLAG_NATURAL_16		0x0009
#define GE_TYPE_FLAG_NATURAL_32		0x000A
#define GE_TYPE_FLAG_NATURAL_64		0x000B
#define GE_TYPE_FLAG_POINTER		0x000C
#define GE_TYPE_FLAG_REAL_32		0x000D
#define GE_TYPE_FLAG_REAL_64		0x000E

/*
 * Convention for attribute types.
 * The values are in sync with REFLECTOR_CONSTANTS.
 */
#define GE_TYPE_KIND_INVALID		-1
#define GE_TYPE_KIND_POINTER		0
#define GE_TYPE_KIND_REFERENCE		1
#define GE_TYPE_KIND_CHARACTER_8	2
#define GE_TYPE_KIND_BOOLEAN		3
#define GE_TYPE_KIND_INTEGER_32		4
#define GE_TYPE_KIND_REAL_32		5
#define GE_TYPE_KIND_REAL_64		6
#define GE_TYPE_KIND_EXPANDED		7
#define GE_TYPE_KIND_INTEGER_8		9
#define GE_TYPE_KIND_INTEGER_16		10
#define GE_TYPE_KIND_INTEGER_64 	11
#define GE_TYPE_KIND_CHARACTER_32	12
#define GE_TYPE_KIND_NATURAL_8		13
#define GE_TYPE_KIND_NATURAL_16		14
#define GE_TYPE_KIND_NATURAL_32 	15
#define GE_TYPE_KIND_NATURAL_64 	16

/*
 * Object flags.
 */
#define GE_OBJECT_FLAG_MARKED		0x0001

/*
 * Ancestor relationship between two types X and Y.
 */
#ifdef GE_USE_ANCESTORS
typedef volatile struct {
	EIF_TYPE_INDEX volatile type_id; /* Type id of Y */
	EIF_BOOLEAN volatile conforms; /* Does X conform to Y? */
	void (**volatile qualified_calls)(); /* Function pointers, indexed by call id, when the static type of the target is Y and the dynamic type is X */
} GE_ancestor;
#endif

/*
 * Attribute.
 */
#ifdef GE_USE_ATTRIBUTES
typedef volatile struct {
#ifdef GE_USE_ATTRIBUTE_NAME
	const char* volatile name; /* Attribute name */
#endif
#ifdef GE_USE_ATTRIBUTE_TYPE_ID
	EIF_ENCODED_TYPE volatile type_id; /* Static type id */
#endif
#ifdef GE_USE_ATTRIBUTE_DYNAMIC_TYPE_SET
	EIF_TYPE_INDEX* volatile dynamic_type_set; /* Dynamic type set */
	uint32_t volatile dynamic_type_count; /* Number of types in `dynamic_type_set` */
#endif
#ifdef GE_USE_ATTRIBUTE_OFFSET
	uint32_t volatile offset; /* Address offset in object */
#endif
#ifdef GE_USE_ATTRIBUTE_SIZE
	uint32_t volatile size; /* Size of attribute in object */
#endif
} GE_attribute;
#endif

/*
 * Type information.
 */
typedef volatile struct {
	EIF_TYPE_INDEX volatile type_id;
	uint16_t volatile flags;
#ifdef GE_USE_TYPE_GENERATOR
	const char* volatile generator; /* Generator class name */
#endif
#ifdef GE_USE_TYPE_NAME
	const char* volatile name; /* Full type name */
#endif
#ifdef GE_USE_TYPE_GENERIC_PARAMETERS
	EIF_ENCODED_TYPE* volatile generic_parameters;
	uint32_t volatile generic_parameter_count;
#endif
#ifdef GE_USE_ANCESTORS
	GE_ancestor** volatile ancestors;
	uint32_t volatile ancestor_count;
#endif
#ifdef GE_USE_ATTRIBUTES
	GE_attribute** volatile attributes;
	uint32_t volatile attribute_count;
#endif
#ifdef GE_USE_TYPE_OBJECT_SIZE
	uint64_t volatile object_size;
#endif
	EIF_REFERENCE (*new_instance)();
	void (*volatile dispose)(GE_context*, EIF_REFERENCE);
} GE_type_info;

typedef volatile struct {
	EIF_TYPE_INDEX volatile id; /* Type id of the "TYPE [X]" object */
	uint16_t volatile flags; 
#ifdef GE_USE_SCOOP
	GE_scoop_region* volatile region;
#endif
	EIF_INTEGER volatile type_id; /* Type id of the type "X" */
	EIF_BOOLEAN volatile is_special;
	void (*volatile dispose)(GE_context*, EIF_REFERENCE);
	EIF_REFERENCE volatile a1; /* internal_name */
	EIF_REFERENCE volatile a2; /* internal_name_32 */
} EIF_TYPE_OBJ;

/*
 * Types indexed by type id.
 * Generated by the compiler.
 */
extern EIF_TYPE_OBJ GE_types[][2];
extern GE_type_info GE_type_infos[];

/*
 * Number of type infos in `GE_type_infos'.
 * Do not take into account the fake item at index 0.
 */
extern int GE_type_info_count;

/*
 * Encode a EIF_TYPE into a EIF_ENCODED_TYPE.
 * The lower part of EIF_ENCODED_TYPE contains the .id field,
 * and the upper part the .annotations.
 */
extern EIF_ENCODED_TYPE GE_encoded_type(EIF_TYPE a_type);

/*
 * Decode a EIF_ENCODED_TYPE into a EIF_TYPE.
 * The lower part of EIF_ENCODED_TYPE contains the .id field,
 * and the upper part the .annotations.
 */
extern EIF_TYPE GE_decoded_type(EIF_ENCODED_TYPE a_type);

/*
 * Type with `a_id' and `a_annotations'.
 */
extern EIF_TYPE GE_new_type(EIF_TYPE_INDEX a_id, EIF_TYPE_INDEX a_annotations);

/*
 * Type of object `obj'.
 */
#define GE_object_type(obj)	GE_new_type(((EIF_REFERENCE)(obj))->id, 0x0)
#define GE_object_encoded_type(obj) GE_encoded_type(GE_object_type(obj))

/*
 * Attachment status of `a_type'.
 */
#define GE_is_attached_type(a_type) EIF_TEST(((a_type).annotations & ATTACHED_FLAG) || GE_is_expanded_type_index((a_type).id))
#define GE_is_attached_encoded_type(a_type) GE_is_attached_type(GE_decoded_type(a_type))

/*
 * Associated detachable type of `a_type' if any,
 * otherwise `a_type'.
 */
extern EIF_TYPE GE_non_attached_type(EIF_TYPE a_type);
#define GE_non_attached_encoded_type(a_type) GE_encoded_type(GE_non_attached_type(GE_decoded_type(a_type)))

/*
 * Associated attached type of `a_type' if any,
 * otherwise `a_type'.
 */
extern EIF_TYPE GE_attached_type(EIF_TYPE a_type);
#define GE_attached_encoded_type(t) GE_encoded_type(GE_attached_type(GE_decoded_type(t)))

/*
 * Is `a_type' a SPECIAL type?
 */
#define GE_is_special_type_index(a_type) EIF_TEST(GE_type_infos[a_type].flags & GE_TYPE_FLAG_SPECIAL)
#define GE_is_special_encoded_type(a_type) GE_is_special_type_index(GE_decoded_type(a_type).id)
#define GE_is_special_object(obj) GE_is_special_type_index(((EIF_REFERENCE)(obj))->id)

/*
 * Is `a_type' a SPECIAL type of user-defined expanded type?
 */
extern EIF_BOOLEAN GE_is_special_of_expanded_type_index(EIF_TYPE_INDEX a_type);
#define GE_is_special_of_expanded_encoded_type(a_type) GE_is_special_of_expanded_type_index(GE_decoded_type(a_type).id)
#define GE_is_special_of_expanded_object(obj) GE_is_special_of_expanded_type_index(((EIF_REFERENCE)(obj))->id)

/*
 * Is `a_type' a SPECIAL type of reference type?
 */
extern EIF_BOOLEAN GE_is_special_of_reference_type_index(EIF_TYPE_INDEX a_type);
#define GE_is_special_of_reference_encoded_type(a_type) GE_is_special_of_reference_type_index(GE_decoded_type(a_type).id)
#define GE_is_special_of_reference_object(obj) GE_is_special_of_reference_type_index(((EIF_REFERENCE)(obj))->id)

/*
 * Is `a_type' a SPECIAL type of reference type or basic expanded type?
 * (Note that user-defined expanded types are excluded.)
 */
extern EIF_BOOLEAN GE_is_special_of_reference_or_basic_expanded_type_index(EIF_TYPE_INDEX a_type);
#define GE_is_special_of_reference_or_basic_expanded_encoded_type(a_type) GE_is_special_of_reference_or_basic_expanded_type_index(GE_decoded_type(a_type).id)
#define GE_is_special_of_reference_or_basic_expanded_object(obj) GE_is_special_of_reference_or_basic_expanded_type_index(((EIF_REFERENCE)(obj))->id)

/*
 * Is `a_type' a TUPLE type?
 */
#define GE_is_tuple_type_index(a_type) EIF_TEST(GE_type_infos[a_type].flags & GE_TYPE_FLAG_TUPLE)
#define GE_is_tuple_encoded_type(a_type) GE_is_tuple_type_index(GE_decoded_type(a_type).id)
#define GE_is_tuple_object(obj) GE_is_tuple_type_index(((EIF_REFERENCE)(obj))->id)

/*
 * Is `a_type' an expanded type?
 */
#define GE_is_expanded_type_index(a_type) EIF_TEST(GE_type_infos[a_type].flags & GE_TYPE_FLAG_EXPANDED)
#define GE_is_expanded_encoded_type(a_type) GE_is_expanded_type_index(GE_decoded_type(a_type).id)
#define GE_is_expanded_object(obj) GE_is_expanded_type_index(((EIF_REFERENCE)(obj))->id)

/*
 * Is `a_type' a type whose base class is deferred?
 */
#define GE_is_deferred_type_index(a_type) EIF_TEST(GE_type_infos[a_type].flags & GE_TYPE_FLAG_DEFERRED)
#define GE_is_deferred_encoded_type(a_type) GE_is_deferred_type_index(GE_decoded_type(a_type).id)

/*
 * Does `i'-th field of `a_object + a_physical_offset' (which is expected to be reference)
 * denote a reference with copy semantics?
 */
extern EIF_BOOLEAN GE_is_copy_semantics_field(EIF_INTEGER i, EIF_POINTER a_object, EIF_INTEGER a_physical_offset);

/*
 * Does `i'-th item of special `a_object' (which is expected to be reference)
 * denote a reference with copy semantics?
 */
extern EIF_BOOLEAN GE_is_special_copy_semantics_item(EIF_INTEGER i, EIF_POINTER a_object);

/*
 * Generator class name of `a_type'.
 */
extern EIF_REFERENCE GE_generator_of_type_index(EIF_TYPE_INDEX a_type);
#define GE_generator_of_encoded_type(a_type) GE_generator_of_type_index(GE_decoded_type(a_type).id)
extern EIF_REFERENCE GE_generator_8_of_type_index(EIF_TYPE_INDEX a_type);
#define GE_generator_8_of_encoded_type(a_type) GE_generator_8_of_type_index(GE_decoded_type(a_type).id)

/*
 * Full name of `a_type'.
 */
extern EIF_REFERENCE GE_generating_type_of_encoded_type(EIF_ENCODED_TYPE a_type);
extern EIF_REFERENCE GE_generating_type_8_of_encoded_type(EIF_ENCODED_TYPE a_type);

/*
 * Encoded type whose name is `a_name'.
 * -1 if no such type.
 */
extern EIF_ENCODED_TYPE GE_encoded_type_from_name(EIF_POINTER a_name);

/*
 * Does `a_type_1' conform to `a_type_2'?
 */
extern EIF_BOOLEAN GE_encoded_type_conforms_to(EIF_ENCODED_TYPE a_type_1, EIF_ENCODED_TYPE a_type_2);

/*
 * Number of generic parameters.
 */
extern EIF_INTEGER GE_generic_parameter_count_of_type_index(EIF_TYPE_INDEX a_type);
#define GE_generic_parameter_count_of_encoded_type(a_type) GE_generic_parameter_count_of_type_index(GE_decoded_type(a_type).id)

/*
 * Type of `i'-th generic parameter of `a_type'.
 */
extern EIF_INTEGER GE_generic_parameter_of_type_index(EIF_TYPE_INDEX a_type, EIF_INTEGER i);
#define GE_generic_parameter_of_encoded_type(a_type,i) GE_generic_parameter_of_type_index(GE_decoded_type(a_type).id, (i))

/*
 * Number of fields of an object of dynamic type `a_type'.
 */
extern EIF_INTEGER GE_field_count_of_type_index(EIF_TYPE_INDEX a_type);
#define GE_field_count_of_encoded_type(a_type) GE_field_count_of_type_index(GE_decoded_type(a_type).id)

/*
 * Physical offset of the `i'-th field for an object of dynamic type `a_type'.
 */
extern EIF_INTEGER GE_field_offset_of_type_index(EIF_INTEGER i, EIF_TYPE_INDEX a_type);
#define GE_field_offset_of_encoded_type(i, a_type) GE_field_offset_of_type_index((i), GE_decoded_type(a_type).id)

/*
 * Name of the `i'-th field for an object of dynamic type `a_type'.
 */
extern EIF_POINTER GE_field_name_of_type_index(EIF_INTEGER i, EIF_TYPE_INDEX a_type);
#define GE_field_name_of_encoded_type(i, a_type) GE_field_name_of_type_index((i), GE_decoded_type(a_type).id)

/*
 * Static type of the `i'-th field for an object of dynamic type `a_type'.
 */
extern EIF_INTEGER GE_field_static_type_of_type_index(EIF_INTEGER i, EIF_TYPE_INDEX a_type);
#define GE_field_static_type_of_encoded_type(i, a_type) GE_field_static_type_of_type_index((i), GE_decoded_type(a_type).id)

/*
 * Kind of type of the `i'-th field for an object of dynamic type `a_type'.
 */
extern EIF_INTEGER GE_field_type_kind_of_type_index(EIF_INTEGER i, EIF_TYPE_INDEX a_type);
#define GE_field_type_kind_of_encoded_type(i, a_type) GE_field_type_kind_of_type_index((i), GE_decoded_type(a_type).id)

/*
 * Physical size of `a_object'.
 */
extern EIF_NATURAL_64 GE_object_size(EIF_POINTER a_object);

/*
 * Is `i'-th field of objects of type `a_type' a user-defined expanded attribute?
 */
extern EIF_BOOLEAN GE_is_field_expanded_of_type_index(EIF_INTEGER i, EIF_TYPE_INDEX a_type);
#define GE_is_field_expanded_of_encoded_type(i, a_type) GE_is_field_expanded_of_type_index((i), GE_decoded_type(a_type).id)

#define GE_field_address_at(a_field_offset, a_object, a_physical_offset) ((char*)(a_object) + (a_physical_offset) + (a_field_offset))
#define GE_object_at_offset(a_enclosing, a_physical_offset) (EIF_REFERENCE)(GE_field_address_at(0, (a_enclosing), (a_physical_offset)))
#define GE_raw_object_at_offset(a_enclosing, a_physical_offset) (EIF_POINTER)(GE_field_address_at(0, (a_enclosing), (a_physical_offset)))
#define GE_object_encoded_type_at_offset(a_enclosing, a_physical_offset) GE_object_encoded_type(GE_raw_object_at_offset((a_enclosing), (a_physical_offset)))
#define GE_boolean_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_BOOLEAN*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_character_8_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_CHARACTER_8*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_character_32_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_CHARACTER_32*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_integer_8_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_INTEGER_8*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_integer_16_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_INTEGER_16*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_integer_32_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_INTEGER_32*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_integer_64_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_INTEGER_64*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_natural_8_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_NATURAL_8*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_natural_16_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_NATURAL_16*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_natural_32_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_NATURAL_32*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_natural_64_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_NATURAL_64*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_pointer_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_POINTER*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_real_32_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_REAL_32*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_real_64_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_REAL_64*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_raw_reference_field_at(a_field_offset, a_object, a_physical_offset) (EIF_POINTER)*(EIF_REFERENCE*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_reference_field_at(a_field_offset, a_object, a_physical_offset) *(EIF_REFERENCE*)(GE_field_address_at((a_field_offset), (a_object), (a_physical_offset)))
#define GE_set_boolean_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_boolean_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_character_8_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_character_8_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_character_32_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_character_32_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_integer_8_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_integer_8_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_integer_16_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_integer_16_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_integer_32_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_integer_32_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_integer_64_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_integer_64_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_natural_8_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_natural_8_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_natural_16_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_natural_16_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_natural_32_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_natural_32_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_natural_64_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_natural_64_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_pointer_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_pointer_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_real_32_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_real_32_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_real_64_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_real_64_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value
#define GE_set_reference_field_at(a_field_offset, a_object, a_physical_offset, a_value) GE_reference_field_at((a_field_offset), (a_object), (a_physical_offset)) = a_value

#if defined(GE_USE_ATTRIBUTES) && defined(GE_USE_ATTRIBUTE_OFFSET)
#define GE_field_address(i, a_object, a_physical_offset) GE_field_address_at(GE_type_infos[((EIF_REFERENCE)(a_object))->id].attributes[(i) - 1]->offset, (a_object), (a_physical_offset))
#define GE_boolean_field(i, a_object, a_physical_offset) *(EIF_BOOLEAN*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_character_8_field(i, a_object, a_physical_offset) *(EIF_CHARACTER_8*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_character_32_field(i, a_object, a_physical_offset) *(EIF_CHARACTER_32*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_integer_8_field(i, a_object, a_physical_offset) *(EIF_INTEGER_8*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_integer_16_field(i, a_object, a_physical_offset) *(EIF_INTEGER_16*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_integer_32_field(i, a_object, a_physical_offset) *(EIF_INTEGER_32*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_integer_64_field(i, a_object, a_physical_offset) *(EIF_INTEGER_64*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_natural_8_field(i, a_object, a_physical_offset) *(EIF_NATURAL_8*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_natural_16_field(i, a_object, a_physical_offset) *(EIF_NATURAL_16*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_natural_32_field(i, a_object, a_physical_offset) *(EIF_NATURAL_32*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_natural_64_field(i, a_object, a_physical_offset) *(EIF_NATURAL_64*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_pointer_field(i, a_object, a_physical_offset) *(EIF_POINTER*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_real_32_field(i, a_object, a_physical_offset) *(EIF_REAL_32*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_real_64_field(i, a_object, a_physical_offset) *(EIF_REAL_64*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_reference_field(i, a_object, a_physical_offset) *(EIF_REFERENCE*)(GE_field_address((i), (a_object), (a_physical_offset)))
#define GE_set_boolean_field(i, a_object, a_physical_offset, a_value) GE_boolean_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_character_8_field(i, a_object, a_physical_offset, a_value) GE_character_8_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_character_32_field(i, a_object, a_physical_offset, a_value) GE_character_32_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_integer_8_field(i, a_object, a_physical_offset, a_value) GE_integer_8_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_integer_16_field(i, a_object, a_physical_offset, a_value) GE_integer_16_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_integer_32_field(i, a_object, a_physical_offset, a_value) GE_integer_32_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_integer_64_field(i, a_object, a_physical_offset, a_value) GE_integer_64_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_natural_8_field(i, a_object, a_physical_offset, a_value) GE_natural_8_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_natural_16_field(i, a_object, a_physical_offset, a_value) GE_natural_16_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_natural_32_field(i, a_object, a_physical_offset, a_value) GE_natural_32_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_natural_64_field(i, a_object, a_physical_offset, a_value) GE_natural_64_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_pointer_field(i, a_object, a_physical_offset, a_value) GE_pointer_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_real_32_field(i, a_object, a_physical_offset, a_value) GE_real_32_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_real_64_field(i, a_object, a_physical_offset, a_value) GE_real_64_field((i), (a_object), (a_physical_offset)) = (a_value)
#define GE_set_reference_field(i, a_object, a_physical_offset, a_value) GE_reference_field((i), (a_object), (a_physical_offset)) = (a_value)
#else
#define GE_boolean_field(i, a_object, a_physical_offset) (EIF_BOOLEAN)0
#define GE_character_8_field(i, a_object, a_physical_offset) (EIF_CHARACTER_8)0
#define GE_character_32_field(i, a_object, a_physical_offset) (EIF_CHARACTER_32)0
#define GE_integer_8_field(i, a_object, a_physical_offset) (EIF_INTEGER_8)0
#define GE_integer_16_field(i, a_object, a_physical_offset) (EIF_INTEGER_16)0
#define GE_integer_32_field(i, a_object, a_physical_offset) (EIF_INTEGER_32)0
#define GE_integer_64_field(i, a_object, a_physical_offset) (EIF_INTEGER_64)0
#define GE_natural_8_field(i, a_object, a_physical_offset) (EIF_NATURAL_8)0
#define GE_natural_16_field(i, a_object, a_physical_offset) (EIF_NATURAL_16)0
#define GE_natural_32_field(i, a_object, a_physical_offset) (EIF_NATURAL_32)0
#define GE_natural_64_field(i, a_object, a_physical_offset) (EIF_NATURAL_64)0
#define GE_pointer_field(i, a_object, a_physical_offset) (EIF_POINTER)0
#define GE_real_32_field(i, a_object, a_physical_offset) (EIF_REAL_32)0
#define GE_real_64_field(i, a_object, a_physical_offset) (EIF_REAL_64)0
#define GE_reference_field(i, a_object, a_physical_offset) (EIF_REFERENCE)0
#define GE_set_boolean_field(i, a_object, a_physical_offset, a_value)
#define GE_set_character_8_field(i, a_object, a_physical_offset, a_value)
#define GE_set_character_32_field(i, a_object, a_physical_offset, a_value)
#define GE_set_integer_8_field(i, a_object, a_physical_offset, a_value)
#define GE_set_integer_16_field(i, a_object, a_physical_offset, a_value)
#define GE_set_integer_32_field(i, a_object, a_physical_offset, a_value)
#define GE_set_integer_64_field(i, a_object, a_physical_offset, a_value)
#define GE_set_natural_8_field(i, a_object, a_physical_offset, a_value)
#define GE_set_natural_16_field(i, a_object, a_physical_offset, a_value)
#define GE_set_natural_32_field(i, a_object, a_physical_offset, a_value)
#define GE_set_natural_64_field(i, a_object, a_physical_offset, a_value)
#define GE_set_pointer_field(i, a_object, a_physical_offset, a_value)
#define GE_set_real_32_field(i, a_object, a_physical_offset, a_value)
#define GE_set_real_64_field(i, a_object, a_physical_offset, a_value)
#define GE_set_reference_field(i, a_object, a_physical_offset, a_value)
#endif

/*
 * Number of non-transient fields of an object of dynamic type `a_type'.
 * TODO: storable not implemented yet.
 */
#define GE_persistent_field_count_of_type_index(a_type) GE_field_count_of_type_index(a_type)
#define GE_persistent_field_count_of_encoded_type(a_type) GE_persistent_field_count_of_type_index(GE_decoded_type(a_type).id)

/*
 * Is `i'-th field of objects of type `a_type' a transient field?
 * TODO: storable not implemented yet.
 */
#define GE_is_field_transient_of_type_index(i, a_type) EIF_FALSE
#define GE_is_field_transient_of_encoded_type(i, a_type) GE_is_field_transient_of_type_index((i), GE_decoded_type(a_type).id)

/*
 * Storable version of objects of type `a_type'.
 * TODO: storable not implemented yet.
 */
#define GE_storable_version_of_type_index(a_type) EIF_VOID
#define GE_storable_version_of_encoded_type(a_type) GE_storable_version_of_type_index(GE_decoded_type(a_type).id)

/*
 * Get a lock on `GE_mark_object' and `GE_unmark_object' routines so that
 * 2 threads cannot `GE_mark_object' and `GE_unmark_object' at the same time.
 */
extern void GE_lock_marking(void);

/*
 * Release a lock on `GE_mark_object' and `GE_unmark_object', so that another
 * thread can use `GE_mark_object' and `GE_unmark_object'.
 */
extern void GE_unlock_marking(void);

/*
 * Is `obj' marked?
 */
extern EIF_BOOLEAN GE_is_object_marked(EIF_POINTER obj);

/*
 * Mark `obj'.
 */
extern void GE_mark_object(EIF_POINTER obj);

/*
 * Unmark `obj'.
 */
extern void GE_unmark_object(EIF_POINTER obj);

/*
 * New instance of dynamic `a_type'.
 * Note: returned object is not initialized and may
 * hence violate its invariant.
 * `a_type' cannot represent a SPECIAL type, use
 * `GE_new_special_of_reference_instance_of_type_index' instead.
 */
extern EIF_REFERENCE GE_new_instance_of_type_index(GE_context* a_context, EIF_TYPE_INDEX a_type);
#define GE_new_instance_of_encoded_type(a_context, a_type) GE_new_instance_of_type_index((a_context), GE_decoded_type(a_type).id)

/*
 * New instance of dynamic `a_type' that represents
 * a SPECIAL with can contain `a_capacity' elements of reference type.
 * To create a SPECIAL of basic type, use class SPECIAL directly.
 */
extern EIF_REFERENCE GE_new_special_of_reference_instance_of_type_index(GE_context* a_context, EIF_TYPE_INDEX a_type, EIF_INTEGER a_capacity);
#define GE_new_special_of_reference_instance_of_encoded_type(a_context, a_type, a_capacity) GE_new_special_of_reference_instance_of_type_index((a_context), GE_decoded_type(a_type).id, (a_capacity))

/*
 * New instance of tuple of type `a_type'.
 * Note: returned object is not initialized and may
 * hence violate its invariant.
 */
#define GE_new_tuple_instance_of_type_index(a_context, a_type) GE_new_instance_of_type_index((a_context), (a_type))
#define GE_new_tuple_instance_of_encoded_type(a_context, a_type) GE_new_tuple_instance_of_type_index((a_context), GE_decoded_type(a_type).id)

/*
 * New instance of TYPE for object of type `a_type'.
 */
extern EIF_REFERENCE GE_new_type_instance_of_encoded_type(GE_context* a_context, EIF_ENCODED_TYPE a_type);

/*
 * Check whether the `a_type' is in `a_dynamic_type_set'.
 * `nb' is the number of ids in `a_dynamic_type_set'.
 * `a_dynamic_type_set' is sorted in increasing order.
 * A type-id 0 means Void (aka 'detachable NONE').
 */
extern EIF_BOOLEAN GE_type_in_dynamic_type_set(EIF_TYPE_INDEX a_type, EIF_TYPE_INDEX a_dynamic_type_set[], int nb);

#ifdef GE_USE_ATTRIBUTES
/*
 * Attribute with name `a_name' (in lower-case) in type `a_type`.
 * Null if no such attribute.
 */
extern GE_attribute* GE_attribute_with_name(EIF_TYPE_INDEX a_type, char* name);
#endif


#ifdef __cplusplus
}
#endif

#endif
